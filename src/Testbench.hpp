#include <systemc.h>
#include <iostream>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

using namespace sc_core;

class Testbench : sc_module
{
    SC_HAS_PROCESS(Testbench);
    
    public: Testbench(sc_module_name name): sc_module(name), 
        tb_to_apb_in("tb_to_apb_in"),
        tb_to_apb_out("tb_to_apb_out"),
        tb_to_pmc_in("tb_to_pmc_in"),
        tb_to_pmc_out("tb_to_pmc_out"),
        tb_to_io_rx("tb_to_io_rx"),
        tb_to_io_tx("tb_to_io_tx"),
        tb_to_irq_out("tb_to_irq_out")
    {
        this->data = 123;
        SC_THREAD(TestProcess);
    }


    // private: void firstProcess()
    // {
    //     tlm::tlm_generic_payload my_payload;
    //     printf("Building Payload with value : %d\n", data);
    //     buidPayload(&(this->data), 1, 0x00000000, &my_payload);
    //     std::cout << "Envoi Data in Testbench" << std::endl;
    //     sendToSocket(&my_payload, &tb_to_apb_in);
    // }

    public: void sendToSocket(tlm::tlm_generic_payload *my_payload, tlm_utils::simple_initiator_socket<Testbench> *socket)
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

    private: void TestProcess()
    {

        tlm::tlm_generic_payload my_payload;

        printf("Init\n");

        // Address 0x00 | offset 2 | write
        buidPayload(&(this->data), 1, 0x01, &my_payload);
        sendToSocket(&my_payload, &tb_to_pmc_in);
        std::cout << "Disable pmc\n" << std::endl;

        // Address 0x00 | offset 2 | write
        buidPayload(&(this->data), 2, 0x00, &my_payload);
        sendToSocket(&my_payload, &tb_to_apb_in);
        std::cout << "Reset Receiver : UART_CR->RSTRX\n" << std::endl;

        // Address 0x00 | offset 3 | write
        buidPayload(&(this->data), 3, 0x00, &my_payload);
        sendToSocket(&my_payload, &tb_to_apb_in);
        std::cout << "Reset Transmitter : UART_CR->RSTTX\n" << std::endl;

        buidPayload(&(this->data), 1, 0x00, &my_payload);
        sendToSocket(&my_payload, &tb_to_pmc_in);
        std::cout << "Enable pmc\n" << std::endl;

        // Address 0x00 | offset 4 | write
        buidPayload(&(this->data), 4, 0x00, &my_payload);
        sendToSocket(&my_payload, &tb_to_apb_in);
        std::cout << "Enable receiver : UART_CR->RXEN\n" << std::endl;

        // Address 0x00 | offset 6 | write
        buidPayload(&(this->data), 6, 0x00, &my_payload);
        sendToSocket(&my_payload, &tb_to_apb_in);
        std::cout << "Enable receiver : UART_CR->TXEN\n" << std::endl;

        printf("TestProcess\n");

        // Address 0x1C | write
        buidPayload(&(this->data), 0, 0x1C, &my_payload);
        sendToSocket(&my_payload, &tb_to_apb_in);
        std::cout << "Send data : UART_THR\n" << std::endl;

        // Address 0x00 | write
        buidPayload(&(this->data), 0, 0x00, &my_payload);
        sendToSocket(&my_payload, &tb_to_apb_in);
        std::cout << "Enable interrupt : UART_IER\n" << std::endl;

        // Address 0x14 | read
        buidPayload(&(this->data), 0, 0x14, &my_payload);
        sendToSocket(&my_payload, &tb_to_apb_in);
        std::cout << "Read interrupt : UART_SR->RXRDY\n" << std::endl;

        // Address 0x18 | Read
        buidPayload(&(this->data), 0, 0x18, &my_payload);
        sendToSocket(&my_payload, &tb_to_apb_in);
        std::cout << "Read data : UART_RHR\n" << std::endl;

    }

    public: tlm_utils::simple_target_socket<Testbench>    tb_to_apb_out;
    public: tlm_utils::simple_initiator_socket<Testbench> tb_to_apb_in;

    public: tlm_utils::simple_target_socket<Testbench>    tb_to_pmc_out;
    public: tlm_utils::simple_initiator_socket<Testbench> tb_to_pmc_in;

    public: tlm_utils::simple_target_socket<Testbench>    tb_to_io_tx;
    public: tlm_utils::simple_initiator_socket<Testbench> tb_to_io_rx;

    public: tlm_utils::simple_target_socket<Testbench>    tb_to_irq_out;

    private: unsigned char data;

};


    
