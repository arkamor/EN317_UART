#include <iostream>
#include <systemc>

int sc_main(int, char**) {  

   std::cout << "Hello World !" << std::endl;
	//Class Class("Class");
	//Testbench testbench("Testbench");

	sc_start(500, SC_NS);

   return 0;
}