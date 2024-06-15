
#include <stdio.h>
#include <stdarg.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "pico_tft.h"
#include "tft_gfx.h"

const uint32_t SCN_DATA_BUFF_SZ=2<<7;
uint default_bd_rate=32*MHz;

void
tft_send_pgm(
  spi_inst_t * spi, 
  const uint8_t prog[])
{
  uint8_t n_cmds, cmd, n_args, addr;
  uint16_t delay; /* uint16_t b/c delay may be 500 ms when 0xFF read */

  n_cmds=prog[addr++];
  while (n_cmds-->0) {
    cmd=prog[addr++], n_args=prog[addr++];
    delay=n_args&st_delay;
    gpio_put(TFT_CHX, 0);

    spi_write_blocking(
      spi, 
      prog+addr,
      1+n_args&(~st_delay)
    );

    if (delay) {
      delay=prog[addr++];
      if (delay==255) {
        delay=500;
      }
      sleep_ms(delay);
    }
  }
}

static void
__decl_spi_pins(size_t n, ...)
{
  va_list vla;
  va_start(vla, n);
  while (n-->0)
  {
    uint ag=va_arg(vla, uint);
    gpio_set_function(ag, GPIO_FUNC_SPI);
  }
  va_end(vla);
}

void
tft_init_ctx()
{
  __decl_spi_pins(TFT_MISO, TFT_MOSI, TFT_SCK);
  gpio_init(TFT_CHX);
  gpio_set_dir(TFT_CHX, GPIO_OUT);
  gpio_put(TFT_CHX, 1);
}

int
main()
{
  spi_inst_t * spi_ctx=spi_default;
  uint baud_rt;

  stdio_init_all();
  baud_rt=spi_init(spi_ctx, default_bd_rate);
  spi_set_format(
    spi_ctx,
    8, // data_bits
    SPI_CPHA_1,
    SPI_CPOL_0,
    SPI_MSB_FIRST
  );
  
  tft_init_ctx();
  tft_send_pgm(spi_ctx, init_scr);
}