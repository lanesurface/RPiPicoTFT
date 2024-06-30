/**
 * TFTGFX:
 * Author(s): Surface, Lane W
 * Created: 06/20/2024
 * 
 * Graphics functions which are agnostic of the driver chip being used. The 
 * implementation of most graphics functions deal with the intermediate frame
 * buffer on the controller, regardless of destination pixel format, etc.
 */
#include "tftgfx.h"

uint8_t clr_fd_wth[]={ 
  RED_FDWTH, 
  GRN_FDWTH, 
  BLU_FDWTH
};

/**
 * __fetch_fmbf_data:
 * Convert (x,y) coordinate pair into a location in the frame buffer (of that
 * respective pixel), and return a pointer to the start of this pixel.
 */
static fmbf_pstn_t
__fetch_fmbf_data(uint x, uint y)
{
  fmbf_size_t loc_ptr=(y*TFT_WIDTH+x)*NUM_CLR_BITS;
  uint m=loc_ptr%sizeof(*frame_buffer);
  
  return (fmbf_pstn_t){
    frame_buffer+(loc_ptr/sizeof(fmbf_size_t)),
    m
  };
}

void
tft_set_pixel(uint x, uint y, tft_color_t clr)
{
  uint fmbf_sz_bts, adv, end, i, j;
  FMBF_TYPE msk, tmp;

  fmbf_pstn_t pos=__fetch_fmbf_data(x,y);
  fmbf_sz_bts=sizeof(FMBF_TYPE)*8;
  adv=pos.advance;
  i=0, j=NUM_CLR_BITS;

  while (j>0) {
    end=MIN(fmbf_sz_bts,j);
    j=j-(end+1-adv);
    msk=fmbf_make_msk(adv,end);
    tmp=pos.pos_ptr[i]&~msk;
    pos.pos_ptr[i]=((clr.data>>(j+pos.advance))&msk)|tmp;
    i++;
  }
}

tft_color_t
tft_get_pixel(uint x, uint y)
{
  return fmbf_extract_clr(__fetch_fmbf_data(
    x,
    y
  ));
}