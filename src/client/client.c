#include <errno.h>
#include <stdbool.h>
#include <stdio.h>

#include "../common.h"
#include "client.h"
#include "log.h"
#include <enet/enet.h>

ENetHost *create_client_host() {
  /** Create an ENetHost that will act as a client. */
  ENetHost *host;

  // How many peers should be able to connect to this host? Only the server.
  int peerCount = 1;
  // ENet allows multiple "channels" of communication over a connection.
  int channelLimit = 2;
  // Unlimited bandwidth - no restrictions.
  int incomingBandwidth = 0;
  int outgoingBandwidth = 0;

  log_debug("Creating a client ENetHost with (peerCount:%d, channelLimit:%d, incomingBandwidth:%d, "
            "outgoingBandwith:%d)",
            peerCount, channelLimit, incomingBandwidth, outgoingBandwidth);
  return enet_host_create(NULL, peerCount, channelLimit, incomingBandwidth, outgoingBandwidth);
}

Client *create_client(const char *server_address, int port) {
  ENetAddress address;

  log_info("Client will connect to %s:%d", server_address, port);

  if (enet_address_set_host(&address, server_address) != 0) {
    log_fatal("An error occurred while trying to set the address.");
    exit(EXIT_FAILURE);
  };

  address.port = port;

  // Create an ENetHost we can use as a client.
  ENetHost *host = create_client_host();

  if (host == NULL) {
    log_fatal("An error occurred while trying to create an ENet client host.");
    exit(EXIT_FAILURE);
  }

  // Create an ENetPeer, from the clients perspective our peer is the server.
  int channelCount = 2;
  int userData = 0; // I don't really know?
  ENetPeer *server = enet_host_connect(host, &address, channelCount, userData);
  if (server == NULL) {
    log_fatal("An error occured creating an ENetPeer for the server.");
  }

  Client client;
  client.peer = server;
  client.host = host;

  Client *pClient = &client;

  return pClient;
}

int main(int argc, char *argv[]) {
  Client *client = create_client("127.0.0.1", 32438);
  ENetHost *host = client->host;
  ENetPeer *server = client->peer;
  ENetAddress address = client->peer->address;

  char hostname[256];
  enet_address_get_host_ip(&address, hostname, sizeof hostname);

  ENetEvent event;

  enet_uint32 timeout = 10;


  /* Wait up to 5 seconds for the connection attempt to succeed. */
  if (enet_host_service(host, &event, 1000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
    log_info("Connection succeeded to %s", hostname);
  } else {
    /* Either the 5 seconds are up or a disconnect event was */
    /* received. Reset the peer in the event the 5 seconds   */
    /* had run out without any significant event.            */
    enet_peer_reset(server);
    log_warn("Connection to %s failed.", hostname);
    enet_host_destroy(host);
  }

  log_debug("Polling for events");
  while (true) {
    int event_status = enet_host_service(host, &event, timeout);
    handle_event(event_status, &event);
  }

  enet_host_destroy(host);
}