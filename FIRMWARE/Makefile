##
## This file is part of the libopencm3 project.
##
## Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
##
## This library is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with this library.  If not, see <http://www.gnu.org/licenses/>.
##

DEVICE=STM32F407VG
##DEVICE=STM32F446RET6

OPENCM3_DIR     = ../libopencm3
//OBJS            += firmware.o cold_startup.o usb_configuration.o
OBJS            += cold_startup.o firmware_f407.o usb_configuration.o rs232.o

CC = arm-none-eabi-gcc 

CFLAGS          += -O0 -ggdb3 -Wall
CPPFLAGS	+= -MD
LDFLAGS         += -static -nostartfiles
LDLIBS          += -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

BINARY = firmware_f407

all: firmware_f407.elf firmware_f407.hex

include $(OPENCM3_DIR)/mk/genlink-config.mk
include $(OPENCM3_DIR)/mk/gcc-config.mk

.PHONY: clean all


clean:
	$(Q)$(RM) -rf ${BINARY}.elf ${BINARY}.bin ${BINARY}.hex ${BINARY}.d *.o

include $(OPENCM3_DIR)/mk/genlink-rules.mk
include $(OPENCM3_DIR)/mk/gcc-rules.mk

# location of OpenOCD Board .cfg files (only used with 'make program')
OPENOCD_BOARD_DIR=/usr/share/openocd/scripts/board/
#OPENOCD_BOARD_DIR=FLASH_DEBUG/

# Configuration (cfg) file containing programming directives for OpenOCD
OPENOCD_PROC_FILE=FLASH_DEBUG/stm32f4-openocd.cfg

#OPENOCD_PROC_FILE=st_nucleo_f4.cfg

#######################################################################""


cold_startup.o:	shield_discoveryf4_wiring.h cold_startup.h

firmware_f407.o:	cold_startup.h  shield_discoveryf4_wiring.h

usb_configuration.o:	usb_configuration.h identity.h shield_discoveryf4_wiring.h 

rs232.o:	rs232.h

program: $(BINARY).bin
	openocd -f FLASH_DEBUG/stm32f4discovery.cfg -f $(OPENOCD_PROC_FILE) -c "stm_flash `pwd`/${BINARY}.bin" -c shutdown 

program_disc1: $(BINARY).bin
	openocd -f FLASH_DEBUG/stm32f4discovery_disc1.cfg -f $(OPENOCD_PROC_FILE) -c "stm_flash `pwd`/${BINARY}.bin" -c shutdown 


debug:	program
	$(GDB) -x FLASH_DEBUG/discoveryf4_gdb_cmds $(BINARY).elf

debug_disc1:	program_disc1
	$(GDB) -x FLASH_DEBUG/discoveryf4_disc1_gdb_cmds $(BINARY).elf



#	openocd -f FLASH_DEBUG/nucleo.cfg -f FLASH_DEBUG/stm32f4-openocd.cfg   -c "stm_flash ./shield_f407_firmware.bin" -c shutdown
#	$(GDB) -x FLASH_DEBUG/nucleo_gdb_cmds $(BINARY).elf
