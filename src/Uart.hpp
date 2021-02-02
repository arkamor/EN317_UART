#include <systemc.h>
#include <iostream>

SC_MODULE(Uart)
{
    // attributs de la classe
    sc_in<sc_uint<16> > D_in;
    sc_out<sc_uint<16> > D_out;

    sc_in<sc_uint<16> > Conf;

    // methodes de la classe
    SC_CTOR(Uart);
    void calcul(); // point d’entree de la SC_METHOD
    // pour l’instant, c’est une methode C++ normale
};


