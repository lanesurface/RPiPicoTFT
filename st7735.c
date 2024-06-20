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
static fmbf_fetch_t
__fetch_fmbf_data(uint x, uint y)
{
  fmbf_size_t loc_ptr=(y*TFT_WIDTH+x)*N_COLOR_BITS;
  uint m=loc_ptr%sizeof(*frame_buffer);
  
  return (fmbf_fetch_t){
    frame_buffer+(loc_ptr/sizeof(fmbf_size_t)),
    m
  };
}

static FMBF_TYPE
__make_mask(uint start, uint end)
{
  uint i;
  FMBF_TYPE r;

  r=0;
  for (i=start; i<=end; i++) {
    r+=1<<(end-i);
  }

  return r;
}

void
tft_put_px(uint x, uint y, tft_color_t clr)
{
  uint fmbf_sz_bts, adv, end, i, j;
  FMBF_TYPE msk, tmp;

  fmbf_fetch_t pos=__fetch_fmbf_data(x,y);
  fmbf_sz_bts=sizeof(FMBF_TYPE)*8;
  adv=pos.adv;
  i=0, j=N_COLOR_BITS;
  while (j>0) {
    end=MIN(fmbf_sz_bts,j);
    j=j-(end+1-adv);
    msk=__make_mask(adv,end);
    tmp=*(pos.ptr+i)&~msk;
    *(pos.ptr+i)=((clr.data>>j)&msk)|tmp;
    i++;
  }
}

/**
 * TODO: Fix this, LOL!
 */
tft_color_t 
tft_get_px(uint x, uint y)
{
  uint i, j, k, r;
  tft_color_t color;
  FMBF_TYPE buffer_data;

  fmbf_fetch_t px_pos=__fetch_fmbf_data(x, y);
  j=(sizeof(FMBF_TYPE)*8)-px_pos.adv, k=0; // j=8-3=5, k=0
  buffer_data=px_pos.ptr[k]; // XXXR RRRR RGGG GGGB BBBB BXXX

  for (fmbf_size_t i=0; i<N_COLOR_BITS; i++) { // i=0..17
    if (j<=0) {
      k++, j=1-(sizeof(FMBF_TYPE)*8);
      buffer_data=px_pos.ptr[k];
    } else {
      r=((*px_pos.ptr)>>(j-1))&1;
      color.data+=r<<(N_COLOR_BITS-i-1);
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