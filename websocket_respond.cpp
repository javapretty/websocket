#include "websocket_respond.h"
#include <string.h>
#include <arpa/inet.h>
#include <stdint.h>

Websocket_Respond::Websocket_Respond() { }

Websocket_Respond::~Websocket_Respond() { }

int Websocket_Respond::makeFrame(const char* msg, int msg_length, char* buffer) {
    int pos = 0;
    buffer[pos++] = 0x81; // text frame

    if (msg_length <= 125) {
        buffer[pos++] = msg_length;
    } else if (msg_length <= 65535) {
        buffer[pos++] = 126; //16 bit length follows
        uint16_t payload_len = htons(msg_length);
        memcpy((void *) (buffer + pos), &payload_len, 2);
        pos += 2;
    } else { // >2^16-1 (65535)
        buffer[pos++] = 127; //64 bit length follows

        uint32_t payload_len = ntohl(msg_length);
        memcpy((void *) (buffer + pos), &payload_len, 4);
        pos += 4;
    }
    memcpy((void *) (buffer + pos), msg, msg_length);
    return (msg_length + pos);
}