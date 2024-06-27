/**
 * Implementations of the library functions which are specific to the ST7735
 * driver chip.
 *
 * Author(s): Surface, Lane W
 * Created: 06/22/2024
 */
#include "dspcfg.h"
#include "tftgfx.h"

/**
 * Chip-specific data structures, type definitions, constants, and function
 * implementations for the ST7735. Refer to the data sheet for more 
 * information about the operation of this chip.
 */
#ifndef __TFT_ST7735__
#define __TFT_ST7735__

/**
 * Instructions for the ST7735 chip.
 */
#define NOP     0x00
#define SWRST   0x01
#define SLPIN   0x10
#define SLPOUT  0x11
#define PLTON   0x12
#define NORON   0x13
#define INVOFF  0x20
#define INVON   0x21
#define GAMSET  0x26
#define DISPOFF 0x28
#define DISPON  0x29
#define CASET   0x2A
#define RASET   0x2B
#define RAMRW   0x2C
#define RAMRD   0x2E
#define PLTAR   0x30
#define TEOFF   0x34
#define TEON    0x35
#define MADCTR  0x36
#define IDMOFF  0x38
#define IDEON   0x39
#define COLMOD  0x3A

/**
 * When an instruction requires a delay before the next may be sent (eg, 
 * slp_off), st_delay may be used as a pseudo-instruction to provide this
 * functionality.
 * 
 * 1000 0000
 * ^..hi bit is set when a delay follows the instruction
 */
extern const uint8_t st_delay;

#endif // __TFT_ST7735__