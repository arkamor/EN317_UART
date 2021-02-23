#include <systemc.h>
#include <iostream>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>

using namespace sc_core;

class Uart : sc_module
{
    SC_HAS_PROCESS(Uart);
private:
    static const unsigned int size = 256;

public:
    Uart(sc_module_name name):
            sc_module(name),
            socket("socket")
    {
        
        socket.register_b_transport(this, &Uart::b_transport);
        socket.register_transport_dbg(this, &Uart::transport_dbg);

        std::cout << "Uart called" << std::endl;

        for (int i = 0; i < size; i++){
            mem[i] = 0xAA000000 | (mem_nr << 20) | (rand() % 256);

        }
    }

private:
    void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay)
    {
        tlm::tlm_command cmd = trans.get_command();
        sc_dt::uint64    adr = trans.get_address() / 4;
        unsigned char*   ptr = trans.get_data_ptr();
        unsigned int     len = trans.get_data_length();
        unsigned char*   byt = trans.get_byte_enable_ptr();
        unsigned int     wid = trans.get_streaming_width();

        printf("Data received : %d at %p\n", *ptr, ptr);

        wait(delay);
        delay = SC_ZERO_TIME;

        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    }

    unsigned int transport_dbg(tlm::tlm_generic_payload& trans)
    {
        
        std::cout << "Debug un transport_dbg" << std::endl;

        tlm::tlm_command cmd = trans.get_command();
        sc_dt::uint64    adr = trans.get_address() / 4;
        unsigned char*   ptr = trans.get_data_ptr();
        unsigned int     len = trans.get_data_length();

        unsigned int num_bytes = (len < (size - adr) * 4) ?
                                 len : (size - adr) * 4;

        // if (cmd == tlm::TLM_READ_COMMAND)
        //     memcpy(ptr, &mem[adr], num_bytes);
        // else if (cmd == tlm::TLM_WRITE_COMMAND)
        //     memcpy(&mem[adr], ptr, num_bytes);

        return num_bytes;
    }

public:
    tlm_utils::simple_target_socket<Uart> socket;
    int mem[size];
    static unsigned int mem_nr;
};

unsigned int Uart::mem_nr = 0;
