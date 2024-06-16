#include "hardware/spi.h"
#include "dspcfg.h"
#ifdef ST7735R
#include "st7735.h"
#else
#include "dvrdmy.h"
#endif

#ifndef __PICO_TFT__
#define __PICO_TFT__

const uint MHz=1000000; // MHz=10^6
extern const uint8_t init_scr[], clear_scr[];

/**
 * Initialize the TFT driver and associated structures.
 */
extern void
tft_init_ctx();

/**
 * send_cmd:
 * drive CHX lo;
 * D/CX hi for OP code, lo for PA[1], ..., PA[N-1], PA[N].
 */

extern void
tft_send_pgm(spi_inst_t * spi, const uint8_t * prog);

#endif // __PICO_TFT__