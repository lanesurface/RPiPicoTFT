
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "pico_tft.h"

const uint32_t SCN_DATA_BUFF_SZ=2<<7;
uint default_bd_rate=32*MHz;

void
tft_send_pgm(
  spi_inst_t * spi, 
  const uint8_t prog[])
{
  uint8_t n_cmds, cmd, n_args, addr;
  uint16_t ms;

  n_cmds=prog[addr++];
  while (n_cmds-->0) {
    cmd=prog[addr++], n_args=prog[addr++];
    ms=n_args&st_delay;
    gpio_set(TFT_CHX, 0);

    spi_write_blocking(
      spi, 
      prog+addr,
      1+n_args&(~st_delay)
    );

    if (ms) {
      ms=prog[addr++];
      if (ms==255) {
        ms=500;
      }
      sleep_ms(ms);
    }
  }
}

int
main()
{
  spi_inst_t * spi_ctx=spi_default;
  uint8_t data_buff[SCN_DATA_BUFF_SZ];
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
  
  gpio_set_function(TFT_MISO, GPIO_FUNC_SPI);
  gpio_set_function(TFT_MOSI, GPIO_FUNC_SPI);
  gpio_set_function(TFT_SCK, GPIO_FUNC_SPI);

  gpio_init(TFT_CHX);
  gpio_set_dir(TFT_CHX, GPIO_OUT);
  gpio_put(TFT_CHX, 1); // CHX is active hi

  // while (true) {
  //   if (spi_is_writable(spi_ctx)) {
  //     spi_write_blocking(
  //       spi_ctx, 
  //       data_buff, 
  //       SCN_DATA_BUFF_SZ
  //     );
  //   } else {
  //     sleep_ms(10);
  //     continue;
  //   }
  // }

  tft_send_pgm(spi_ctx, init_scr);
}