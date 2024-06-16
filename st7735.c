#include "pico_tft.h"
#include "st7735.h"
#include "tftgfx.h"
#include "dspcfg.h"

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
 * pixel in the frame buffer.
 */
// XXXR RRRR RGGG GGGB BBBB BXXX
// ^..1,     ^..2,     ^..3
// XRRR RRRG GGGG GBBB BBBX
// XXRR RRRR GGGG GGBB BBBB
// XXXR RRRR RGGG GGGB BBBB BXXX
// RRRR RRGG GGGG BBBB BBXX
// ^..1,     ^..2,     ^..3

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