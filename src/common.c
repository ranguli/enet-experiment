#include <errno.h>
#include <enet/enet.h>
#include "common.h"
#include <string.h>
#include "log.h"

void handle_event_error(ENetEvent *event) {
  log_error("Event error: %s", strerror(errno));
}

void handle_event_occured(ENetEvent *event) {
  log_debug("Handling occured event");
  ENetPeer *peer = event->peer;
  ENetAddress address = event->peer->address;

  char hostname[256];
  enet_address_get_host_ip(&address, hostname, sizeof hostname);

  switch (event->type) {
  case ENET_EVENT_TYPE_CONNECT:
    log_info("A new peer connected from %s:%u.", hostname, event->peer->address.port);
    /* Store any relevant client information here. */
    event->peer->data = "Client information";
    break;
  case ENET_EVENT_TYPE_RECEIVE:
    log_debug("A packet of length %lu containing %s was received from %p on channel %u.\n",
           event->packet->dataLength, event->packet->data, event->peer->data, event->channelID);
    /* Clean up the packet now that we're done using it. */
    enet_packet_destroy(event->packet);
    break;

  case ENET_EVENT_TYPE_DISCONNECT:
    log_info("Peer %s:%u disconnected.", hostname, event->peer->address.port);
    /* Reset the peer's client information. */
    event->peer->data = NULL;
  }
}


// TODO: alternate idea for this function. It accepts four callback functions as arguments. One for each of the possibilities if an event occured (connect, disconnect, receive), and one for the event error.
// The switching logic as well as debug logging is common code that can be shared between the client and server, but the client and server code each pass their own role-specific functions. 

bool handle_event(int event_status, ENetEvent *event) {
  switch (event_status) {
  case EVENT_OCCURED:
    handle_event_occured(event);
  case EVENT_ERROR:
    handle_event_error(event);
  }
}

