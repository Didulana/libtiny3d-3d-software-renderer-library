# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

# Directories
SRC_DIR = src
OBJ_DIR = build
TEST_DIR = tests
DEMO_DIR = visual_tests/demo

# Files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
TARGET = $(OBJ_DIR)/libtiny3d.a
DEMO = $(OBJ_DIR)/demo
DEMO_LIGHT = $(OBJ_DIR)/demo_lighting

# Default target
all: $(TARGET) $(DEMO) $(DEMO_LIGHT)

# Build static library
$(TARGET): $(OBJS)
	@mkdir -p $(OBJ_DIR)
	ar rcs $@ $^

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Basic wireframe demo (visual_tests/main.c)
$(DEMO): visual_tests/main.c $(TARGET)
	$(CC) $(CFLAGS) $^ -o $@ -lm

# Animated lighting demo (visual_tests/demo/main.c)
$(DEMO_LIGHT): $(DEMO_DIR)/main.c $(TARGET)
	$(CC) $(CFLAGS) $^ -o $@ -lm

# Test: projection math
test_math: $(TEST_DIR)/test_math.c $(SRC_DIR)/math3d.c
	$(CC) $(CFLAGS) $^ -o $@ -lm

# Test: rasterizer pipeline
test_pipeline: $(TEST_DIR)/test_pipeline.c $(SRC_DIR)/math3d.c
	$(CC) $(CFLAGS) $^ -o $@ -lm

# Run default demo
run: $(DEMO)
	./$(DEMO)

demo_lighting: $(DEMO_LIGHT)

# Clean everything
clean:
	rm -rf $(OBJ_DIR) *.o demo test_math test_pipeline triangle.pgm soccer.pgm build/*.pgm build/demo_lighting build/demo
