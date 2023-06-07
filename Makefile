CC = gcc-9
CFLAGS := -Wall -Wextra -g
INCLUDE = -Iinclude/ 

SRC_DIR := ./src
OBJ_DIR := ./obj
BIN_DIR := ./bin
INC_DIRS := -I./include/server -I./include/command -I./include/common -I/usr/include/PCSC

$(shell mkdir -p $(OBJ_DIR)/common $(OBJ_DIR)/server $(OBJ_DIR)/command)
$(shell mkdir -p $(BIN_DIR))

# Source files
COMMON_SRCS := $(wildcard $(SRC_DIR)/common/*.c)
SERVER_SRCS := $(wildcard $(SRC_DIR)/server/*.c)
COMMAND_SRCS := $(wildcard $(SRC_DIR)/command/*.c)

# Object files
COMMON_OBJS := $(patsubst $(SRC_DIR)/common/%.c,$(OBJ_DIR)/common/%.o,$(COMMON_SRCS))
SERVER_OBJS := $(patsubst $(SRC_DIR)/server/%.c,$(OBJ_DIR)/server/%.o,$(SERVER_SRCS))
COMMAND_OBJS := $(patsubst $(SRC_DIR)/command/%.c,$(OBJ_DIR)/command/%.o,$(COMMAND_SRCS))

# Output binaries
SERVER_TARGET := $(BIN_DIR)/server
COMMAND_TARGET := $(BIN_DIR)/command

# Default target
all: $(SERVER_TARGET) $(COMMAND_TARGET)

# Linking
$(SERVER_TARGET): $(COMMON_OBJS) $(SERVER_OBJS)
	$(CC) $(CFLAGS) $(INC_DIRS) $^ ./lib/libminiaudio.a -o $@ -pthread -ldl -lpthread -lm -lpcsclite

$(COMMAND_TARGET): $(COMMON_OBJS) $(COMMAND_OBJS)
	$(CC) $(CFLAGS) $(INC_DIRS) $^ -o $@ -pthread

# Compilation
$(OBJ_DIR)/common/%.o: $(SRC_DIR)/common/%.c
	$(CC) $(CFLAGS) $(INC_DIRS) -c $< -o $@ -pthread

$(OBJ_DIR)/server/%.o: $(SRC_DIR)/server/%.c
	$(CC) $(CFLAGS) $(INC_DIRS) -c $< -o $@ -pthread

$(OBJ_DIR)/command/%.o: $(SRC_DIR)/command/%.c
	$(CC) $(CFLAGS) $(INC_DIRS) -c $< -o $@ -pthread

# Clean
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

# Phony targets
.PHONY: all clean