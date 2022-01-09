CC=g++
RM=rm
CFLAGS = -c -std=c++11 -g
BUILD = ../build

IPPORT := ipPort 
UTIL := util

all: $(IPPORT) $(UTIL)


$(IPPORT): ipPort.cpp ipPort.hpp
	$(CC) $(CFLAGS) ipPort.cpp -o $(BUILD)/ipPort.o

$(UTIL): util.cpp util.hpp
	$(CC) $(CFLAGS) util.cpp -o $(BUILD)/util.o