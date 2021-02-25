#include <iostream>
#include <systemc>

#include "Uart.hpp"
#include "Testbench.hpp"


int sc_main(int, char**) {  

	std::cout << std::endl << "Main run" << std::endl;

	Uart Uart("Uart");
    Testbench Testbench("Testbench");

	int baudrate = 115200;
	int parity = 2;

	Uart.apb_in.bind(Testbench.tb_to_apb_in);
	Uart.apb_out.bind(Testbench.tb_to_apb_out);

	Uart.pmc_in.bind(Testbench.tb_to_pmc_in);
	Uart.pmc_out.bind(Testbench.tb_to_pmc_out);

	Uart.io_rx.bind(Testbench.tb_to_io_rx);
	Uart.io_tx.bind(Testbench.tb_to_io_tx);

	Uart.irq_out.bind(Testbench.tb_to_irq_out);
	

	// Start simulation
	std::cout << "Global simulation starts" << std::endl;
	sc_start(500, SC_MS);
	
	std::cout << std::endl;

	return 0;
}

