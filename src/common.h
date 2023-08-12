#include <stdbool.h>
#include <enet/enet.h>

#define EVENT_OCCURED 1
#define EVENT_ERROR -1
#define EVENT_NO_EVENT 0


void handle_event_error(ENetEvent *event);
void handle_event_occured(ENetEvent *event);
bool handle_event(int event_status, ENetEvent *event);