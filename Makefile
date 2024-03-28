# Compiler
CC = g++
# Compiler flags
CFLAGS = -Wall -I/app/libs/libxlsxwriter/include
# Linker flags
LDFLAGS = -L/app/libs/libxlsxwriter/lib -lxlsxwriter

all: main messageClass

main: main.cpp
	$(CC) $(CFLAGS) -o main main.cpp $(LDFLAGS)

messageClass: messageClass.cpp
	$(CC) -o messageClass messageClass.cpp $(LDFLAGS)
