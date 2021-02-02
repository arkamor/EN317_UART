#include <systemc.h>
#include <iostream>
#include "Testbench.hpp"


Testbench::Testbench(sc_module_name nom) : sc_module(nom) // constructor
{
    SC_METHOD(start); // calcul devient une SC_METHOD
    //sensitive << a << b; // qui se "reveille" quand a ou b change.
}

// Description d'une methode random
void Testbench::start() // Méthode
{
    std::cout << "TestBench Start" << std::endl;

    int a = 212;
    int b = 57;

    D_in.write(a);
    Conf.write(b);

    std::cout << "Add : " << a << " + " << b << std::endl;

    int c = D_out.read();

    std::cout << "result : " << c << std::endl;

}