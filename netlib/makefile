SRC =  EventLoop.cpp Channel.cpp Poller.cpp Buffer.cpp Socket.cpp TcpServer.cpp main.cpp 
CC= g++
FLAG =  -Wall -g -std=c++11 -lglog
BIN=myserv
all:$(BIN)
$(BIN):$(SRC:.cpp=.o)
	$(CC) $(FLAG) -o $@ $^
%.o: %.cpp  makefile
	$(CC) $(FLAG) -c  -o $@ $<
.PHONY:clean
clean:
	@-rm -f *.o *.oxx *.po *.so *.d .dep.* 

