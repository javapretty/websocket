#include <unistd.h>
#include "websocket_handler.h"

Websocket_Handler::Websocket_Handler(int fd):
		buff_(),
		status_(WEBSOCKET_UNCONNECT),
		header_map_(),
		fd_(fd),
		request_(new Websocket_Request)
{
}

Websocket_Handler::~Websocket_Handler(){
}

int Websocket_Handler::process(){
	if(status_ == WEBSOCKET_UNCONNECT){
		return handshark();
	}
	request_->fetch_websocket_info(buff_);
	request_->print();
	memset(buff_, 0, sizeof(buff_));
	return 0;
}

int Websocket_Handler::handshark(){
	char request[1024] = {};
	status_ = WEBSOCKET_HANDSHARKED;
	fetch_http_info();
	parse_str(request);
	memset(buff_, 0, sizeof(buff_));
	return send_data(request);
}

void Websocket_Handler::parse_str(char *request){  
	strcat(request, "HTTP/1.1 101 Switching Protocols\r\n");
	strcat(request, "Connection: upgrade\r\n");
	strcat(request, "Sec-WebSocket-Accept: ");
	std::string server_key = header_map_["Sec-WebSocket-Key"];
	server_key += MAGIC_KEY;

	SHA1 sha;
	unsigned int message_digest[5];
	sha.Reset();
	sha << server_key.c_str();

	sha.Result(message_digest);
	for (int i = 0; i < 5; i++) {
		message_digest[i] = htonl(message_digest[i]);
	}
	server_key = base64_encode(reinterpret_cast<const unsigned char*>(message_digest),20);
	server_key += "\r\n";
	strcat(request, server_key.c_str());
	strcat(request, "Upgrade: websocket\r\n\r\n");
}

int Websocket_Handler::fetch_http_info(){
	std::istringstream s(buff_);
	std::string request;

	std::getline(s, request);
	if (request[request.size()-1] == '\r') {
		request.erase(request.end()-1);
	} else {
		return -1;
	}

	std::string header;
	std::string::size_type end;

	while (std::getline(s, header) && header != "\r") {
		if (header[header.size()-1] != '\r') {
			continue; //end
		} else {
			header.erase(header.end()-1);	//remove last char
		}

		end = header.find(": ",0);
		if (end != std::string::npos) {
			std::string key = header.substr(0,end);
			std::string value = header.substr(end+2);
			header_map_[key] = value;
		}
	}

	return 0;
}

int Websocket_Handler::send_data(char *buff){
	return write(fd_, buff, strlen(buff));
}
