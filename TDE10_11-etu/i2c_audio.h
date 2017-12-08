#ifndef I2S_AUDIO_H
#define I2S_AUDIO_H

#include <stdint.h>
#include <stdbool.h>

void configure_i2c_audio (void);
void wait_not_busy ();
void wait_ready_to_address ();
void wait_address_sent ();
void clear_addr_bit ();
void wait_byte_sent ();
void wait_byte_received ();
void do_send_cs (uint8_t regnum, bool sendval, uint8_t regval);
void send_cs (uint8_t regnum, uint8_t regval);
int8_t read_cs (uint8_t regnum);

#endif
