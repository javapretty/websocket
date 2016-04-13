#ifndef __WEBSOCKET_HANDLER__
#define __WEBSOCKET_HANDLER__

#include <arpa/inet.h>
#include <iostream>
#include <map>
#include <sstream>
#include "base64.h"
#include "sha1.h"
#include "debug_log.h"
#include "websocket_request.h"

#define MAGIC_KEY "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

enum WEBSOCKET_STATUS {
	WEBSOCKET_UNCONNECT = 0,
	WEBSOCKET_HANDSHARKED = 1,
};

typedef std::map<std::string, std::string> HEADER_MAP;

class Websocket_Handler{
public:
	Websocket_Handler(int fd);
	~Websocket_Handler();
	int process();
	inline char *getbuff();
private:
	int handshark();
	void parse_str(char *request);
	int fetch_http_info();
	int send_data(char *buff);
private:
	char buff_[2048];
	WEBSOCKET_STATUS status_;
	HEADER_MAP header_map_;
	int fd_;
	Websocket_Request *request_;
};

inline char *Websocket_Handler::getbuff(){
	return buff_;
}

#endif
