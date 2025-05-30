CC = gcc
CFLAGS = -Wall -Wextra -O3 -fopenmp -march=native -ffast-math -fno-trapping-math $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs) -fopenmp

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))
OUT = $(BIN_DIR)/fractal.out

# Default target
all: $(OUT)

# Link step
$(OUT): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compile step
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: clean all

