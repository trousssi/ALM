#include "i2c_audio.h"
#include "i2s_audio.h"
#include "cs_audio_config.h"
#include "systick.h"
#include "discoveryf4_wiring.h"

#include <libopencm3/stm32/i2c.h>
#include <stdbool.h>

// I2C audio peripheral address
// On I2C bus sda : 0x94 write 0x95 read

#define CS43L22_I2C_ADDRESS 0x4a

// I2C apb1 bus clock input = 42 MHz
#define FREQ_FROM_APB1 I2C_CR2_FREQ_42MHZ


void configure_i2c_audio (void) {
  // reset cs43L22 : >= 1ms
  write_pin(AUDIO_RESET,false);
  // 2ms pulse
  sysms = 2;
  while (sysms >0) {}
  write_pin(AUDIO_RESET,true);
  // Wait >= 0,55 microsec before start condition
  // wait 1ms
  sysms =1;
  while (sysms >0) {}

  i2c_reset(I2C1);
  i2c_peripheral_disable(I2C1);

  // Speed configuration -> standard mode@100KHz
  i2c_set_clock_frequency(I2C1,FREQ_FROM_APB1);
  i2c_set_standard_mode(I2C1);
  i2c_set_speed(I2C1,i2c_speed_sm_100k,FREQ_FROM_APB1);
  i2c_set_trise(I2C1,FREQ_FROM_APB1+1);

  i2c_peripheral_enable(I2C1);
  cs_config(cs_digital_init_seq);
  i2s_init();
}

void wait_not_busy () {
   while ((I2C_SR2(I2C1) & I2C_SR2_BUSY)) {}
}

void wait_ready_to_address () {
   while ((I2C_SR1(I2C1) & I2C_SR1_SB)==0) {}
}

void wait_address_sent () {
   while ((I2C_SR1(I2C1) & I2C_SR1_ADDR)==0) {}
}

void clear_addr_bit () {
   (void) I2C_SR2(I2C1);    // Read SR2 to clear ADDR bit, ignore value
}

void wait_byte_sent () {
   while ((I2C_SR1(I2C1) & I2C_SR1_BTF) ==0) {}
}

void wait_byte_received () {
   while ((I2C_SR1(I2C1) & I2C_SR1_RxNE) ==0) {}
}

void do_send_cs (uint8_t regnum, bool sendval, uint8_t regval) {
  // attente de la disponibilite du CS43L22 :
  wait_not_busy ();

  // envoi de la condition de start :
  // a completer...
  i2c_send_start(I2C1);
  // attente de la disponibilite pour envoi de l'adresse :
  wait_ready_to_address ();

  // envoi de l'adresse du CS43L22 + bit d'ecriture :
  // a completer...
  i2c_send_7bit_address(I2C1,CS43L22_I2C_ADDRESS,I2C_WRITE);
  // attente acknowledge de l'adresse :
  wait_address_sent ();
  clear_addr_bit();

  // envoi d'une donnee, qui est l'adresse de registre regnum
  // a completer...
  i2c_send_data(I2C1, regnum);
  // attente acknowledge de la donnee :
  wait_byte_sent();

  if (sendval) {
    // envoi d'une donnee, qui est la valeur regval :
    // a completer...
    i2c_send_data(I2C1, regval);
    // attente acknowledge de la donnee :
    wait_byte_sent();
  }
  // envoi de la condition de stop :
  // a completer...
  i2c_send_stop(I2C1);
}

void send_cs (uint8_t regnum, uint8_t regval) {
  do_send_cs(regnum,true,regval);
}

int8_t read_cs (uint8_t regnum) {
  uint8_t data;

  do_send_cs(regnum,false,0);

  // attente de la disponibilite du CS43L22 :
  wait_not_busy ();

  // envoi de la condition de start :
  // a completer...
  i2c_send_start(I2C1);
  // attente de la disponibilite pour envoi de l'adresse :
  wait_ready_to_address ();

  // envoi de l'adresse du CS43L22 + bit de lecture :
  // a completer...
  i2c_send_7bit_address(I2C1,CS43L22_I2C_ADDRESS,I2C_READ);
  // attente acknowledge de l'adresse :
  wait_address_sent ();
  i2c_disable_ack (I2C1); // No ack for last byte received
  clear_addr_bit();

  // attente disponibilite de la donnee :
  wait_byte_received();

  // reception de la donnee, et stockage dans data
  // a completer...
  data = i2c_get_data(I2C1);
  // envoi de la condition de stop :
  // a completer...
  i2c_send_stop(I2C1);

  return data;
}
