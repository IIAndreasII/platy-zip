# Variables
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
TARGET = plzip

# Automatically find all .c files in the src directory
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Generate object file names by replacing src/ with build/ and .c with .o
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Default target
all: $(BUILD_DIR) $(TARGET)

# Rule to build the target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# PHONY targets
.PHONY: all clean
