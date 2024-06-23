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
  adv=pos.adv;
  i=0, j=NUM_CLR_BITS;

  while (j>0) {
    end=MIN(fmbf_sz_bts,j);
    j=j-(end+1-adv);
    msk=__make_mask(adv,end);
    tmp=pos.ptr[i]&~msk;
    pos.ptr[i]=((clr.data>>(j+pos.adv))&msk)|tmp;
    i++;
  }
}

tft_color_t
tft_get_pixel(uint x, uint y)
{
  fmbf_pstn_t pos=__fetch_fmbf_data(x,y);
  return __fmbf_extrct(pos);
}

// /**
//  * TODO: Fix this, LOL!
//  */
// tft_color_t 
// tft_get_px(uint x, uint y)
// {
//   uint i, j, k, r;
//   tft_color_t color;
//   FMBF_TYPE buffer_data;

//   fmbf_fetch_t px_pos=__fetch_fmbf_data(x,y);
//   j=(sizeof(FMBF_TYPE)*8)-px_pos.adv, k=0; // j=8-3=5, k=0
//   buffer_data=px_pos.ptr[k]; // XXXR RRRR RGGG GGGB BBBB BXXX

//   for (fmbf_size_t i=0; i<NUM_CLR_BITS; i++) { // i=0..17
//     if (j<=0) {
//       k++, j=1-(sizeof(FMBF_TYPE)*8);
//       buffer_data=px_pos.ptr[k];
//     } else {
//       r=((*px_pos.ptr)>>(j-1))&1;
//       color.data+=r<<(NUM_CLR_BITS-i-1);
//       j--;
//     }
//   }

//   return color;
// }