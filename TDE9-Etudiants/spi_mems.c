/*
 *
 *  Copyright (C) 2017 Philippe Waille <Philippe.Waille@univ-grenoble-alpes.fr>
 *
 *  This discovery examples code is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this code.  If not, see <http://www.gnu.org/licenses/>.
 *
*/


#include "spi_mems.h"
#include "lis3dsh.h"


#include "discoveryf4_wiring.h"

#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/exti.h>


extern uint32_t sysms;

// Mems x and y values
int8_t x=0;
int8_t y=0;


void init_spi1 () {
   // baud rate : APB2 freq/16 : 82Mhz/16 = 5,125MHz  (< 10MHz required by mems)
   // Clock idle high, sample on rising edge : CPOl,CPHA = (1,1)
   // 8 bits frame, MSB bit first, no crc
	// Full duplex mode
   
   spi_disable(SPI1);
    
   // Initial slave select inactive (1)
   write_pin(CS_SPI1,1);

   spi_disable_crc(SPI1);

  spi_init_master(SPI1,
                  SPI_CR1_BAUDRATE_FPCLK_DIV_16,
                  SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE,
                  SPI_CR1_CPHA_CLK_TRANSITION_2,
                  SPI_CR1_DFF_8BIT,
                  SPI_CR1_MSBFIRST);

  spi_set_full_duplex_mode(SPI1);

   spi_enable(SPI1);
   
}

uint8_t spi_send_command (uint8_t reg, uint8_t data) {
   uint16_t result;

   // select mems
   write_pin(CS_SPI1,0);

   // send register number, ignore incoming data
   // **** TODO **** 
    spi_send(SPI1, reg);

    (void) spi_read(SPI1);
	 // Send data, read back result
   // **** TODO **** 
    spi_send(SPI1, data);
    result = spi_read(SPI1);

   // deselect mems
   write_pin(CS_SPI1,1);
    
   return (uint8_t) result;
}

uint8_t spi_read_command (uint8_t reg) {
   return spi_send_command (reg | (1<<7),  0);
}

void spi_write_command (uint8_t reg,  uint8_t data) {
   (void) spi_send_command (reg, data);
}

void mems_init () {

   init_spi1 ();

   // wait 10ms for memsboot process
   sysms = 10;
   while (sysms >0) {}

   spi_send_command(MEMS_CTRL_REG5, LIS3_CTRL5_INIT);
   spi_send_command(MEMS_CTRL_REG6, LIS3_CTRL6_INIT);
   spi_send_command(MEMS_FIFO_CTRL, LIS3_FIFO_CTRL_INIT);

   spi_send_command(MEMS_CTRL_REG4, LIS3_CTRL4_INIT);
   spi_send_command(MEMS_CTRL_REG3, LIS3_CTRL3_INIT);

}


void mems_wait_sample () {
   while (1) {
      if ((spi_read_command(MEMS_STATUS) & LIS3_STATUS_ZYXA) != 0) break;
   }  
}

void mems_read_sample () {

  x = (int8_t) spi_read_command(MEMS_OUT_XH);
  y = (int8_t) spi_read_command(MEMS_OUT_YH);
}

void mems_display_sample () {

  if (x > 20) {
     write_pin(RED_2E_LED,1);
     write_pin(GREEN_0W_LED,0);
  } else if (x < -20) {
     write_pin(RED_2E_LED,0);
     write_pin(GREEN_0W_LED,1);
  } else {
     write_pin(RED_2E_LED,0);
     write_pin(GREEN_0W_LED,0);
  }
   

  if (y > 20) {
     write_pin(ORANGE_1N_LED,1);
     write_pin(BLUE_3S_LED,0);
  } else if (y < -20) {
     write_pin(ORANGE_1N_LED,0);
     write_pin(BLUE_3S_LED,1);
  } else {
     write_pin(ORANGE_1N_LED,0);
     write_pin(BLUE_3S_LED,0);
  }
   
}

void exti0_isr () {
  exti_reset_request(EXTI0);
  mems_read_sample();
}


