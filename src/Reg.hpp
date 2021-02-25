#include <stdio.h>

//MAIN REGISTER
//#define BASE_ADDR 0x400E0800

#define REG_SIZE 32

//CONTROL REGISTER
#define UART_CR_ADDR 0x00
#define RSTRX_Bp 2  //Reset Receiver
#define RSTTX_Bp 3  //Reset Transmitter
#define RXEN_Bp 4   //Receiver Enable
#define RXDIS_Bp 5  //Receiver Disable
#define TXEN_Bp 6   //Transmitter Enable
#define TXDIS_Bp 7  //Transmitter Disable
#define RSTSTA_Bp 8 //Reset Status Bits

//MODE REGISTER
#define UART_MR_ADDR 0x04
#define PAR_Bp 9 //Parity Type : 0 EVEN - 1 ODD - 2 SPACE - 3 MARK - 4 NO

//SATUS REGISTER
#define UART_SR_ADDR 0x14
#define RXRDY_Bp 0   //Receiver Ready
#define TXRDY_Bp 1   //Transmitter Ready
#define ENDRX_Bp 3   //End of Receiver Transfer
#define ENDTX_Bp 4   //End of Transmitter Transfer
#define OVRE_Bp 5    //Overrun Error
#define FRAME_Bp 6   //Framing Error
#define PARE_Bp 7    //Parity Error
#define TXEMPTY_Bp 9 //Transmitter Empty
#define TXBUFF_Bp 11 //Transmission Buffer Empty
#define RXBUFF_Bp 12 //Receive Buffer Full

//RECEIVER HOLDING REGISTER
#define UART_RHR_ADDR 0x18
#define RXCHR_Bp 0 //Received Character [0:7]

//TRANSMIT HOLDING REGISTER
#define UART_THR_ADDR 0x1C
#define TXCHR_Bp 0 //Character to be Transmitted [0:7]

class Reg
{

public:
   Reg()
   {
      this->base_addr = reinterpret_cast<unsigned char *>(malloc(REG_SIZE));
      std::cout << "Reg created" << std::endl;
   }

public:
   void writeToRegister(sc_dt::uint64 offset, unsigned char *data, unsigned int len)
   {
      printf("Wrote to register");
      memcpy(base_addr + offset, &data, len);
   }

public:
   void readRegister(sc_dt::uint64 offset, unsigned char *data, unsigned int len)
   {
      memcpy(&data, base_addr + offset, len);
   }

public:
   void isIRQEn()
   {
      //return (*(base_addr + UART_CR_ADDR) >> RXEN_Bp) & 1;
   }

public:
   unsigned char isRxEnable()
   {
      return (*(base_addr + UART_CR_ADDR) >> RXEN_Bp) & 1;
   }

public:
   unsigned char isTxEnable()
   {
      return (*(base_addr + UART_CR_ADDR) >> TXEN_Bp) & 1;
   }

public:
   unsigned char isRxReady()
   {
      return (*(base_addr + UART_SR_ADDR) >> RXRDY_Bp) & 1;
   }

public:
   unsigned char isTxReady()
   {
      return (*(base_addr + UART_SR_ADDR) >> TXRDY_Bp) & 1;
   }

public:
   void resetTxReady()
   {
      *(base_addr + UART_SR_ADDR) = *(base_addr + UART_SR_ADDR) | (1 << TXRDY_Bp); 
   }

public:
   void setRxReady()
   {
      *(base_addr + UART_SR_ADDR) |= (1 << RXRDY_Bp); 
   }

public:
   void disableTx()
   {
      *(base_addr + UART_CR_ADDR) &= ~(1 << TXEN_Bp);
      *(base_addr + UART_CR_ADDR) |= (1 << TXDIS_Bp);
   }

public:
   void disableRx()
   {
      *(base_addr + UART_CR_ADDR) &= ~(1 << RXEN_Bp);
      *(base_addr + UART_CR_ADDR) |= (1 << RXDIS_Bp);
   }

public:
   void enableTx()
   {
      *(base_addr + UART_CR_ADDR) &= ~(1 << TXDIS_Bp);
      *(base_addr + UART_CR_ADDR) |= (1 << TXEN_Bp);
   }

public:
   void enableRx()
   {
      *(base_addr + UART_CR_ADDR) &= ~(1 << RXDIS_Bp);
      *(base_addr + UART_CR_ADDR) |= (1 << RXEN_Bp);
   }

public:
   void writeRHR(unsigned char data)
   {
      *(base_addr + UART_RHR_ADDR) = *(base_addr + UART_RHR_ADDR) & (data << RXCHR_Bp));
   }

public:
   unsigned char readTHR()
   {
      return *(base_addr + UART_THR_ADDR) & (1 << TXCHR_Bp);
   }

public:
   unsigned char readyParity()
   {
      return (*(base_addr + UART_MR_ADDR) >> PAR_Bp) & 0x07;
   }

private:
   unsigned char *base_addr;
};
