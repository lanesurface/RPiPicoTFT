
#ifndef __DISPLAY_CFG__
#define __DISPLAYCFG__

#define N_COLOR_BITS 18

#define TFT_WIDTH  0x80
#define TFT_HEIGHT 0x84

#define TFT_MISO 6
#define TFT_MOSI 3
#define TFT_CHX  5
#define TFT_SCK  2

/* Define the appropriate driver chip for your configuration:
 * ST_7735, etc 
 */
#define ST_7735 

#endif // __DISPLAYCFG__