#include <iostream>
#include <systemc.h>
#include "BaudRateGen.hpp"

baud_rate_generator::baud_rate_generator(sc_module_name nom) : sc_module(nom)
{
    SC_METHOD(divide_freq);
    sensitive << mck << cd;
}

void baud_rate_generator::divide_freq()
{
	if(cd==0){
		receiver_sapmling_clock.write(0);
		baud_rate_clock.write(0);
	}
	else if(cd==1){
		receiver_sapmling_clock.write(1);
		baud_rate_clock.write(mck.read()/16);
	}
	else{
		receiver_sapmling_clock.write(mck.read()/cd.read());
		baud_rate_clock.write(mck.read()/cd.read()/16);
	}
}