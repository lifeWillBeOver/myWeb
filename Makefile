CC=g++
RM=rm
CFLAGS = -std=c++11 -g 
SRC = ./src

CLIENT:=client
SERVER:=server
DEBUG:=debug
RELEASE:=release


CLIENTO = build/client.o build/ipPort.o build/util.o
SERVERO = build/server.o build/ipPort.o build/util.o

all: submodule $(DEBUG)

$(CLIENT): CFLAGS += -c 
$(CLIENT): $(SRC)/client.cpp
	$(CC) $(CFLAGS) $(SRC)/client.cpp -o build/client.o

$(SERVER): CFLAGS += -c 
$(SERVER):$(SRC)/server.cpp 
	$(CC) $(CFLAGS) $(SRC)/server.cpp -o build/server.o

.PHONY: clean 

clean:
	-$(RM) -f build/*.o

$(DEBUG): CFLAGS += -D __DEBUG__
$(DEBUG): $(CLIENT) $(SERVER)
	$(CC) $(CFLAGS) $(CLIENTO) -o bin/client
	$(CC) $(CFLAGS) $(SERVERO) -o bin/server

$(RELEASE):$(CLIENT) $(SERVER)
	$(CC) $(CFLAGS) $(CLIENTO) -o bin/client
	$(CC) $(CFLAGS) $(SERVERO) -o bin/server

submodule:
	make -C utils
