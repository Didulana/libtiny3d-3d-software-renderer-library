CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude

SRC_DIR = src
BUILD_DIR = build

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

LIB = $(BUILD_DIR)/libtiny3d.a

# Demo files
DEMO_SRC = visual_tests/demo/main.c
DEMO_OBJ = $(BUILD_DIR)/demo_main.o
DEMO_EXE = $(BUILD_DIR)/demo

.PHONY: all clean run

all: $(DEMO_EXE)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Compile object files from src/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Archive static library
$(LIB): $(OBJ_FILES)
	ar rcs $@ $^

# Compile demo executable
$(DEMO_EXE): $(DEMO_SRC) $(LIB) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(DEMO_SRC) -o $(DEMO_OBJ)
	$(CC) $(CFLAGS) $(DEMO_OBJ) $(LIB) -lm -o $@

# Run demo
run: $(DEMO_EXE)
	./$(DEMO_EXE)

# Clean all build outputs
clean:
	rm -rf $(BUILD_DIR)
