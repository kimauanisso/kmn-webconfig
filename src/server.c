#include "kmn_webconfig/server.h"

#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <pico/types.h>
#include <stdio.h>
#include <string.h>

#include "dhcpserver/dhcpserver.h"
#include "dnsserver/dnsserver.h"
#include "kmn_webconfig/handlers/routes.h"
#include "kmn_webconfig/http_response.h"
#include "kmn_webconfig/router.h"

#define TCP_PORT 80
#define DEBUG_printf printf
#define POLL_TIME_S 5
#define HTTP_GET "GET"

typedef struct TCP_CONNECT_STATE_T_ {
  struct tcp_pcb *pcb;
  int sent_len;
  HttpBuffer request;
  HttpBuffer response;
  ip_addr_t *gw;
} TCP_CONNECT_STATE_T;

static err_t tcp_close_client_connection(TCP_CONNECT_STATE_T *con_state,
                                         struct tcp_pcb *client_pcb,
                                         err_t close_err) {
  if (client_pcb) {
    assert(con_state && con_state->pcb == client_pcb);
    tcp_arg(client_pcb, NULL);
    tcp_poll(client_pcb, NULL, 0);
    tcp_sent(client_pcb, NULL);
    tcp_recv(client_pcb, NULL);
    tcp_err(client_pcb, NULL);
    err_t err = tcp_close(client_pcb);
    if (err != ERR_OK) {
      DEBUG_printf("close failed %d, calling abort\n", err);
      tcp_abort(client_pcb);
      close_err = ERR_ABRT;
    }
    if (con_state) {
      free(con_state);
    }
  }
  return close_err;
}

static void tcp_server_close(TCP_SERVER_T *state) {
  if (state->server_pcb) {
    tcp_arg(state->server_pcb, NULL);
    tcp_close(state->server_pcb);
    state->server_pcb = NULL;
  }
}

static err_t tcp_server_sent(void *arg, struct tcp_pcb *pcb, u16_t len) {
  TCP_CONNECT_STATE_T *con_state = (TCP_CONNECT_STATE_T *)arg;
  DEBUG_printf("tcp_server_sent %u\n", len);
  con_state->sent_len += len;

  int allSent =
      con_state->sent_len >= con_state->request.used + con_state->response.used;
  if (allSent) {
    DEBUG_printf("all done\n");
    return tcp_close_client_connection(con_state, pcb, ERR_OK);
  }

  return ERR_OK;
}

err_t tcp_server_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p,
                      err_t err) {
  TCP_CONNECT_STATE_T *con_state = (TCP_CONNECT_STATE_T *)arg;
  if (!p) {
    DEBUG_printf("connection closed\n");
    return tcp_close_client_connection(con_state, pcb, ERR_OK);
  }
  assert(con_state && con_state->pcb == pcb);
  if (p->tot_len > 0) {
    DEBUG_printf("tcp_server_recv %d err %d\n", p->tot_len, err);
    // Copy the request into the buffer
    u16_t bufSize = p->tot_len > con_state->request.size - 1
                        ? con_state->request.size - 1
                        : p->tot_len;
    pbuf_copy_partial(p, con_state->request.buffer, bufSize, 0);
    DEBUG_printf("tcp_server_recv %s err %d\n", con_state->request.buffer, err);

    // TODO: Handle memory errors
    handleRoute(&con_state->request, &con_state->response);

    if (con_state->response.used) {
      err = tcp_write(pcb, con_state->response.buffer, con_state->response.used,
                      0);
      DEBUG_printf("Sent: %s\n", con_state->response.buffer);
      if (err != ERR_OK) {
        DEBUG_printf("failed to write result data %d\n", err);
        return tcp_close_client_connection(con_state, pcb, err);
      }
    }
    tcp_recved(pcb, p->tot_len);
  }
  pbuf_free(p);
  return ERR_OK;
}

static err_t tcp_server_poll(void *arg, struct tcp_pcb *pcb) {
  TCP_CONNECT_STATE_T *con_state = (TCP_CONNECT_STATE_T *)arg;
  DEBUG_printf("tcp_server_poll_fn\n");
  return tcp_close_client_connection(con_state, pcb,
                                     ERR_OK); // Just disconnect clent?
}

static void tcp_server_err(void *arg, err_t err) {
  TCP_CONNECT_STATE_T *con_state = (TCP_CONNECT_STATE_T *)arg;
  if (err != ERR_ABRT) {
    DEBUG_printf("tcp_client_err_fn %d\n", err);
    tcp_close_client_connection(con_state, con_state->pcb, err);
  }
}

static err_t tcp_server_accept(void *arg, struct tcp_pcb *client_pcb,
                               err_t err) {
  TCP_SERVER_T *state = (TCP_SERVER_T *)arg;
  if (err != ERR_OK || client_pcb == NULL) {
    DEBUG_printf("failure in accept\n");
    return ERR_VAL;
  }
  DEBUG_printf("client connected\n");

  // Create the state for the connection
  TCP_CONNECT_STATE_T *con_state = calloc(1, sizeof(TCP_CONNECT_STATE_T));
  con_state->request.size = MAX_BUFFER_SIZE;
  con_state->response.size = MAX_BUFFER_SIZE;

  if (!con_state) {
    DEBUG_printf("failed to allocate connect state\n");
    return ERR_MEM;
  }
  con_state->pcb = client_pcb; // for checking
  con_state->gw = &state->gw;

  // setup connection to client
  tcp_arg(client_pcb, con_state);
  tcp_sent(client_pcb, tcp_server_sent);
  tcp_recv(client_pcb, tcp_server_recv);
  tcp_poll(client_pcb, tcp_server_poll, POLL_TIME_S * 2);
  tcp_err(client_pcb, tcp_server_err);

  return ERR_OK;
}

static bool tcp_server_open(void *arg, const char *ap_name) {
  TCP_SERVER_T *state = (TCP_SERVER_T *)arg;
  DEBUG_printf("starting server on port %d\n", TCP_PORT);

  struct tcp_pcb *pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
  if (!pcb) {
    DEBUG_printf("failed to create pcb\n");
    return false;
  }

  err_t err = tcp_bind(pcb, IP_ANY_TYPE, TCP_PORT);
  if (err) {
    DEBUG_printf("failed to bind to port %d\n", TCP_PORT);
    return false;
  }

  state->server_pcb = tcp_listen_with_backlog(pcb, 1);
  if (!state->server_pcb) {
    DEBUG_printf("failed to listen\n");
    if (pcb) {
      tcp_close(pcb);
    }
    return false;
  }

  tcp_arg(state->server_pcb, state);
  tcp_accept(state->server_pcb, tcp_server_accept);

  printf("Try connecting to '%s' (press 'd' to disable access point)\n",
         ap_name);
  return true;
}

int webconf_init_config(ServerConfig *config, const char *ssid,
                        const char *passwd) {
  config->ssid = ssid;
  config->passwd = passwd;
  config->tcp_server = calloc(1, sizeof(TCP_SERVER_T));

  if (!config->tcp_server) {
    DEBUG_printf("failed to allocate state\n");
    return 1;
  }
}

int webconf_start(ServerConfig *config) {
  init_routes();

  if (cyw43_arch_init()) {
    DEBUG_printf("failed to initialise\n");
    return 1;
  }

  cyw43_arch_enable_ap_mode(config->ssid, config->passwd,
                            CYW43_AUTH_WPA2_AES_PSK);

  ip_addr_t *gw = &config->tcp_server->gw;
  gw->addr = PP_HTONL(CYW43_DEFAULT_IP_AP_ADDRESS);

  ip4_addr_t mask;
  mask.addr = PP_HTONL(CYW43_DEFAULT_IP_MASK);

  // Start the dns and dhcp servers
  dhcp_server_init(&config->dhcp_server, gw, &mask);
  dns_server_init(&config->dns_server, gw);

  if (!tcp_server_open(config->tcp_server, config->ssid)) {
    DEBUG_printf("failed to open server\n");
    return 1;
  }

  config->tcp_server->complete = false;
}

int webconf_stop(ServerConfig *config) {
  tcp_server_close(config->tcp_server);
  dns_server_deinit(&config->dns_server);
  dhcp_server_deinit(&config->dhcp_server);
  cyw43_arch_deinit();
}