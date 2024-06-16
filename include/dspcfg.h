#include <stdint.h>

#ifndef __DSPCFG__
#define __DSPCFG__

typedef unsigned int uint;

typedef enum 
{ 
  x4kC=12,  // 4,096 color mode
  x65kC=16, // 65,536 color mode
  x262kC=18 // 262,144 color mode
} clr_accuracy;

#define N_COLOR_BITS x262kC
/* --Do Not Modify-- */
#if N_COLOR_BITS==x4kC
#define RED_FDWTH 4
#define GRN_FDWTH 4
#define BLU_FDWTH 4
#endif
#if N_COLOR_BITS==x65kC
#define RED_FDWTH 5
#define GRN_FDWTH 6
#define BLU_FDWTH 5
#endif
#if N_COLOR_BITS==x262kC
#define RED_FDWTH 6
#define GRN_FDWTH 6
#define BLU_FDWTH 6
#endif
/* --- */

#define TFT_WIDTH  128
#define TFT_HEIGHT 160

#define TFT_MISO 6 // Master In/Slave Out (SDI)
#define TFT_MOSI 3 // Master Out/Slave In (SDO)
#define TFT_CHX  5 // Chip Select
#define TFT_SCK  2 // Serial Clock
#define TFT_DCX  7 // Data/Command Select

/**
 * Define the appropriate driver chip for your configuration:
 * ST7735R, ILI9341, etc.
 */
#define ST7735R

#endif // __DSPCFG__