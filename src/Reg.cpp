#include "Reg.hpp"

Reg::Reg(sc_module_name nom) : sc_module(nom) {

}

Reg::getRegister(unsigned long addr) {

}

void Reg::setData(unsigned char data, unsigned char bp){
  return ( data <<  bp ) & cr_register;
}

void Reg::getData(unsigned long addr, unsigned char bp){
  return 
}

Reg::resetReceiver() {
  cr_register = setData(1, RSTRX_Bp);
}

Reg::resetTransmitter() {
  //cr_register = 
}





