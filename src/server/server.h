#include <enet/enet.h>

typedef struct Server {
  ENetHost *host;

} Server;

bool handle_event(int event_status, ENetEvent *event);
bool start_server(Server *server);
Server *create_server(const char *ip, int port, int max_clients);
