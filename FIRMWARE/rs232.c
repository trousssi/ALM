#include "rs232.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

void rs232_reset (bool line) {
  rcc_peripheral_reset(&RCC_APB1RSTR, line ? RCC_APB1RSTR_USART3RST : RCC_APB1RSTR_USART2RST);
  rcc_peripheral_clear_reset(&RCC_APB1RSTR, line ? RCC_APB1RSTR_USART3RST : RCC_APB1RSTR_USART2RST);
}
  
void rs232_init (bool line) {
   usart_enable(line ? USART3 : USART2);
   usart_set_mode (line ? USART3 : USART2, USART_MODE_TX_RX);
   usart_set_parity (line ? USART3 : USART2, USART_PARITY_NONE);
   usart_set_stopbits (line ? USART3 : USART2, USART_STOPBITS_1);
   usart_set_databits (line ? USART3 : USART2, 8);
   usart_set_flow_control (line ? USART3 : USART2, USART_FLOWCONTROL_NONE);
   usart_set_baudrate (line ? USART3 : USART2,57600 );
}

void test_rs232 () {
  uint16_t x;
   while (1) {
     usart_wait_send_ready(USART2);
     usart_send(USART2,'#');
//     usart_wait_recv_ready(USART2);
//     x=usart_recv(USART2);
   }
}  
