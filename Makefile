# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

# Directories
SRC_DIR = src
OBJ_DIR = build
DEMO_DIR = visual_tests/demo

# Files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
TARGET = $(OBJ_DIR)/libtiny3d.a
DEMO = $(OBJ_DIR)/demo

# Default target
all: $(TARGET) $(DEMO)

# Static library
$(TARGET): $(OBJS)
	ar rcs $@ $^

# Object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Demo build
$(DEMO): $(DEMO_DIR)/main.c $(TARGET)
	$(CC) $(CFLAGS) $^ -o $@ -lm

# Build test_math from tests/test_math.c and math3d.c
test_math: tests/test_math.c src/math3d.c
	$(CC) $(CFLAGS) -Iinclude $^ -o $@ -lm

# Run demo
run: $(DEMO)
	./$(DEMO)

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR)

.PHONY: all clean run
