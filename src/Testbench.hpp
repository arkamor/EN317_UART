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
    
    public: Testbench(sc_module_name name): sc_module(name), apb_in("apb_in"), apb_out("apb_out")
    {
         SC_THREAD(firstProcess);
    }

    public: void set_data(int data)
    {
        this->data[0] = data;
        this->data[1] = 0;
        this->data[2] = 0b10000000;
    }
    
    public: void set_config(int data)
    {
        this->data[0] = ((data & 0x0000ff));
        this->data[1] = ((data & 0x00ff00) >> 8);
        this->data[2] = ((data & 0xff0000) >> 16);

        printf("config set with %d to {%d,%d,%d}\n", data, this->data[2], this->data[1], this->data[0]);
    }

    private: void firstProcess()
    {

        tlm::tlm_generic_payload my_payload;

        printf("Building Payload with value : %d - %d - %d\n", this->data[2], this->data[1], this->data[0]);
    
        buidPayload(this->data, 3, 0x00000000, &my_payload);

        std::cout << "Envoi Data in Testbench" << std::endl;
        sendToSocket(&my_payload, &apb_out);

    }

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

    public: tlm_utils::simple_initiator_socket<Testbench> apb_out;
    public: tlm_utils::simple_target_socket<Testbench> apb_in;
    private: unsigned char data[3];

};

