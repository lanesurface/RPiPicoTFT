#include "dspcfg.h"

#ifndef __TFTGFX__
#define __TFTGFX__

#define FMBF_BDY (TFT_WIDTH*N_COLOR_BITS*TFT_HEIGHT) // Max value for fmbf_ptr
#define FMBF_TYPE uint8_t
#define FMBF_SZ  (FMBF_BDY/(sizeof(FMBF_TYPE)*8)) // Size of frame buffer in bytes

typedef struct
{
  union {
    //FMBF_TYPE fmbf_comp[FMBF_NUM_COMP];
    uint32_t 
      red   : RED_FDWTH, 
      green : GRN_FDWTH, 
      blue  : BLU_FDWTH;
    uint32_t data : N_COLOR_BITS;
  };
} tft_color_t;

typedef struct {
  /**
   * Pointer to the first byte associated with (x,y).
   */
  FMBF_TYPE * const ptr;
  /**
   * Advance N bits into the first byte fetched.
   */
  uint8_t adv;
} fmbf_fetch_t;

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
 * Get the frame (in the appropriate format for the parameter of RAMRW) 
 * produced by the calling the functions in this file.
 * 
 * NOTE: The display RAM (DRAM) on the driver chip is non-volatile memory;
 * therefore, a frame should only be written when a call to a frame-modifying 
 * function preceeds it. Fetching the frame and writing it to the chip 
 * needlessly is sure to reduce the performance of the program.
 */
extern const uint8_t *
tft_get_frame();

extern void
tft_put_px(uint x, uint y, tft_color_t color);

extern tft_color_t
tft_get_px(uint x, uint y);

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