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
public:
    Testbench(sc_module_name name):
            sc_module(name),
            socket("socket")
    {
         SC_THREAD(firstProcess);
        
    }

private:

    void firstProcess()
    {
        tlm::tlm_generic_payload payload;

        std::cout << "Envoi Data in Testbench" << std::endl;
        
        int data = 100;

        payload.set_command(tlm::TLM_WRITE_COMMAND);
        payload.set_address(0x0000FFFF);
        payload.set_data_ptr(reinterpret_cast<unsigned char*>(&data));
        payload.set_data_length(8);
        payload.set_streaming_width(4);
        payload.set_byte_enable_ptr(0);
        payload.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

        sc_time time(SC_ZERO_TIME);
        wait(sc_time(100, SC_MS));
        socket->b_transport(payload, time);

        if (payload.is_response_error())
        {
            char txt[100];
            sprintf(txt, "Error from b_transport, response status = %s",
                    payload.get_response_string().c_str());
            SC_REPORT_ERROR("TLM-2", txt);
        }

    }

public:
    tlm_utils::simple_initiator_socket<Testbench> socket;

};

