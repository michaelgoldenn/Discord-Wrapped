# Compiler
CC = g++
# Compiler flags
CFLAGS = -Wall -I/app/libs/libxlsxwriter/include
# Linker flags
LDFLAGS = -L/app/libs/libxlsxwriter/lib -lxlsxwriter

# Makefile
all: main

main: main.o message.o
	g++ main.o message.o -o main

main.o: main.cpp message.h
	g++ -c main.cpp

message.o: message.cpp message.h
	g++ -c message.cpp

clean:
	rm *.o main
