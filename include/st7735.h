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
extern const uint8_t nop, sw_rst, slp_in, slp_out, plt_on,
  nor_on, inv_off, inv_on, gam_set, disp_off, disp_on,
  ca_set, ra_set, ram_rw, ram_rd, ptlar, te_off, te_on,
  mad_ctl, idm_off, ide_on, col_mod;

/**
 * When an instruction requires a delay before the next may be sent (eg, 
 * slp_off), st_delay may be used as a pseudo-instruction to provide this
 * functionality
 * 
 * 1000 0000
 * ^..hi bit is set when a delay follows the instruction
 */
extern const uint8_t st_delay;

#endif // __TFT_ST7735__