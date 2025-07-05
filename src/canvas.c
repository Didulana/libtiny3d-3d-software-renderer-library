#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "canvas.h"

void canvas_init(canvas_t* canvas, int width, int height) {
    canvas->width = width;
    canvas->height = height;
    canvas->pixels = (float*)calloc(width * height, sizeof(float));
}

void canvas_clear(canvas_t* canvas, float value) {
    int total = canvas->width * canvas->height;
    for (int i = 0; i < total; i++) {
        canvas->pixels[i] = value;
    }
}

void set_pixel_f(canvas_t* canvas, float x, float y, float intensity) {
    if (intensity <= 0.0f) return;

    int xi = (int)floorf(x);
    int yi = (int)floorf(y);

    float fx = x - xi;
    float fy = y - yi;

    float w00 = (1 - fx) * (1 - fy);
    float w10 = fx * (1 - fy);
    float w01 = (1 - fx) * fy;
    float w11 = fx * fy;

    int w = canvas->width;
    int h = canvas->height;

    if (xi >= 0 && yi >= 0 && xi < w && yi < h)
        canvas->pixels[yi * w + xi] += intensity * w00;
    if (xi + 1 >= 0 && yi >= 0 && xi + 1 < w && yi < h)
        canvas->pixels[yi * w + (xi + 1)] += intensity * w10;
    if (xi >= 0 && yi + 1 >= 0 && xi < w && yi + 1 < h)
        canvas->pixels[(yi + 1) * w + xi] += intensity * w01;
    if (xi + 1 >= 0 && yi + 1 >= 0 && xi + 1 < w && yi + 1 < h)
        canvas->pixels[(yi + 1) * w + (xi + 1)] += intensity * w11;
}

void draw_line_f(canvas_t* canvas, float x0, float y0, float x1, float y1, float thickness) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    float length = hypotf(dx, dy);
    if (length == 0.0f) return;

    float step = 1.0f / length;
    float nx = -dy / length;
    float ny = dx / length;

    for (float t = 0.0f; t <= 1.0f; t += step) {
        float cx = x0 + t * dx;
        float cy = y0 + t * dy;
        for (float d = -thickness / 2.0f; d <= thickness / 2.0f; d += 0.25f) {
            float px = cx + nx * d;
            float py = cy + ny * d;
            float falloff = 1.0f - fabsf(d) / (thickness / 2.0f);
            set_pixel_f(canvas, px, py, falloff);
        }
    }
}

void canvas_write_to_pgm(canvas_t* canvas, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) return;

    fprintf(f, "P2\n%d %d\n255\n", canvas->width, canvas->height);
    for (int y = 0; y < canvas->height; y++) {
        for (int x = 0; x < canvas->width; x++) {
            float v = canvas->pixels[y * canvas->width + x];
            int pixel = (int)(fminf(fmaxf(v, 0.0f), 1.0f) * 255);
            fprintf(f, "%d ", pixel);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

void canvas_free(canvas_t* canvas) {
    free(canvas->pixels);
}
