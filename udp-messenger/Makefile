CC = gcc
CFLAGS = -Wall -Wextra -g -pthread
LDFLAGS = 

SRC_DIR = src
BUILD_DIR = build

TARGETS = $(BUILD_DIR)/server $(BUILD_DIR)/client

.PHONY: all clean

all: $(TARGETS)

$(BUILD_DIR)/server: $(SRC_DIR)/server.c | $(BUILD_DIR) 
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
$(BUILD_DIR)/client: $(SRC_DIR)/client.c | $(BUILD_DIR) 
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
	
$(BUILD_DIR): 
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)



