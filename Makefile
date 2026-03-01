CC = gcc
CFLAGS = -Wall -Wextra -g -O2 `pkg-config --cflags gtk+-3.0` -Iinclude -Isrc
LIBS = `pkg-config --libs gtk+-3.0`
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TARGET = $(BIN_DIR)/vsysinfo

# List all C files recursively
SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Subdirectories in obj corresponding to src
OBJ_SUBDIRS = $(sort $(dir $(OBJS)))

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_SUBDIRS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_SUBDIRS):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run: all
	./$(TARGET)
