#include <string.h> // For memcpy
#include "pico_tft.h"
#include "st7735.h"

const uint8_t 
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

const uint8_t st_delay=0x80;

const uint8_t init_scr[]={
  /*0x0f*/
  0x03, sw_rst, st_delay, 0x96,
  slp_out, st_delay, 0xff, col_mod, 
  NUM_CLR_BITS,
};

const uint8_t clear_scr[]={
  0x02, ra_set, 0x00, TFT_HEIGHT,
  ca_set, 0x00, TFT_WIDTH, 
};

static uint
__cnk_load_next(uint8_t *cnk, fmbf_size_t cnk_sz)
{
  static fmbf_size_t i=0, j=FMBF_BDY;

  if (j<cnk_sz) {
    memcpy(cnk, frame_buffer+i, j);
    return CNK_END_DATA_FRAME;
  } else {
    memcpy(cnk, frame_buffer+i, cnk_sz);
    i+=cnk_sz;
    j=(j-i);
  }
  return 0;
}

/**
 * Get chunk of data from [fmbf_ptr..fmbf_ptr+(cnk_sz-1)], with each color
 * component aligned on the boundary of a byte (ie, each pixel is three bytes 
 * in the TX buffer).
 */
static uint
__cnk_load_next_aligned(uint8_t *cnk, fmbf_size_t cnk_sz)
{
  static fmbf_size_t j=NUM_CLR_COMPS-1;
  static fmbf_pstn_t pos=(fmbf_pstn_t){
    0,
    0
  };

  uint fmbf_sz_bts, cnk_sz_bts, fd_wth, i;
  tft_color_t clr;
  FMBF_TYPE msk;

  fmbf_sz_bts=sizeof(FMBF_TYPE)*8;
  cnk_sz_bts=sizeof(uint8_t)*8;
  i=0;

  while (i<cnk_sz) { // >>WIP<<
    clr=__fmbf_extrct(pos);
    fd_wth=clr_fd_wth[NUM_CLR_COMPS-j];
    msk=__make_mask(0,fd_wth);
    cnk[i]=clr.data>>(j*fd_wth)&msk;
    // pos=__fmbf_next(); // Error: const pointer, cannot assign after initialization.
  }
  return 0;
}

uint
tft_load_cnk(uint8_t *cnk, fmbf_size_t cnk_sz)
{
  switch (NUM_CLR_BITS) {
  case 18:
    return __cnk_load_next_aligned(cnk, cnk_sz);
  default: // 12 and 16 bit color can be sent as is.
    return __cnk_load_next(cnk, cnk_sz);
  }
}