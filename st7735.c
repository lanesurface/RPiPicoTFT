#include "pico_tft.h"
#include "st7735.h"
#include "tftgfx.h"
#include "dspcfg.h"

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

// col_mod, 0x06 (18 bit/pixel)

// 1000 0000
// ^..hi bit is set when a delay follows the instruction
const uint8_t st_delay=0x80;

const uint8_t init_scr[]={
  /*0x0f*/
  0x03, sw_rst, st_delay, 0x96,
  slp_out, st_delay, 0xff, col_mod, 
  N_COLOR_BITS,
};

const uint8_t clear_scr[]={
  0x02, ra_set, 0x00, TFT_HEIGHT,
  ca_set, 0x00, TFT_WIDTH, 
};

/**
 * __fetch_fmbf_data:
 * Convert (x,y) coordinate pair into a location in the frame buffer (of that
 * respective pixel), and return a pointer to the start of this pixel.
 */
fmbf_fetch_t
__fetch_fmbf_data(uint x, uint y)
{
  fmbf_size_t loc_ptr=(y*TFT_WIDTH+x)*N_COLOR_BITS;
  uint m=loc_ptr%sizeof(*frame_buffer);
  
  return (fmbf_fetch_t){
    frame_buffer+(loc_ptr/sizeof(fmbf_size_t)),
    m
  };
}

void
tft_put_px(uint x, uint y, tft_color_t color)
{
  
}
/**
 * A maximum of three bytes must be read, regardless of the boundary of the
 * pixel in the frame buffer, eg:
 *
 * XXXR RRRR RGGG GGGB BBBB BXXX
 * ^..1,     ^..2,     ^..3
 *
 * RRRR RRGG GGGG BBBB BBXX
 * ^..1,     ^..2,     ^..3
 */
tft_color_t 
tft_get_px(uint x, uint y)
{
  uint i, j, k, r;
  tft_color_t color;
  FMBF_TYPE buffer_data;

  fmbf_fetch_t px=__fetch_fmbf_data(x, y);
  j=(sizeof(FMBF_TYPE)*8)-px.adv, k=0; // j=8-3=5, k=0
  buffer_data=px.ptr[k]; // XXXR RRRR RGGG GGGB BBBB BXXX

  for (fmbf_size_t i=0; i<N_COLOR_BITS; i++) { // i=0..17
    if (j<=0) {
      k++, j=1-(sizeof(FMBF_TYPE)*8);
      buffer_data=px.ptr[k];
    } else {
      r=((*px.ptr)>>(j-1))&1;
      color.bit_data+=r<<(N_COLOR_BITS-i-1);
      j--;
    }
  }

  return color;
}

const uint8_t * 
tft_get_frame()
{
  return (const uint8_t *)frame_buffer;
}