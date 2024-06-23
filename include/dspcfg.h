#include <stdint.h>

#ifndef __DSPCFG__
#define __DSPCFG__

typedef unsigned int uint;

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

/**
 * Choose one of these options: x4kC (12 bit color), x65kC (16 bit color), and 
 * x262kC (18 bit color).
 */
#define x262kC
#define NUM_CLR_BITS 18
#define NUM_CLR_COMPS 3
// ---
// #define x65kC
// #define NUM_CLR_BITS 16
// ---
// #define x4kC
// #define NUM_CLR_BITS 12

/* --Do Not Modify-- */
#if defined(x4kC)
# define RED_FDWTH 4
# define GRN_FDWTH 4
# define BLU_FDWTH 4
#elif defined(x65kC)
# define RED_FDWTH 5
# define GRN_FDWTH 6
# define BLU_FDWTH 5
#elif defined(x262kC)
# define RED_FDWTH 6
# define GRN_FDWTH 6
# define BLU_FDWTH 6
#endif
/* --- */

#endif // __DSPCFG__