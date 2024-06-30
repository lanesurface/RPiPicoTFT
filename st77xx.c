/**
 * Author(s): Surface, Lane W
 * Date: 06/28/2024
 * 
 * Most of the interfacing with ST77XX chips is the same; what differs in most
 * cases is the size of the display supported (which is fixed, due to the
 * VRAM available on the chip). 
 */
#include <string.h> // For memcpy
#include "st77xx.h"

#define LINE_SZ (NUM_CLR_COMPS*TFT_WIDTH)

uint8_t *
tft_init_screen()
{
  return (uint8_t[]){
    0x03, // << N instructions
    SWRST, 
    ST_DELAY, 
    0x96,
    SLPOUT, 
    ST_DELAY, 
    0xff,
    COLMOD, 
    0x06 // << Change
  };
}

uint8_t *
tft_clear_screen()
{
  return (uint8_t[]){
    0x02, // << N instructions
    RASET,
    0x00,
    TFT_HEIGHT,
    CASET,
    0x00,
    TFT_WIDTH
  };
}

static cnk_data_t
__cnk_load_next(uint8_t * cnk, fmbf_size_t cnk_sz)
{
  static fmbf_size_t i=0, j=FMBF_BDY;

  if (j<cnk_sz) {
    memcpy(cnk, frame_buffer+i, j);
    i=0, j=FMBF_BDY;
    return (cnk_data_t){
      END_DATA_FRAME,
      j
    };
  } else {
    memcpy(cnk, frame_buffer+i, cnk_sz);
    i+=cnk_sz;
    j=(j-i);
  }
  return (cnk_data_t){
    0,
    cnk_sz
  };
}

// RRRR RRGG GGGG BBBB BBXX
// XXRR RRRR GGGG GGBB BBBB
// XRRR RRRG GGGG GBBB BBBX
// RRRR GGGG BBBB XXXX
// XRRR RGGG GBBB BXXX

/**
 * Get chunk of data from [fmbf_ptr..fmbf_ptr+(cnk_sz-1)], with each color
 * component aligned on the boundary of a byte (ie, each pixel is three bytes
 * in the TX buffer).
 */
static cnk_data_t
__cnk_load_next_aligned(uint8_t * cnk, fmbf_size_t cnk_sz)
{
  static fmbf_pstn_t pos=(fmbf_pstn_t){
    0,
    0
  };

  fmbf_size_t i, j, k, l, m;
  tft_color_t clr;
  uint flag;

  j=0, l=cnk_sz;
  while (l>0) {
    if (flag) {
      return (cnk_data_t){
        END_DATA_FRAME,
        cnk_sz-l
      };
    } else {
      clr=fmbf_extract_clr(pos);
      for (i=0; i<NUM_CLR_COMPS; i++) {
        k=NUM_CLR_BITS-clr_fd_wth[i]; 
        cnk[j++]=clr.data<<k;
        l--;
      }
      pos=fmbf_next_pixel(&flag);
    }
  }
  return (cnk_data_t){
    0,
    cnk_sz
  };
}

cnk_data_t
tft_load_next_cnk(uint8_t * cnk, fmbf_size_t cnk_sz)
{
  switch (NUM_CLR_BITS) {
  case 18:
    return __cnk_load_next_aligned(cnk,cnk_sz);
  default: // 12 and 16 bit color can be written as is.
    return __cnk_load_next(cnk,cnk_sz);
  }
}

/**
 * Loads the next line (row in the frame buffer) into the provided buffer, and
 * returns the state of this transfer operation (eg, END_DATA_FRAME if the 
 * entire contents of the frame buffer have been provided; and indicating that
 * the VSYNC follows).
 */
cnk_data_t
tft_load_next_line(uint8_t * ln)
{
  return tft_load_next_cnk(
    ln,
    LINE_SZ
  );
}
