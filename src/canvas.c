#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "canvas.h"
#include <math.h>

canvas_t* canvas_create(int width, int height) {
    canvas_t* c = malloc(sizeof(canvas_t));
    if (!c) return NULL;
    c->width = width;
    c->height = height;
    c->data = calloc(width * height, sizeof(float));
    return c;
}

void canvas_clear(canvas_t* c, float value) {
    for (int i = 0; i < c->width * c->height; ++i) {
        c->data[i] = value;
    }
}

void canvas_save(canvas_t* c, int frame) {
    char filename[64];
    snprintf(filename, sizeof(filename), "frame_%03d.pgm", frame);
    canvas_to_pgm(c, filename);
}

void canvas_destroy(canvas_t* c) {
    if (c) {
        free(c->data);
        free(c);
    }
}

void set_pixel_f(canvas_t* c, float x, float y, float intensity) {
    int xi = (int)x;
    int yi = (int)y;
    if (xi >= 0 && xi < c->width && yi >= 0 && yi < c->height) {
        int idx = yi * c->width + xi;
        if (intensity > c->data[idx]) {
            c->data[idx] = intensity;
        }
    }
}

void draw_line_f(canvas_t* c, float x0, float y0, float x1, float y1, float intensity) {
    int steps = (int)(fmaxf(fabsf(x1 - x0), fabsf(y1 - y0))) * 2;
    for (int i = 0; i <= steps; ++i) {
        float t = (float)i / steps;
        float x = x0 + (x1 - x0) * t;
        float y = y0 + (y1 - y0) * t;
        set_pixel_f(c, x, y, intensity);
    }
}

void canvas_to_pgm(canvas_t* c, const char* filename) {
    FILE* f = fopen(filename, "w");
    fprintf(f, "P2\n%d %d\n255\n", c->width, c->height);
    for (int y = 0; y < c->height; ++y) {
        for (int x = 0; x < c->width; ++x) {
            float val = c->data[y * c->width + x];
            int pixel = (int)(255.0f * fminf(fmaxf(val, 0.0f), 1.0f));
            fprintf(f, "%d ", pixel);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}
