#include "dspcfg.h"

#ifndef __TFTGFX__
#define __TFTGFX__

#define FMBF_BDY (TFT_WIDTH*NUM_CLR_BITS*TFT_HEIGHT) // Max value for fmbf_ptr
#define FMBF_TYPE uint8_t
#define FMBF_SZ  (FMBF_BDY/(sizeof(FMBF_TYPE)*8)) // Size of frame buffer in bytes

#ifndef MIN
# define MIN(x,y) x>y ? y : x;
#endif

/**
 * Various flags for the status register.
 */
#define CNK_END_DATA_FRAME 0x40

typedef struct
{
  union {
    //FMBF_TYPE fmbf_comp[FMBF_NUM_COMP];
    uint32_t 
      red   : RED_FDWTH, 
      green : GRN_FDWTH, 
      blue  : BLU_FDWTH;
    uint32_t data : NUM_CLR_BITS;
  };
} tft_color_t;

/**
 * Width (in bits) of each color component. NOte: these elements must appear in
 * the order that the display expects to read them (eg, an RGB display mode
 * has RD_FDWTH, ..., BLU_FDWTH).
 */
extern uint8_t clr_fd_wth[NUM_CLR_COMPS];

typedef struct {
  /**
   * Pointer to the first byte associated with (x,y).
   */
  FMBF_TYPE * const ptr;
  /**
   * Advance N bits into the first byte fetched.
   */
  uint8_t adv;
} fmbf_pstn_t;

/**
 * As it is, the frame buffer will already occupy a significant amount of 
 * system RAM (eg, 128x160 at 18 bits per color is 46kB, or about 18% of the 
 * total memory available). Therefore, pixel data needs to be stored in a 
 * compressed format. For this purpose, the size of the frame buffer can be 
 * reduced simply by having color data follow each other in memory
 * bit-by-bit. 
 */
typedef uint32_t fmbf_size_t;
static FMBF_TYPE frame_buffer[FMBF_SZ];
static fmbf_size_t fmbf_ptr=0U;

/**
 * Access to the frame buffer pointer should always be controlled through this
 * function.
 */
static fmbf_pstn_t
__fmbf_next()
{
  // if (fmbf_ptr+1>FMBF_BDY) {
  //   fmbf_ptr=0;
  //   status|=CNK_END_DATA_TX;
  //   return (fmbf_pstn_t){0xE0,0xE0};
  // } else {

  //   fmbf_ptr++;
  // }
  
  static uint i=0, j=0;
  uint fmbf_sz_bts, k, l, m;

  fmbf_sz_bts=sizeof(FMBF_TYPE)*8;
  k=(j+NUM_CLR_BITS);
  i+=k/fmbf_sz_bts, j=k%fmbf_sz_bts;
  return (fmbf_pstn_t){ 
    frame_buffer+i,
    j
  };
}

static FMBF_TYPE
__make_mask(uint start, uint end)
{
  uint i;
  FMBF_TYPE r;

  r=0;
  for (i=start; i<end; i++) {
    r+=1<<(end-i);
  }

  return (r+1);
}

static tft_color_t
__fmbf_extrct(fmbf_pstn_t pos)
{
  uint fmbf_sz_bts, adv, end, i, j;
  uint32_t data;
  uint8_t msk;
  tft_color_t clr;

  fmbf_sz_bts=sizeof(FMBF_TYPE)*8;
  adv=pos.adv;
  i=0, j=NUM_CLR_BITS;

  while (j>0) {
    end=MIN(fmbf_sz_bts,j);
    j=j-(end+1-adv);
    msk=__make_mask(adv,end);
    clr.data+=(*pos.ptr&msk)<<(j+pos.adv);
    i++;
  }

  return clr;
}

/**
 * Get the frame (in the appropriate format for the parameter of RAMRW) 
 * produced by the calling the functions in this file.
 * 
 * NOTE: The display RAM (DRAM) on the driver chip is non-volatile memory;
 * therefore, a frame should only be written when a call to a frame-modifying 
 * function preceeds it. Fetching the frame and writing it to the chip 
 * needlessly is sure to reduce the performance of the program.
 */
extern uint
tft_load_cnk(uint8_t *cnk, fmbf_size_t cnk_sz);

extern void
tft_put_pixel(uint x, uint y, tft_color_t color);

extern tft_color_t
tft_get_pixel(uint x, uint y);

extern void
tft_draw_line(uint xa, uint ya, uint xb, uint yb);

extern void
tft_draw_circle(uint x, uint y, uint r);

// extern const uint8_t *
// tft_draw_bitmap(tft_bitmap * bitmap);

// extern const uint8_t *
// tft_render_text(
//   tft_font * fnt, 
//   int x, 
//   int y, 
//   const char * txt);

#endif // __TFTGFX__