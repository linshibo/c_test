SRC = main.cpp net.cpp global.cpp
CC= g++
FLAG =  -Wall -g  
BIN=myserv
all:$(BIN)
$(BIN):$(SRC:.cpp=.o)
	$(CC) $(FLAG) -o $@ $^
%.o: %.cpp  makefile
	$(CC) $(FLAGS) -c  -o $@ $<
.PHONY:clean
clean:
	@-rm -f *.o *.oxx *.po *.so *.d .dep.* 

