#include "canvas.h"
#include <math.h>

int main() {
    canvas_t *canvas = create_canvas(256, 256);
    float cx = canvas->width / 2.0f;
    float cy = canvas->height / 2.0f;

    for (int i = 0; i < 24; i++) {
        float angle = i * M_PI / 12.0f;
        float x = cx + 100 * cosf(angle);
        float y = cy + 100 * sinf(angle);
        draw_line_f(canvas, cx, cy, x, y, 2.0f);
    }

    canvas_to_pgm(canvas, "output.pgm");
    free_canvas(canvas);
    return 0;
}
