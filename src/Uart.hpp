#include <systemc.h>
#include <iostream>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>
#include "Reg.hpp"
#include "Socket.hpp"

using namespace sc_core;

typedef struct out_frame{
    bool start = 0;
    int payload = 0;
    bool parity = NULL;
    bool stop = 1;
}out_frame;

class Uart : sc_module
{
    SC_HAS_PROCESS(Uart);

    public: Uart(sc_module_name name): sc_module(name),
        apb_out("apb_out"),
        apb_in("apb_in"),
        pmc_out("pmc_out"),
        pmc_in("pmc_in"),
        io_tx("io_tx"),
        io_rx("io_rx"),
        irq_out("irq_out")
    {
        apb_in.register_b_transport(this, &Uart::apb_receive);
        pmc_in.register_b_transport(this, &Uart::pmc_receive);
        io_rx.register_b_transport(this, &Uart::io_receive);

        std::cout << "Uart called" << std::endl;
    }

    private: void apb_receive(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay)
    {
        tlm::tlm_command cmd = trans.get_command();
        sc_dt::uint64    adr = trans.get_address() / 4;
        unsigned char*   ptr = trans.get_data_ptr();
        unsigned int     len = trans.get_data_length();

        printf("APB received (%d o) : ", len);
        for(int i=0; i<len;i++)
        {
            printf("%d", *(ptr+(len-i-1))); // 2 - 1 - 0
            if(i != len-1) printf(" - ");
        }

        printf("\n");

        my_reg.writeToRegister(adr, ptr, len);

        if(my_reg.isTxReady() && my_reg.isTxEnable()){
            sendData(my_reg.readTHR());
            my_reg.resetTxReady();
        }
        
        wait(delay);
        delay = SC_ZERO_TIME;

        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    }

    private: void pmc_receive(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay)
    {
        tlm::tlm_command cmd = trans.get_command();
        sc_dt::uint64    adr = trans.get_address() / 4;
        unsigned char*   ptr = trans.get_data_ptr();
        unsigned int     len = trans.get_data_length();

        printf("PMC received (%d o) : ", len);
        for(int i=0; i<len;i++)
        {
            printf("%d", *(ptr+(len-i-1))); // 2 - 1 - 0
            if(i != len-1) printf(" - ");
        }

        printf("\n");

        memcpy(&PMC_data, ptr, len);

        // Process du PMC ICI

        if(PMC_data.MCK)
        {
            my_reg.enableTx();
            my_reg.enableRx();
        }
        else
        {
            my_reg.disableTx();
            my_reg.disableRx();
        }

        wait(delay);
        delay = SC_ZERO_TIME;

        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    }

    private: void io_receive(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay)
    {
        tlm::tlm_command cmd = trans.get_command();
        sc_dt::uint64    adr = trans.get_address() / 4;
        unsigned char*   ptr = trans.get_data_ptr();
        unsigned int     len = trans.get_data_length();

        printf("IO received (%d o) : ", len);
        for(int i=0; i<len;i++)
        {
            printf("%d", *(ptr+(len-i-1))); // 2 - 1 - 0
            if(i != len-1) printf(" - ");
        }

        printf("\n");

        // On remplie le registre avec les data IN
        out_frame my_frame;

        memcpy(&my_frame, ptr, len);

        my_reg.writeRHR(my_frame.payload);

        if(my_reg.isIRQEn())
        {
            sendIRQ();
        }

        my_reg.setRxReady();

        wait(delay);
        delay = SC_ZERO_TIME;

        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    }

    private: void sendIRQ(){
        tlm::tlm_generic_payload my_payload;
        unsigned char value = 1;
        buidPayload(&value, 1, 0x00000000, &my_payload);

        std::cout << "Envoi IRQ" << std::endl;
        sendToSocket(&my_payload, &irq_out);
    }

    private: bool getParity(unsigned char n) 
    { 
        bool parity = 0; 
        while(n) 
        { 
            parity = !parity; 
            n = n & (n - 1); 
        }
        return parity; 
    } 

    private: void sendData(unsigned char data_out)
    {   
        // Calcul de la parite
        unsigned char reg_data = my_reg.readyParity();
        bool parity;
        switch (reg_data){
            case 0: //EVEN
                parity = getParity(data_out) & 1; // calculer
            break;
            case 1: //ODD
                parity = getParity(data_out) & 0; // calculer
            break;
            case 2: // Force 0
                parity = 0;
            break;
            case 3: // Force 1
                parity = 1;
            break;
            default: // Force NONE
                parity = NULL;
            break;
        }

        out_frame payload_data;
        
        payload_data.parity = parity;
        payload_data.payload = data_out;

        tlm::tlm_generic_payload my_payload;
        buidPayload(reinterpret_cast<unsigned char *>(&payload_data), sizeof(payload_data), 0x00000000, &my_payload);

        std::cout << "Envoi Data to Tx" << std::endl;
        sendToSocket(&my_payload, &io_tx);

        int baud_rate = PMC_data.MCK/(my_reg.getCD()/16);

        int parity_nb_bits = 1;
        if(reg_data == 4) parity_nb_bits = 0;
        int time = 1000 * 1000 * (10 + parity_nb_bits) / baud_rate;

        wait(sc_time(time, SC_US));
    }

    public: void sendToSocket(tlm::tlm_generic_payload *my_payload, tlm_utils::simple_initiator_socket<Uart> *socket)
    {
        sc_time time(SC_ZERO_TIME);
        wait(sc_time(100, SC_MS));

        (*socket)->b_transport(*my_payload, time);

        if (my_payload->is_response_error())
        {
            char txt[100];
            sprintf(txt, "Error from b_transport, response status = %s", my_payload->get_response_string().c_str());
            SC_REPORT_ERROR("TLM-2", txt);
        }
    }
    
    public: void buidPayload(unsigned char *data, int len, sc_dt::uint64 address, tlm::tlm_generic_payload *payload)
    {
        payload->set_command(tlm::TLM_WRITE_COMMAND);
        payload->set_address(address);
        payload->set_data_ptr(data);
        payload->set_data_length(len);
        payload->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    }

    public: tlm_utils::simple_initiator_socket<Uart> apb_out;
    public: tlm_utils::simple_target_socket<Uart>    apb_in;

    public: tlm_utils::simple_initiator_socket<Uart> pmc_out;
    public: tlm_utils::simple_target_socket<Uart>    pmc_in;

    public: tlm_utils::simple_initiator_socket<Uart> io_tx;
    public: tlm_utils::simple_target_socket<Uart>    io_rx;

    public: tlm_utils::simple_initiator_socket<Uart> irq_out;

    private: Reg my_reg;

    private: genericPayload PMC_data;

};
