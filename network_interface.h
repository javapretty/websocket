#ifndef __NETWORK_INTERFACE__
#define __NETWORK_INTERFACE__

#include "websocket_handler.h"
#include "singleton.h"

#define PORT 9000
#define TIMEWAIT 100
#define BUFFLEN 2048
#define MAXEVENTSSIZE 20

typedef std::map<int, Websocket_Handler *> WEB_SOCKET_HANDLER_MAP;

class NetworkInterface {
public:
	NetworkInterface();
	~NetworkInterface();

private:
	int init();
	int epoll_loop();
	int set_noblock(int fd);
	void ctl_event(int fd, bool flag);

	int comsume_msg();

public:
	void run();
	int broadcast(const char* data, int len);
	int singleSend(int fd, const char* data, int len);
private:
	int epollfd_;
	int listenfd_;
	WEB_SOCKET_HANDLER_MAP websocket_handler_map_;
};

#define NETWORK_INTERFACE Singleton<NetworkInterface>::Instance()

#endif
