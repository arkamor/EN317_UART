#include <iostream>
#include <systemc>
#include "Testbench.hpp"

int sc_main(int, char**) {  

   std::cout << "Sample starting" << std::endl;
	//Class Class("Class");
	//Testbench testbench("Testbench");

	sc_start(500, SC_NS);

   return 0;
}