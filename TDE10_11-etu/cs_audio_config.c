#include "cs_audio_config.h"
#include "i2c_audio.h"

#include <stdio.h>

cs_command_t cs_digital_init_seq [] = {
        // Standard init sequence, CS43l22 documentation 4.1 page 32
        {0x00,0x00},
        {0x47,0x80},
        {0x32,(1<<7)},
        {0x32,0x00},
        {0x00,0x00},

        // Init Digital
        {CODEC_MAP_PWR_CTRL2,0xAF},
        {CODEC_MAP_PLAYBACK_CTRL1,0x70},
        {CODEC_MAP_CLK_CTRL,0x81},
        {CODEC_MAP_IF_CTRL1,0x07},
        {0x0A,0x00},
        {0x27,0x0},
        {0x1A,0x0A},
        {0x1B,0x0A},
        {0x1F,0x0F},
        {CODEC_MAP_PWR_CTRL1,0x9E},
        {CS_NOREGISTER,0}
};

void cs_config (cs_command_t *seq) {
   while ((seq != NULL) && (seq->regnum != CS_NOREGISTER)) {
     send_cs(seq->regnum,seq->regval);
     seq++;
   }
}
