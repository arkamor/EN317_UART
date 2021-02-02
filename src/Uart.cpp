#include <systemc.h>
#include <iostream>
#include "Uart.hpp"

Uart::Uart(sc_module_name nom) : sc_module(nom) // constructor
{
    SC_METHOD(calcul); // calcul devient une SC_METHOD
    sensitive << D_in << Conf; // qui se "reveille" quand a ou b change.
}

void Uart::calcul() // Méthode
{
    D_out.write(D_in.read() + Conf.read());
}