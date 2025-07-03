// src/canvas.c

#include <stdlib.h>
#include <math.h>
#include "canvas.h"
#include <stdio.h>


canvas_t *create_canvas(int width, int height) {
    canvas_t *canvas = malloc(sizeof(canvas_t));
    canvas->width = width;
    canvas->height = height;

    // Allocate pixel buffer
    canvas->pixels = malloc(sizeof(float *) * height);
    for (int y = 0; y < height; y++) {
        canvas->pixels[y] = calloc(width, sizeof(float)); // Initializes to 0.0
    }

    return canvas;
}

void free_canvas(canvas_t *canvas) {
    for (int y = 0; y < canvas->height; y++) {
        free(canvas->pixels[y]);
    }
    free(canvas->pixels);
    free(canvas);
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

        // Stamp a circle of pixels around this center
        for (int dy = -(int)radius; dy <= (int)radius; dy++) {
            for (int dx = -(int)radius; dx <= (int)radius; dx++) {
                float dist = sqrtf(dx * dx + dy * dy);
                if (dist <= radius) {
                    float falloff = 1.0f - (dist / radius); // Soft edge
                    set_pixel_f(canvas, cx + dx, cy + dy, falloff);
                }
            }
        }
    }
}


void set_pixel_f(canvas_t *canvas, float x, float y, float intensity) {
    int xi = (int)floorf(x);
    int yi = (int)floorf(y);

    float xf = x - xi;
    float yf = y - yi;

    // Bilinear weights
    float w00 = (1 - xf) * (1 - yf);
    float w10 = xf * (1 - yf);
    float w01 = (1 - xf) * yf;
    float w11 = xf * yf;

    // Apply to 4 surrounding pixels if they’re inside bounds
    if (xi >= 0 && xi < canvas->width && yi >= 0 && yi < canvas->height)
        canvas->pixels[yi][xi] += intensity * w00;

    if (xi + 1 < canvas->width && yi >= 0 && yi < canvas->height)
        canvas->pixels[yi][xi + 1] += intensity * w10;

    if (xi >= 0 && yi + 1 < canvas->height)
        canvas->pixels[yi + 1][xi] += intensity * w01;

    if (xi + 1 < canvas->width && yi + 1 < canvas->height)
        canvas->pixels[yi + 1][xi + 1] += intensity * w11;
}

#include <stdio.h>

void canvas_to_pgm(canvas_t *canvas, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("fopen");
        return;
    }

    fprintf(fp, "P2\n%d %d\n255\n", canvas->width, canvas->height);

    for (int y = 0; y < canvas->height; y++) {
        for (int x = 0; x < canvas->width; x++) {
            float value = canvas->pixels[y][x];
            int pixel = (int)(255.0f * fminf(value, 1.0f)); // Clamp to [0,1]
            fprintf(fp, "%d ", pixel);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}
