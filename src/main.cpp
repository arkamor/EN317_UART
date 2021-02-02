#include <iostream>
#include <systemc.h>
#include "Testbench.hpp"
#include "Uart.hpp"
//#include "Reg.hpp"


int sc_main(int, char**) {  

	std::cout << std::endl << std::endl << "Global simulation" << std::endl;

	Uart Uart("Uart");
    Testbench Testbench("Testbench");

	// Internal signal declaration
    sc_signal<sc_uint<16> > tb2uart_Tx_s;
	sc_signal<sc_uint<16> > tb2uart_Rx_s;
	sc_signal<sc_uint<16> > tb2uart_Conf_s;

	// Bind sinals to TB
    Testbench.D_in.bind(tb2uart_Tx_s);
    Testbench.D_out.bind(tb2uart_Rx_s);
    Testbench.Conf.bind(tb2uart_Conf_s);

	// Bind signals to UUT
    Uart.D_in.bind(tb2uart_Tx_s);
    Uart.D_out.bind(tb2uart_Rx_s);
    Uart.Conf.bind(tb2uart_Conf_s);

	// Start simulation
	sc_start(500, SC_NS);

	Testbench.start();

	return 0;
}

