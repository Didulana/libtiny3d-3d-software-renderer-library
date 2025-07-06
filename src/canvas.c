#include "canvas.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

canvas_t *create_canvas(int width, int height) {
    canvas_t *canvas = malloc(sizeof(canvas_t));
    canvas->width = width;
    canvas->height = height;
    canvas->pixels = malloc(height * sizeof(float *));
    for (int y = 0; y < height; y++)
        canvas->pixels[y] = calloc(width, sizeof(float));
    return canvas;
}

void free_canvas(canvas_t *canvas) {
    for (int y = 0; y < canvas->height; y++)
        free(canvas->pixels[y]);
    free(canvas->pixels);
    free(canvas);
}

void set_pixel_f(canvas_t *canvas, float x, float y, float intensity) {
    int xi = (int)floorf(x);
    int yi = (int)floorf(y);
    float xf = x - xi;
    float yf = y - yi;

    float w00 = (1 - xf) * (1 - yf);
    float w10 = xf * (1 - yf);
    float w01 = (1 - xf) * yf;
    float w11 = xf * yf;

    if (xi >= 0 && yi >= 0 && xi < canvas->width && yi < canvas->height)
        canvas->pixels[yi][xi] += intensity * w00;
    if (xi+1 < canvas->width && yi >= 0 && yi < canvas->height)
        canvas->pixels[yi][xi+1] += intensity * w10;
    if (xi >= 0 && yi+1 < canvas->height)
        canvas->pixels[yi+1][xi] += intensity * w01;
    if (xi+1 < canvas->width && yi+1 < canvas->height)
        canvas->pixels[yi+1][xi+1] += intensity * w11;
}

void draw_line_f(canvas_t *canvas, float x0, float y0, float x1, float y1, float thickness) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    float length = fmaxf(fabsf(dx), fabsf(dy));
    if (length == 0.0f) return;

    float step_x = dx / length;
    float step_y = dy / length;
    float radius = thickness / 2.0f;

    for (float t = 0; t <= length; t += 1.0f) {
        float cx = x0 + step_x * t;
        float cy = y0 + step_y * t;

        for (int dy = -(int)radius; dy <= (int)radius; dy++) {
            for (int dx = -(int)radius; dx <= (int)radius; dx++) {
                float dist = sqrtf(dx * dx + dy * dy);
                if (dist <= radius) {
                    float falloff = 1.0f - (dist / radius);
                    set_pixel_f(canvas, cx + dx, cy + dy, falloff);
                }
            }
        }
    }
}

void canvas_to_pgm(canvas_t *canvas, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("fopen");
        return;
    }

    fprintf(fp, "P2\n%d %d\n255\n", canvas->width, canvas->height);
    for (int y = 0; y < canvas->height; y++) {
        for (int x = 0; x < canvas->width; x++) {
            int pixel = (int)(255.0f * fminf(canvas->pixels[y][x], 1.0f));
            fprintf(fp, "%d ", pixel);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}
