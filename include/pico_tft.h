#include "displaycfg.h"
#ifdef ST_7735
#include "st_7735.h"
#else
#include "dvrdmy.h"
#endif

#ifndef __PICO_TFT__
#define __PICO_TFT__

const uint MHz=1000000; // MHz=10^6
extern const uint8_t init_scr[], clear_scr[];

typedef uint32_t tft_color_t;
static tft_color_t ** frame_buff;

/**
 * Initialize the TFT driver and associated structures.
 */
extern void
tft_init_ctx();

extern const uint8_t *
tft_put_pixel(int x, int y, uint32_t color);

/**
 * send_cmd:
 * drive CHX lo;
 * D/CX hi for OP code, lo for PA[1], ..., PA[N-1], PA[N].
 */

extern void
tft_send_pgm(spi_inst_t * spi, const uint8_t * prog);

#endif // __PICO_TFT__