#include <stdio.h>

//MAIN REGISTER
#define BASE_ADDR 0x400E0800

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
#define PAR_Bp 9     //Parity Type : 0 EVEN - 1 ODD - 2 SPACE - 3 MARK - 4 NO
#define CHMODE_Bp 14 //Channel Mode : 0 NORMAL - 1 AUTO - 2 LOCAL_LOOPBACK - 3 REMOTE_LOOPBACK

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
#define UART_SR_ADDR 0x18
#define RXCHR_Bp 0 //Received Character [0:7]

//TRANSMIT HOLDING REGISTER
#define UART_SR_ADDR 0x1C
#define TXCHR_Bp 0 //Character to be Transmitted [0:7]

class Reg
{

public:
   void writeToRegister(sc_dt::uint64 offset, unsigned char *data, unsigned int len)
   {
      memcpy(BASE_ADDR + offset, &data, len);
   }

   void readRegister(sc_dt::uint64 offset, unsigned char *data, unsigned int len)
   {
      memcpy(&data, BASE_ADDR + offset, len);
   }
};
