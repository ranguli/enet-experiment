#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../common.h"
#include "log.h"
#include "server.h"
#include <enet/enet.h>

// bool start_server(Server *server) { return false; }

// Given an optional string such as "127.0.0.1"
Server *create_server(const char *ip, int port, int max_clients) {

  Server server;
  ENetAddress address;
  ENetHost *host;

  log_info("Creating server...");

  if (ip == NULL) {
    log_info("Setting host to 0.0.0.0 (No host provided");
    address.host = ENET_HOST_ANY;
  } else {
    log_info("Setting host to %s", ip);
    address.host = enet_address_set_host(&address, ip);
  }

  log_debug("Set host to %s", ip);

  if (port == 0) {
    log_info("Setting to random port (No port specified)");
    address.port = ENET_PORT_ANY;
  } else {
    log_info("Setting port to %d", port);
    address.port = port;
  }
  log_debug("Set port to %d", address.port);

  host = enet_host_create(&address /* the address to bind the server host to */,
                          max_clients /* allow up to 32 clients and/or outgoing
                                         add_library(lib_third_party_ti_mmcsdlib "connections */
                          ,
                          2 /* allow up to 2 channels to be used, 0 and 1 */,
                          0 /* assume any amount of incoming bandwidth */,
                          0 /* assume any amount of outgoing bandwidth */);

  if (host == NULL) {
    log_error("An error occurred while trying to create an ENet server host.\n");
    exit(EXIT_FAILURE);
  }

  server.host = host;

  Server *pServer = &server;
  return pServer;
}

void stop_server(Server *server) {
  enet_host_destroy(server->host);
}
int main() {

  if (enet_initialize() != 0) {
    log_fatal("An error occurred while initializing ENet: %s", strerror(errno));
    return EXIT_FAILURE;
  }
  atexit(enet_deinitialize);

  ENetEvent event;
  Server *server = create_server(NULL, 32438, 64);
  log_debug("Server created");
  ENetHost *host = server->host;

  enet_uint32 timeout = 10;

  log_debug("Polling for events");
  while (true) {
    int event_status = enet_host_service(host, &event, timeout);
    handle_event(event_status, &event);
  }

  enet_host_destroy(host);
  return 0;
}