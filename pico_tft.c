
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "pico_tft.h"

const uint32_t SCN_DATA_BUFF_SZ=2<<8;
uint default_bd_rate=32*MHz;

int
main()
{
  spi_inst_t * spi_ctx=spi0;
  uint8_t data_buff[SCN_DATA_BUFF_SZ];
  uint baud_rt;

  stdio_init_all();
  baud_rt=spi_init(spi_ctx, default_bd_rate);
  spi_set_format(
    spi_ctx,
    8, // data_bits
    SPI_CPHA_0,
    SPI_CPOL_1,
    SPI_MSB_FIRST
  );

  while (true) {
    if (spi_is_writable(spi_ctx)) {
      spi_write_blocking(
        spi_ctx, 
        data_buff, 
        SCN_DATA_BUFF_SZ
      );
    } else {
      sleep_ms(10);
      continue;
    }
  }
}