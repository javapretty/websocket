#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include "debug_log.h"
#include "network_interface.h"
#include "broadcast_queue.h"
#include "websocket_respond.h"


NetworkInterface::NetworkInterface():
		epollfd_(0),
		listenfd_(0),
		websocket_handler_map_()
{
	if(0 != init())
		exit(1);
}

NetworkInterface::~NetworkInterface(){

}

int NetworkInterface::init(){
	listenfd_ = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd_ == -1){
		DEBUG_LOG("创建套接字失败!");
		return -1;
	}
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);
	int opt = 1;
	setsockopt(listenfd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if(-1 == bind(listenfd_, (struct sockaddr *)(&server_addr), sizeof(server_addr))){
		DEBUG_LOG("绑定套接字失败!");
		return -1;
	}
	if(-1 == listen(listenfd_, 5)){
		DEBUG_LOG("监听失败!");
		return -1;
	}
	epollfd_ = epoll_create(MAXEVENTSSIZE);

	ctl_event(listenfd_, true);	
	DEBUG_LOG("服务器启动成功!");
	return 0;
}

int NetworkInterface::epoll_loop(){
	struct sockaddr_in client_addr;
	socklen_t clilen;
	int nfds = 0;
	int fd = 0;
	int bufflen = 0;
	struct epoll_event events[MAXEVENTSSIZE];
	while(true){
		nfds = epoll_wait(epollfd_, events, MAXEVENTSSIZE, TIMEWAIT);
		for(int i = 0; i < nfds; i++){
			if(events[i].data.fd == listenfd_){
				fd = accept(listenfd_, (struct sockaddr *)&client_addr, &clilen);
				ctl_event(fd, true);
			}
			else if(events[i].events & EPOLLIN){
				if((fd = events[i].data.fd) < 0)
					continue;
				Websocket_Handler *handler = websocket_handler_map_[fd];
				if(handler == NULL)
					continue;
				if((bufflen = read(fd, handler->getbuff(), BUFFLEN)) <= 0){
					ctl_event(fd, false);
				}
				else{
					handler->process();
				}
			}
		}
		comsume_msg();

	}

	return 0;
}

int NetworkInterface::comsume_msg() {
	int size = BROAD_QUEUE_PTR->queueSize();
	Websocket_Respond respond;

	while (size-- > 0) {
		BroadcastMsg msg;
		BROAD_QUEUE_PTR->getMsg(msg);
		char buf[BUFFLEN] = {0};
		int len = respond.makeFrame(msg.data.c_str(), msg.data.length(), buf);
		broadcast(buf, len);
	}
}

int NetworkInterface::set_noblock(int fd){
	int flags;
    if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void NetworkInterface::ctl_event(int fd, bool flag){
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = flag ? EPOLLIN : 0;
	epoll_ctl(epollfd_, flag ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
	if(flag){
		set_noblock(fd);
		websocket_handler_map_[fd] = new Websocket_Handler(fd);
		if(fd != listenfd_)
			DEBUG_LOG("fd: %d 加入epoll循环", fd);
	}
	else{
		close(fd);
		delete websocket_handler_map_[fd];
		websocket_handler_map_.erase(fd);
		DEBUG_LOG("fd: %d 退出epoll循环", fd);
	}
}

void NetworkInterface::run(){
	epoll_loop();
}

int NetworkInterface::broadcast(const char *data, int len) {
	if (data == NULL || len <= 0) {
		return 0;
	}
	WEB_SOCKET_HANDLER_MAP::iterator iter = websocket_handler_map_.begin();
	for (; iter != websocket_handler_map_.end(); ++iter) {
		if (iter->first == listenfd_) {
			continue;
		}
		if (singleSend(iter->first, data, len) < 0) {
			//ctl_event(iter->first, false);
		}
	}
	return len;
}

int NetworkInterface::singleSend(int fd, const char *data, int len) {
	int n = 0, sendSize = 0;
	while (sendSize < len) {
		if ((n = write(fd, data+sendSize, len-sendSize)) <= 0) {
			DEBUG_LOG("write packet error:(%s)", strerror(errno));
			return -1;
		}
		sendSize += n;
	}
	return sendSize;
}