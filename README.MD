# Protocol Specification

## Types of messages

The principle is that a message starts with an ID number which indicates what the message is for, and how the rest of the message should be interpreted. 

### Message Reference

| Message type | ID | Parameters             |                           |                           |                           | Payload Size |
|--------------|----|------------------------|---------------------------|---------------------------|---------------------------|--------------|
| POSITION     | 1 (`char`)  | Object ID (`uint64_t`) | X Position ( `uint64_t` ) | Y Position ( `uint64_t` ) | Z Position ( `uint64_t` ) | 12 bytes     |
|              |    |                        |                           |                           |                           |              |
|              |    |                        |                           |                           |                           |              |


#### POSITION

A position message tells a host where a given game object (denoted by its ID) should exist in the game world with x, y, z coordinates. A client sends a position message to the server telling it where the client wants to be. A server sends a position message telling a client where it must be.

_TODO_: We'd need to actually handle disagreements between the client and server on positioning. The server should be authoritative but we'd need to implement some kind of way to mediate. Maybe client-side prediction.