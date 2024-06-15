#ifndef __TFT_GFX__
#define __TFT_GFX__

struct tft_font;
struct tft_bitmap;

typedef uint32_t tft_color_t;
static tft_color_t ** frame_buff;

extern const uint8_t *
tft_put_pixel(uint x, uint y, tft_color_t color);

extern const uint8_t *
tft_draw_line(uint xa, uint ya, uint xb, uint yb);

extern const uint8_t *
tft_draw_circle(uint x, uint y, uint r);

// extern const uint8_t *
// tft_draw_bitmap(tft_bitmap * bitmap);

// extern const uint8_t *
// tft_render_text(
//   tft_font * fnt, 
//   int x, 
//   int y, 
//   const char * txt);

#endif // TFT_GFX