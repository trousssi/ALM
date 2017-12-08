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


#ifndef LIS3DSH
#define LIS3DSH

typedef enum {

   MEMS_CTRL_REG3 		= 0x23,     // Ctrl interrupt generation
   MEMS_CTRL_REG4 		= 0x20,     // Ctrl frequency and axis enable
   MEMS_CTRL_REG5 		= 0x24,     // Aliasing filter + g sacle
   MEMS_CTRL_REG6 		= 0x25,     // Fifo and autoincrement register address

   MEMS_STATUS		= 0x27,     // Synchro bits

   MEMS_OUT_T		= 0x00,    // Temperture 25 + out_t   C

   MEMS_STAT			= 0x18,     // Interrupt status

   MEMS_OUT_XL               = 0x28,     // X data, LSB
   MEMS_OUT_XH               = 0x29,     // X data, MSB
   MEMS_OUT_YL               = 0x2A,     // Y data, LSB
   MEMS_OUT_YH               = 0x2B,     // Y data, MSB
   MEMS_OUT_ZL               = 0x2C,     // Z data, LSB
   MEMS_OUT_ZH               = 0x2D,     // Z data, MSB

   MEMS_FIFO_CTRL		= 0x2E	    // FIFO control (to disable FIFO)

} lis_reg_t;


/******************************* Ctrl reg 3 *************************************/

typedef enum {

   LIS3_CTRL3_DR_EN		= (1<<7),   // Data ready connection to INT1
   LIS3_CTRL3_IEA               = (1<<6),   // Interrupt polarity (default 0  active low)
   LIS3_CTRL3_IEL		= (1<<5),   // Interrupt latching (default 0 latch, 1 for pule)
   LIS3_CTRL3_INT2EN		= (1<<4),   // Enable int2 :  (default 0 : disabled)   boot + sm2 : not used
   LIS3_CTRL3_INT1EN		= (1<<3),   // Enable int1 :  (default 0 : disabled) 
   LIS3_CTRL3_VFILT		= (1<<2),   // vector filter (dfault 0 : disbaled)
   LIS3_CTRL3_STRT		= (1<<0)    // Soft reset (default 0 : inactive)

} lis_ctrl3_t;


// No soft reset, data ready interrupt  : INT1 
#define LIS3_CTRL3_INIT  (LIS3_CTRL3_DR_EN | LIS3_CTRL3_INT1EN | LIS3_CTRL3_IEA )

/******************************* Ctrl reg 4 *************************************/

typedef enum {
						// Output data rate
   LIS3_CTRL4_ODR_DOWN		= ( 0 << 4),
   LIS3_CTRL4_ODR_3HZ		= ( 1 << 4),
   LIS3_CTRL4_ODR_6HZ		= ( 2 << 4),
   LIS3_CTRL4_ODR_12HZ		= ( 3 << 4),
   LIS3_CTRL4_ODR_25HZ		= ( 4 << 4),
   LIS3_CTRL4_ODR_50HZ		= ( 5 << 4),
   LIS3_CTRL4_ODR_100HZ		= ( 6 << 4),
   LIS3_CTRL4_ODR_400HZ		= ( 7 << 4),
   LIS3_CTRL4_ODR_800HZ		= ( 8 << 4),
   LIS3_CTRL4_ODR_1600HZ	= ( 9 << 4),
   LIS3_CTRL4_ODR_MASK		= (15 << 4),

   LIS3_CTRL4_BDU		= ( 1 << 3),    // block data (SB and LSB synchonized)

   LIS3_CTRL4_ZEN		= ( 1 << 2),    // Z axis enable
   LIS3_CTRL4_YEN		= ( 1 << 1),    // Y axis enable
   LIS3_CTRL4_XEN		= ( 1 << 0)     // X axis enable

} lis_ctrl4_t;

// Data rate 12HZ, XYZ enabled
#define LIS3_CTRL4_INIT  (LIS3_CTRL4_ODR_12HZ | \
                          LIS3_CTRL4_ZEN |  LIS3_CTRL4_YEN | LIS3_CTRL4_XEN)

/******************************* Ctrl reg 5 *************************************/

typedef enum {
						// Antialiasing filter
   LIS3_CTRL5_BW_800HZ		= ( 0 << 6),
   LIS3_CTRL5_BW_200HZ		= ( 1 << 6),
   LIS3_CTRL5_BW_400HZ		= ( 2 << 6),
   LIS3_CTRL5_BW_50HZ		= ( 3 << 6),
   LIS3_CTRL5_BW_MASK		= ( 3 << 6),

						// G full scale
   LIS3_CTRL4_FSCALE_2G		= ( 0 << 3),
   LIS3_CTRL4_FSCALE_4G		= ( 1 << 3),
   LIS3_CTRL4_FSCALE_6G		= ( 2 << 3),
   LIS3_CTRL4_FSCALE_8G		= ( 3 << 3),
   LIS3_CTRL4_FSCALE_16G	= ( 4 << 3),
   LIS3_CTRL4_FSCALE_MASK	= ( 3 << 3),

						// self test
   LIS3_CTRL5_ST_NORMAL		= ( 0 << 2),    
   LIS3_CTRL5_ST_PLUS		= ( 1 << 2),   
   LIS3_CTRL5_ST_MINUS		= ( 2 << 2),   
   LIS3_CTRL5_ST_MASK		= ( 3 << 2),   

   LIS3_CTRL5_SIM   	  	= ( 1 << 0)     // SPI wire (default 0 : 4 wires, 1 : 3 wires)

} lis_ctrl5_t;

// Aliasing 200 Hz, 2G scale, no self test, 4 wires
#define LIS3_CTRL5_INIT  ( LIS3_CTRL5_BW_200HZ | LIS3_CTRL4_FSCALE_2G )

/******************************* Ctrl reg 6 *************************************/

typedef enum {
						
   LIS3_CTRL6_REBOOT  		= ( 1 << 7),	// Set by software, cleared by hardware on boot done
   LIS3_CTRL6_ADDINC  		= ( 1 << 4)     // auto increment reg address if set

} lis_ctrl6_t;

// Autoincrement to get X,y,Z in sequence
#define LIS3_CTRL6_INIT  ( LIS3_CTRL6_ADDINC )


/******************************* Status     *************************************/

typedef enum {
					    // axis data overrun
   LIS3_STATUS_ZYXOR		= (1<<7),   
   LIS3_STATUS_ZOR              = (1<<6),   
   LIS3_STATUS_YOR		= (1<<5),   
   LIS3_STATUS_XOR   		= (1<<4),   
					    // axis data valiable
   LIS3_STATUS_ZYXA		= (1<<3),   
   LIS3_STATUS_ZA            	= (1<<2),   
   LIS3_STATUS_YA		= (1<<1),   
   LIS3_STATUS_XA   		= (1<<0),   

} lis_status_t;


/******************************* Stat       *************************************/

typedef enum {
					    
   LIS3_STAT_DOR		= (1<<1),   // Data overrun (1 = overrun)
   LIS3_STAT_DRDY   		= (1<<0),   // Data ready (1 = ready)

} lis_stat_t;

/******************************* FIFO_CTRL  *************************************/

typedef enum {
					    
   LIS3_FIFO_CTRL_BYPASS	= ( 0<<4),   // Bypass FIFO
   LIS3_FIFO_CTRL_MASK  	= (15<<4),   
   LIS3_FIFO_CTRL_WTMP  	= (15<<0),   // watermark mask

} lis_fifo_ctrl_t;

#define LIS3_FIFO_CTRL_INIT  ( LIS3_FIFO_CTRL_BYPASS )

#endif
