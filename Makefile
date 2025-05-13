#
#  Created by CHATGPT
#


CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
TARGET = bank_system

# Source directories
SRC_DIR = src
INCLUDE_DIR = include

# List all source files explicitly
SRCS = main.cpp \
       $(SRC_DIR)/Algorithms/Algorithms.cpp \
       $(SRC_DIR)/Algorithms/FileHandler.cpp \
       $(SRC_DIR)/Bank\ System/BankSystem.cpp \
       $(SRC_DIR)/System\ details/Account.cpp \
       $(SRC_DIR)/System\ details/CheckingAccount.cpp \
       $(SRC_DIR)/System\ details/SavingsAccount.cpp \
       $(SRC_DIR)/System\ details/Transaction.cpp \
       $(SRC_DIR)/Viewer\ POV/Menu.cpp

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