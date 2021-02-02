#include <iostream>
#include <systemc.h>
#include "Testbench.hpp"

int sc_main(int, char**) {  

   std::cout << std::endl << std::endl << "Global simulation" << std::endl;
	//Class Class("Class");
	//Testbench testbench("Testbench");

	sc_start(500, SC_NS);

   return 0;
}