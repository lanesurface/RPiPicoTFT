#include "dspcfg.h"

#ifndef __TFTGFX__
#define __TFTGFX__

#define FMBF_TYPE uint8_t
#define FMBF_BDY (TFT_WIDTH*NUM_CLR_BITS*TFT_HEIGHT) // Max value for fmbf_ptr
#define FMBF_SZ_BTS (sizeof(FMBF_TYPE)*8)
#define FMBF_SZ  (FMBF_BDY/(sizeof(FMBF_TYPE)*8)) // Size of frame buffer in bytes

#ifndef MIN
# define MIN(x,y) x>y ? y : x;
#endif

/**
 * Various flags for the status register.
 */
#define LINE_READY     0x00
#define END_DATA_FRAME 0x40

typedef struct
{
  union {
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
  FMBF_TYPE * pos_ptr;
  /**
   * Advance N bits into the first byte fetched.
   */
  uint8_t advance;
} fmbf_pstn_t;

 /**
  * It is not expected that the frame buffer will ever occupy more than 4 
  * gigabytes in any case; however, it will most certainly be larger than 16
  * kB, so a 32 bit integer should always be large enough to store a position 
  * within this buffer.
  */
typedef uint32_t fmbf_size_t;

static FMBF_TYPE frame_buffer[FMBF_SZ];
static fmbf_size_t fmbf_ptr=0U;

typedef struct cnk_data {
  uint flag;
  fmbf_size_t bytes_read;
} cnk_data_t;

/**
 * fmbf_next_pixel:
 * This function, upon the first call, stores the byte and bit offsets into
 * frame buffer; and, for each subsequent call, will return the position of the
 * next pixel in the frame.
 */
static fmbf_pstn_t
fmbf_next_pixel(uint * flag)
{
  static fmbf_size_t i=0, j=0;
  fmbf_size_t k, l, m;

  k=(j+NUM_CLR_BITS);
  i+=k/FMBF_SZ_BTS, j=k%FMBF_SZ_BTS;

  if (i>FMBF_BDY) {
    k=FMBF_BDY-i;
    i=k, j=k%FMBF_SZ_BTS;
    (*flag)|=END_DATA_FRAME;
  }
  return (fmbf_pstn_t){
    frame_buffer+i,
    j
  };
}

static FMBF_TYPE
fmbf_make_msk(uint8_t start, uint8_t end)
{
  fmbf_size_t i;
  FMBF_TYPE r;

  r=0;
  for (i=start; i<end; i++) {
    r+=1<<(end-i);
  }

  return (r+1);
}

static tft_color_t
fmbf_extract_clr(fmbf_pstn_t pos)
{
  uint adv, end;
  fmbf_size_t i, j;
  FMBF_TYPE msk;
  tft_color_t clr;

  adv=pos.advance;
  i=0, j=NUM_CLR_BITS;

  while (j>0) {
    end=MIN(FMBF_SZ_BTS,j);
    j=j-(end+1-adv);
    msk=fmbf_make_msk(adv,end);
    clr.data+=(*pos.pos_ptr&msk)<<(j+pos.advance);
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
extern cnk_data_t
tft_load_next_cnk(uint8_t * cnk, fmbf_size_t cnk_sz);

/**
 * Because some driver chips may require HSYNC and VSYNC, this necessitates
 * the ability of the frame to be loaded line-by-line.
 * 
 * The return value is the flag indicating whether a VSYNC should follow this 
 * function call.
 */
extern cnk_data_t
tft_load_next_line(uint8_t * ln);

extern uint8_t *
tft_init_screen();

extern uint8_t *
tft_clr_screen();

extern void
tft_put_pixel(uint x, uint y, tft_color_t color);

extern tft_color_t
tft_get_pixel(uint x, uint y);

extern void
tft_draw_line(uint xa, uint ya, uint xb, uint yb);

extern void
tft_draw_circle(uint x, uint y, uint r);

// tftbmp.h
// extern void
// tft_draw_bitmap(tft_bitmap * bitmap);

// tftfnt.h
// extern void
// tft_render_text(
//   tft_font * fnt, 
//   int x, 
//   int y, 
//   const char * txt);

#endif // __TFTGFX__
