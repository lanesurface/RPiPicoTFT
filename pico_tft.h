
#ifndef __PICO_TFT__
#define __PICO_TFT__

const uint MHz=1000000; // MHz=10^6

extern const uint8_t *init_scr, *clr_scr; /*...*/
extern const uint8_t TFT_SW_RST;

/**
 * An OP code (command), as defined in the respective datasheet, together with
 * it's associated parameters, if any. A command taking no parameters should
 * set `params` to NULL or nullptr.
 */
typedef struct
{
  uint8_t tft_cmd, *params;
} tft_cmd_t;

/**
 * send_cmd:
 * drive CHX lo;
 * D/CX hi for OP code, lo for PA[1], ..., PA[N-1], PA[N].
 */
extern void
send_cmd(spi_inst_t * spi, tft_cmd_t * cmd);

#endif // __PICO_TFT__