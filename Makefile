CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 $(shell pkg-config --cflags raylib)
LDLIBS   := $(shell pkg-config --libs raylib) -lGL -lm -lpthread -ldl -lrt -lX11

TARGET := raycast
SRCS   := main.cpp grid.cpp player.cpp
OBJS   := $(SRCS:.cpp=.o)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDLIBS)

%.o: %.cpp grid.h player.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
