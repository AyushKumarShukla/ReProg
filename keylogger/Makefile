CC = gcc
CFLAGS = -Wall
LDFLAGS =

SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/keylogger

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Device to read from — override at call time if needed:
# make run DEVICE=/dev/input/event5
DEVICE = /dev/input/event2

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: all
	sudo ./$(TARGET) $(DEVICE)

clean:
	rm -rf $(BUILD_DIR)
