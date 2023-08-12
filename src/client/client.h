#include <enet/enet.h>

typedef struct Client {
  ENetPeer *peer;
  ENetAddress *address;
  ENetHost *host;
} Client;

Client *create_client(const char *server, int port);
