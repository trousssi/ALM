#ifndef RS232_H
#define RS232_H

#include <stdbool.h>
// reset  RS232 engine : P2/UART3 if line true, P1:UART2 if line false)

void rs232_reset (bool line);

// Init rs232 line :
//    56K, 8 bits 1 stop no parity no flow control

void rs232_init (bool line);
#endif
