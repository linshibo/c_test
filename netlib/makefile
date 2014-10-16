SRC = Poller.cpp Channel.cpp EventLoop.cpp main.cpp 
CC= g++
FLAG =  -Wall -g -std=c++11 -Wc++11-extensions
BIN=myserv
all:$(BIN)
$(BIN):$(SRC:.cpp=.o)
	$(CC) $(FLAG) -o $@ $^
%.o: %.cpp  makefile
	$(CC) $(FLAGS) -c  -o $@ $<
.PHONY:clean
clean:
	@-rm -f *.o *.oxx *.po *.so *.d .dep.* 

