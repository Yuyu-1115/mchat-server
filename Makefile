TARGET_EXEC := mchat_server
TARGET_DIR := build

CC = gcc
CFLAGS = -g -Wall -Wextra
LDFLAGS = -lpthread

SRC_FILES := $(wildcard src/*.c)
OBJ_FILES := $(patsubst src/%.c,$(TARGET_DIR)/%.o,$(SRC_FILES))



$(TARGET_DIR)/$(TARGET_EXEC): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $@ 

$(TARGET_DIR)/%.o: src/%.c | $(TARGET_DIR)
	@echo "$@: $<"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@ $(LDFLAGS)

$(TARGET_DIR):
	@echo "Target directory does not exist, creating $@..."
	@mkdir -p $(TARGET_DIR)

-include $(DEPS)

.PHONY: run
run: $(TARGET_DIR)/$(TARGET_EXEC)
	./$<

.PHONY: clean
clean:
	@rm -rf $(TARGET_DIR)
	@echo "Target directory cleaned"


