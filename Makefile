CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 $(shell pkg-config --cflags raylib)
LDLIBS   := $(shell pkg-config --libs raylib) -lGL -lm -lpthread -ldl -lrt -lX11

SRC_DIR   := src
BUILD_DIR := build

TARGET := $(BUILD_DIR)/raycast
SRCS   := $(wildcard $(SRC_DIR)/*.cpp)
HDRS   := $(wildcard $(SRC_DIR)/*.h)
OBJS   := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HDRS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
