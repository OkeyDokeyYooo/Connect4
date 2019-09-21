# the compiler: gcc for C program, define as g++ for C++
CC_C = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall -std=c++11

# the build target executable:
TARGET = connect4
OBJS = Board.o connect4.o

all: clean $(TARGET)

%.o : %.cpp
	$(CC) -c $(CFLAGS) $<

$(TARGET): $(OBJS)
	$(CC_C) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)