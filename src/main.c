#include "kmn_webconfig/server.h"

#include "pico/stdlib.h"

ServerConfig webConfConfig;

/// Test
int main() {
  stdio_init_all();

  while (!stdio_usb_connected()) {
    sleep_ms(100);
  }

  webconf_init_config(&webConfConfig, "WebConf", "12345678");
  webconf_start(&webConfConfig);
  while (1) {
  }
  webconf_stop(&webConfConfig);
}