# Compiler
CC = g++
# Compiler flags
CFLAGS = -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference -g3 -O0
# Linker flags
LDFLAGS =

# Source files
SRCS = main.cpp message.cpp user.cpp
# Object files
OBJS = $(SRCS:.cpp=.o)

# Makefile
all: main

main: $(OBJS)
	$(CC) $(OBJS) -o main $(LDFLAGS)

# Rule to compile .cpp files to .o files
%.o: %.cpp %.h
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o main
