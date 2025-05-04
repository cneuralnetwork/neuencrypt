CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./include
LDFLAGS = -lssl -lcrypto

SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

TARGET = neuencrypt

$(shell mkdir -p $(OBJ_DIR))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

install: $(TARGET)
	install -m 755 $(TARGET) /usr/local/bin/

.PHONY: all clean install
