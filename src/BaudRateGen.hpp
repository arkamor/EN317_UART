#include <iostream>
#include <systemc.h>

SC_MODULE(baud_rate_generator)
{
    sc_in<sc_uint<32> > mck;
    sc_in<sc_uint<16> > cd;
    sc_out<sc_uint<32> > baud_rate_clock, receiver_sapmling_clock;

    SC_CTOR(baud_rate_generator);

    void divide_freq();
};