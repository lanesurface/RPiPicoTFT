#ifndef __TFT_GFX__
#define __TFT_GFX__

extern const uint8_t *
tft_draw_line(int xa, int ya, int xb, int yb);

extern const uint8_t *
tft_draw_circle(int x0, int y0, int r);

// extern const uint8_t *
// tft_draw_bitmap(tft_bitmap * bitmap);

// extern const uint8_t *
// tft_render_text(
//   tft_font * fnt, 
//   int x, 
//   int y, 
//   const char * txt
// );

#endif // TFT_GFX