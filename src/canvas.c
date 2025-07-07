#include "canvas.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

canvas_t* create_canvas(int width, int height) {
    canvas_t* c = malloc(sizeof(canvas_t));
    c->width = width;
    c->height = height;
    c->data = calloc(width * height, sizeof(float));
    return c;
}

void free_canvas(canvas_t* c) {
    free(c->data);
    free(c);
}

void set_pixel_f(canvas_t* c, float x, float y, float intensity) {
    int ix = (int)floorf(x), iy = (int)floorf(y);
    float fx = x - ix, fy = y - iy;

    for (int dx = 0; dx <= 1; ++dx)
        for (int dy = 0; dy <= 1; ++dy) {
            int px = ix + dx, py = iy + dy;
            if (px >= 0 && px < c->width && py >= 0 && py < c->height) {
                float w = (dx ? fx : 1 - fx) * (dy ? fy : 1 - fy);
                float *cell = &c->data[py * c->width + px];
                *cell += w * intensity;
                if (*cell > 1.0f) *cell = 1.0f;
            }
        }
}

void draw_line_f(canvas_t* c, float x0, float y0, float x1, float y1, float thickness) {
    float dx = x1 - x0, dy = y1 - y0;
    float len = sqrtf(dx * dx + dy * dy);
    int steps = (int)(len * 2.0f);

    for (int i = 0; i <= steps; ++i) {
        float t = (float)i / steps;
        float x = x0 + t * dx;
        float y = y0 + t * dy;

        for (float tx = -thickness / 2; tx <= thickness / 2; tx += 0.5f)
            for (float ty = -thickness / 2; ty <= thickness / 2; ty += 0.5f)
                set_pixel_f(c, x + tx, y + ty, 1.0f);
    }
}

void canvas_to_pgm(canvas_t* c, const char* filename) {
    FILE* f = fopen(filename, "w");
    fprintf(f, "P2\n%d %d\n255\n", c->width, c->height);
    for (int y = 0; y < c->height; ++y) {
        for (int x = 0; x < c->width; ++x) {
            int val = (int)(c->data[y * c->width + x] * 255.0f);
            if (val > 255) val = 255;
            fprintf(f, "%d ", val);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}
