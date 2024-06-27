#include <string.h> // For memcpy
#include "pico_tft.h"
#include "st7735.h"

const uint8_t st_delay=0x80;

const uint8_t init_scr[]={
  /*0x0f*/
  0x03, // << N instructions
  SWRST, st_delay, 0x96,
  SLPOUT, st_delay, 0xff, 
  COLMOD, 0x06 // << Change
};

const uint8_t clear_scr[]={
  0x02, // << N instructions
  RASET, 0x00, TFT_HEIGHT,
  CASET, 0x00, TFT_WIDTH, 
};

static uint
__cnk_load_next(uint8_t * cnk, fmbf_size_t cnk_sz)
{
  static fmbf_size_t i=0, j=FMBF_BDY;

  if (j<cnk_sz) {
    memcpy(cnk, frame_buffer+i, j);
    i=0, j=FMBF_BDY;
    return END_DATA_FRAME;
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
__cnk_load_next_aligned(uint8_t * cnk, fmbf_size_t cnk_sz)
{
  static fmbf_size_t j=NUM_CLR_COMPS;
  static fmbf_pstn_t pos=(fmbf_pstn_t){
    0,
    0
  };

  uint fmbf_sz_bts, fd_wth, fd_acm, flag, i;
  tft_color_t clr;

  fmbf_sz_bts=sizeof(FMBF_TYPE)*8;
  i=0, flag=0, fd_acm=NUM_CLR_BITS;

  while (i<cnk_sz) {
    if (flag) {
      return END_DATA_FRAME;
    }
    clr=__fmbf_extrct_clr(pos); // pos=(0,0)
    fd_wth=clr_fd_wth[NUM_CLR_COMPS-j]; // R:6
    fd_acm=(fd_acm-fd_wth)+pos.advance; // 12
    cnk[i++]=(clr.data>>fd_acm)&__make_mask(
      0, 
      fd_wth
    );
    pos=__fmbf_next(&flag), j=(j-1);
  }

  return 0;
}

uint
cnk_load_next(uint8_t * cnk, fmbf_size_t cnk_sz)
{
  switch (NUM_CLR_BITS) {
  case 18:
    return __cnk_load_next_aligned(cnk,cnk_sz);
  default: // 12 and 16 bit color can be written as is.
    return __cnk_load_next(cnk,cnk_sz);
  }
}

/**
 * extern uint
 * tft_load_next_line(uint8_t * ln, uint dpl);
 * 
 * uint dpl, flag;
 * const char line_buffer[NUM_CLR_COMPS*TFT_WIDTH];
 * 
 * flag=tf_load_next_line(
 *   line_buffer, 
 *   3*TFT_WIDTH
 * );
 * if (flag) {
 *   VSYNC;
 * } else HSYNC;
 */

#define LINE_LENGTH (TFT_WIDTH*NUM_CLR_COMPS)
#define LINE_SZ (NUM_CLR_COMPS*TFT_WIDTH)

static uint
__ld_line(uint8_t * ln, uint dpl)
{
  static fmbf_pstn_t pos=(fmbf_pstn_t){
    0,
    0
  };
  uint num_bytes, flag, fmbf_sz_bts, i, j, k;
  FMBF_TYPE msk;

  j=(NUM_CLR_BITS*TFT_WIDTH);
  fmbf_sz_bts=sizeof(uint8_t)*8;
  num_bytes=j/fmbf_sz_bts, k=j%fmbf_sz_bts;
  if (k) {
    num_bytes+=1;
  }

  for (i=0; i<num_bytes; i++) {
    pos=__fmbf_next(&flag);
    // **FIXME**
    if (j<fmbf_sz_bts) {
      ln[i]=frame_buffer[i]&__make_mask(
        0,
        j
      );
    } else {
      ln[i]=frame_buffer[i];
      j=(j-fmbf_sz_bts);
    }
  }
  return 0;
}

static uint
__ld_line_aligned(uint8_t * ln, uint dpl)
{
  /* TODO */
}

/**
 * Loads the next line (row in the frame buffer) into the provided buffer, and
 * returns the state of this transfer operation (eg, END_DATA_FRAME if the 
 * entire contents of the frame buffer have been provided; and indicating that
 * the VSYNC follows).
 */
uint
tft_load_next_line(uint8_t * ln, uint dpl)
{
  switch (NUM_CLR_BITS) {
  case 18:
    return __ld_line_aligned(ln,dpl);
  default: // 12 and 16 bit color can be sent as is.
    return __ld_line(ln,dpl);
  }
}