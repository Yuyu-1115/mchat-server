TARGET_DIR := build

SERVER_EXEC := mchat_server
CLIENT_EXEC := mchat_client

CC = gcc
CFLAGS = -g -Wall -Wextra -MMD -MP
CPPFLAGS = -Isrc
LDFLAGS = -lpthread -lncurses

CLIENT_SRC := $(wildcard src/client/*.c)
SERVER_SRC := $(wildcard src/server/*.c)
COMMON_SRC := $(wildcard src/common/*.c)

CLIENT_OBJ := $(patsubst src/client/%.c,$(TARGET_DIR)/client/%.o,$(CLIENT_SRC))
SERVER_OBJ := $(patsubst src/server/%.c,$(TARGET_DIR)/server/%.o,$(SERVER_SRC))
COMMON_OBJ := $(patsubst src/common/%.c,$(TARGET_DIR)/common/%.o,$(COMMON_SRC))

DEPS := $(CLIENT_OBJ:.o=.d) $(SERVER_OBJ:.o=.d) $(COMMON_OBJ:.o=.d)

all: $(TARGET_DIR)/$(CLIENT_EXEC) $(TARGET_DIR)/$(SERVER_EXEC)

$(TARGET_DIR)/$(CLIENT_EXEC): $(COMMON_OBJ) $(CLIENT_OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

$(TARGET_DIR)/$(SERVER_EXEC): $(COMMON_OBJ) $(SERVER_OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

$(TARGET_DIR)/client/%.o: src/client/%.c | $(TARGET_DIR)
	@echo "$@: $<"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@ 

$(TARGET_DIR)/server/%.o: src/server/%.c | $(TARGET_DIR)
	@echo "$@: $<"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(TARGET_DIR)/common/%.o: src/common/%.c | $(TARGET_DIR)
	@echo "$@: $<"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(TARGET_DIR):
	@echo "Target directory does not exist, creating $@..."
	@mkdir -p $(TARGET_DIR)/client $(TARGET_DIR)/server $(TARGET_DIR)/common

-include $(DEPS)

.PHONY: run
run: $(TARGET_DIR)/$(SERVER_EXEC)
	./$<

.PHONY: client
client: $(TARGET_DIR)/$(CLIENT_EXEC)
	./$<

.PHONY: clean
clean:
	@rm -rf $(TARGET_DIR)
	@echo "Target directory cleaned"


