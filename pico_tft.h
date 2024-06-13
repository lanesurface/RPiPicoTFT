
#ifndef __PICO_TFT__
#define __PICO_TFT__

const uint MHz=1000000; // MHz=10^6

#define TFT_MISO 6
#define TFT_MOSI 3
#define TFT_CHX  5
#define TFT_SCK  2

/**
 * Procedures common to all TFT displays, but which have (potentially)
 * different implementions.
 */
// extern const uint8_t *init_scr, *clr_scr; /*...*/
extern const uint8_t 
  nop=0x00, 
  sw_rst=0x01, 
  slp_in=0x10, 
  slp_out=0x11,
  plt_on=0x12,
  nor_on=0x13,
  inv_off=0x20,
  inv_on=0x21,
  gam_set=0x26,
  disp_off=0x28,
  disp_on=0x29,
  ca_set=0x2a,
  ra_set=0x2b,
  ram_rw=0x2c,
  ram_rd=0x2e,
  ptlar=0x30,
  te_off=0x34,
  te_on=0x35,
  mad_ctl=0x36,
  idm_off=0x38,
  ide_on=0x39,
  col_mod=0x3a;

// col_mod, 0x06 (18 bit/pixel)

// 1000 0000
// ^..hi bit is set when a delay follows the instruction
const uint8_t st_delay=0x80;

const uint8_t init_scr[]={
  /*0x0f*/
  0x02, sw_rst, st_delay, 0x96,
  slp_out, st_delay, 0xff, 
};

const uint8_t clear_scr[]={
  0x02, ra_set, 0x00, 0x84, // ra_set <0,180>
  ca_set, 0x00, 0x80, // ca_set <0,128>
};

const uint8_t * draw_line(int xa, int ya, int xb, int yb);

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
send_pgm(spi_inst_t * spi, const uint8_t * prog);

#endif // __PICO_TFT__