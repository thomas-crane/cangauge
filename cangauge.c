#include <stdio.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "mcp/mcp.h"

int main()
{
  mcp_can_packet_t packet = {0};
  const uint LED_PIN = PICO_DEFAULT_LED_PIN;
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  stdio_init_all();

  // Warm up the serial.
  // TODO: there is a better way of doing this. wait_for_usb or something.
  for (int i = 0; i < 10; i++) {
    printf("starting...\n");
    sleep_ms(250);
  }

  // I2C init.
  i2c_init(i2c_default, 100000);
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
  printf("I2C init.\n");

  // MCP init.
  uint8_t baud_rate = 16;
  mcp_write_reg(MCP_REG_BAUD, &baud_rate, 1);
  printf("MCP init.\n");

  while (true) {
    uint8_t cnt = mcp_packet_count();
    if (cnt > 0) {
      if (!mcp_read_packet(&packet)) {
        continue;
      }
      mcp_print_packet(&packet);
    }
  }
}
