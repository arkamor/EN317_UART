#include <iostream>
#include <systemc>

#include "Uart.hpp"
#include "Testbench.hpp"
//#include "Reg.hpp"

int sc_main(int, char**) {  

	std::cout << std::endl << "Main run" << std::endl;

	Uart Uart("Uart");
    Testbench Testbench("Testbench");

	int baudrate = 115200;
	int parity = 2;

	int config_data = (parity << 21) + baudrate;

	printf("conifg = %d + %d = %d\n",  (parity) << 21, baudrate, config_data);
	
	
	Testbench.set_config(config_data);
	//Testbench.set_data(0xAB);


	Uart.apb_in.bind(Testbench.apb_out);
	Testbench.apb_in.bind(Uart.apb_out);
	
	// Start simulation
	std::cout << "Global simulation starts" << std::endl;
	sc_start(500, SC_MS);
	
	std::cout << std::endl;

	return 0;
}

