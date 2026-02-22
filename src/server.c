#include "kmn_webconfig/server.h"
#include <pico/types.h>
#include <stdio.h>
#include <string.h>

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include "lwip/pbuf.h"
#include "lwip/sockets.h"
#include "lwip/tcp.h"

void Hostpot_Init() {
  if (cyw43_arch_init()) {
    return;
  }

  cyw43_arch_enable_ap_mode(SSID, PASSWD, CYW43_AUTH_WPA2_AES_PSK);
}
