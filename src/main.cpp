#include <iostream>
#include <systemc>
#include "Testbench.hpp"
#include "Uart.hpp"
//#include "Reg.hpp"



int sc_main(int, char**) {  

	std::cout << std::endl << std::endl << "Global simulation" << std::endl;

	Uart Uart("Uart");
    Testbench Testbench("Testbench");

	Uart.socket.bind(Testbench.socket);
	
	// Start simulation
	sc_start(500, SC_MS);

	return 0;
}

