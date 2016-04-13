CC = g++
FLAG = -g 
INCLUDE = 
LIBDIR =
LIB = 
BIN = 
TARGET = websocketserver
SRCS = base64.cpp sha1.cpp network_interface.cpp debug_log.cpp websocket_handler.cpp \
		websocket_request.cpp main.cpp

$(TARGET):$(SRCS:.cpp=.o)
	$(CC) $(FLAG) $(LIBDIR) $(LIB) -o $@ $^
	-rm -f *.o *.d

%.o:%.cpp
	$(CC) $(FLAG) $(INCLUDE) -c -o $@ $<

clean:
	-rm -f *.o *.d
