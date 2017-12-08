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
OBJS            += cold_startup.o firmware.o spi_mems.o

CC = arm-none-eabi-gcc 

CFLAGS          += -O0 -ggdb3 -Wall
CPPFLAGS	+= -MD
LDFLAGS         += -static -nostartfiles
LDLIBS          += -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

BINARY = firmware

all: firmware.elf firmware.hex

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

#######################################################################

spi_mems.o:	lis3dsh.h

cold_startup.o:	discoveryf4_wiring.h cold_startup.h

firmware.o:	cold_startup.h  discoveryf4_wiring.h spi_mems.h

program: $(BINARY).bin
	openocd -f FLASH_DEBUG/stm32f4discovery.cfg -f $(OPENOCD_PROC_FILE) -c "stm_flash `pwd`/${BINARY}.bin" -c shutdown 

debug:	program
	$(GDB) -x FLASH_DEBUG/discoveryf4_gdb_cmds $(BINARY).elf

ddd_debug: program
	ddd --debugger "$(GDB) -x FLASH_DEBUG/discoveryf4_gdb_cmds $(BINARY).elf"
