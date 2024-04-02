CC = g++
CFLAGS = -std=c++11 -Wall -Wextra
LIBS = -lcurl

SRCS = main.cpp message.cpp user.cpp call.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = discord_api

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS) $(TARGET)