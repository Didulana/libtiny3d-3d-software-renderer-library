CC = gcc
CFLAGS = -Wall -Iinclude -lm

all: demo

demo: visual_tests/demo/main.c src/canvas.c
	$(CC) $(CFLAGS) $^ -o build/demo

test_math: tests/test_math.c src/math3d.c
	$(CC) $(CFLAGS) -Iinclude $^ -o build/test_math -lm

demo: visual_tests/demo/main.c src/canvas.c src/math3d.c src/renderer.c
	$(CC) $(CFLAGS) -Iinclude $^ -o build/demo -lm

run:
	./build/demo

clean:
	rm -rf build/* output.pgm
