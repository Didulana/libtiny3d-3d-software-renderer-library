CC = gcc
CFLAGS = -Wall -Iinclude -lm

all: demo

demo: visual_tests/demo/main.c src/canvas.c
	$(CC) $(CFLAGS) $^ -o build/demo

run:
	./build/demo

clean:
	rm -rf build/* output.pgm
