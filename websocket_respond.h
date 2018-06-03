#ifndef __WEBSOCKET_RESPOND__
#define __WEBSOCKET_RESPOND__

class Websocket_Respond {
public:
	Websocket_Respond();
	~Websocket_Respond();

	int makeFrame(const char* msg, int msg_length, char* buffer);
};

#endif
