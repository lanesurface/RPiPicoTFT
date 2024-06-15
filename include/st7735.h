#include "dspcfg.h"

/**
 * Chip-specific data structures, type definitions, constants, and function
 * implementations for the ST7735. Refer to the data sheet for more 
 * information about the operation of this chip.
 */
#ifndef __TFT_ST7735__
#define __TFT_ST7735__

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
  0x02, ra_set, 0x00, TFT_HEIGHT,
  ca_set, 0x00, TFT_WIDTH, 
};

#endif // __TFT_ST7735__