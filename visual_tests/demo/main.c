#include <stdio.h>
#include <stdlib.h>
#include "canvas.h"
#include <math.h>

void print_canvas(canvas_t *canvas) {
    for (int y = 0; y < canvas->height; y++) {
        for (int x = 0; x < canvas->width; x++) {
            float val = canvas->pixels[y][x];
            char symbol = ' ';

            if (val > 0.7) symbol = '#';
            else if (val > 0.4) symbol = '+';
            else if (val > 0.1) symbol = '.';

            printf("%c", symbol);
        }
        printf("\n");
    }
}

int main() {
    canvas_t *canvas = create_canvas(256, 256);

    float center_x = canvas->width / 2.0f;
    float center_y = canvas->height / 2.0f;    

    for (int i = 0; i < 24; i++) {
        float angle = i * M_PI / 12.0f;
        float x = center_x + 100 * cosf(angle);
        float y = center_y + 100 * sinf(angle);
        draw_line_f(canvas, center_x, center_y, x, y, 2.5f);
    }


    canvas_to_pgm(canvas, "output.pgm");


    free_canvas(canvas);
    return 0;
}
