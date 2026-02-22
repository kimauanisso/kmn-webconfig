#pragma once

#include "dhcpserver/dhcpserver.h"
#include "dnsserver/dnsserver.h"

typedef struct TCP_SERVER_T_ {
  struct tcp_pcb *server_pcb;
  bool complete;
  ip_addr_t gw;
} TCP_SERVER_T;

typedef struct ServerConfig {
  const char *ssid;
  const char *passwd;
  TCP_SERVER_T *tcp_server;
  dhcp_server_t dhcp_server;
  dns_server_t dns_server;
} ServerConfig;

/// @brief Creates server's config
/// @param ssid Networks' SSID
/// @param passwd Networks' Password
int webconf_init_config(ServerConfig *config, const char *ssid,
                        const char *passwd);

/// @brief Starts webconfig server
int webconf_start(ServerConfig *config);

/// @brief Stops webconfig server
int webconf_stop(ServerConfig *config);