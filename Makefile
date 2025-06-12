CC := gcc

SRC_DIR := src
BUILD_DIR := build

SRC := $(shell find $(SRC_DIR) -name '*.c')
HEADERS := $(shell find $(SRC_DIR) -name '*.h')
OBJ := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

TARGET := $(BUILD_DIR)/main

CFLAGS := -std=c17 -Wall -lm

all: $(TARGET)
	@echo "Build complete 😎"

-include $(OBJ:.o=.d)

$(TARGET): $(OBJ) | $(BUILD_DIR)
	$(CC) $(OBJ) $(CFLAGS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.d: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MM -MT $(@:.d=.o) $< > $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
