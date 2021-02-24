#include <systemc.h>
#include <iostream>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

using namespace sc_core;

class Uart : sc_module
{
    SC_HAS_PROCESS(Uart);
    


    public: Uart(sc_module_name name): sc_module(name), apb_out("apb_out"), apb_in("apb_in")
    {
        apb_in.register_b_transport(this, &Uart::b_transport);
        std::cout << "Uart called" << std::endl;
    }

    private: void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay)
    {

        tlm::tlm_command cmd = trans.get_command();
        sc_dt::uint64    adr = trans.get_address() / 4;
        unsigned char*   ptr = trans.get_data_ptr();
        unsigned int     len = trans.get_data_length();
        unsigned char*   byt = trans.get_byte_enable_ptr();
        unsigned int     wid = trans.get_streaming_width();

        printf("Data received (%do) : ", len);
        for(int i=0; i<len;i++)
        {
            printf("%d", *(ptr+(len-i-1))); // 2 - 1 - 0
            if(i != len-1) printf(" - ");
        }

        printf("\n");

        if((*(ptr+len-1) & 0x80))
        {
            printf("Data : 0x%02X\n", *(ptr));
        }
        else
        {
            int parity = ((*(ptr+2) & 0b01100000) >> 5);
            int baudrate = *(ptr) + (*(ptr+1) << 8) + ((*(ptr+2) & 0x01) << 16);
            
            printf("Config :\n");
            printf("  - Parity   : %d\n", parity);
            printf("  - BaudRate : %d\n", baudrate);
            
        }
        

        wait(delay);
        delay = SC_ZERO_TIME;

        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    }

    private: void sendData()
    {
        ; // Tu meux mettre ton code ici nico
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
    public: tlm_utils::simple_target_socket<Uart> apb_in;

    

};
