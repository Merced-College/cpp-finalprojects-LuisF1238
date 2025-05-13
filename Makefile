CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
TARGET = bank_system

# Source directories
SRC_DIR = src
INCLUDE_DIR = include

# Find all source files
SRCS = main.cpp \
       $(wildcard $(SRC_DIR)/models/*.cpp) \
       $(wildcard $(SRC_DIR)/controllers/*.cpp) \
       $(wildcard $(SRC_DIR)/utils/*.cpp) \
       $(wildcard $(SRC_DIR)/views/*.cpp)

# Create object file names
OBJS = $(SRCS:.cpp=.o)

# Include path
INCLUDES = -I$(INCLUDE_DIR)

# Data directory
DATA_DIR = data

all: directories $(TARGET)

# Rule to create the target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Create directories if they don't exist
directories:
	mkdir -p $(DATA_DIR)

clean:
	rm -f $(OBJS) $(TARGET)

run: all
	./$(TARGET)

.PHONY: all clean run directories