#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "canvas.h"
#include "math3d.h"
#include "renderer.h"

#define WIDTH 512
#define HEIGHT 512
#define FRAME_COUNT 60

int main() {
    for (int frame = 0; frame < FRAME_COUNT; frame++) {
        canvas_t *canvas = create_canvas(WIDTH, HEIGHT);

        vec3_t vertices[8] = {
            vec3_init(-0.5, -0.5, -0.5), vec3_init(0.5, -0.5, -0.5),
            vec3_init(0.5,  0.5, -0.5), vec3_init(-0.5, 0.5, -0.5),
            vec3_init(-0.5, -0.5,  0.5), vec3_init(0.5, -0.5,  0.5),
            vec3_init(0.5,  0.5,  0.5), vec3_init(-0.5, 0.5,  0.5)
        };

        int edges[12][2] = {
            {0,1}, {1,2}, {2,3}, {3,0},
            {4,5}, {5,6}, {6,7}, {7,4},
            {0,4}, {1,5}, {2,6}, {3,7}
        };

        float angle = frame * 2 * M_PI / FRAME_COUNT;

        mat4_t model, view, proj, rotation, final_model;

        mat4_rotate_xyz(&rotation, vec3_init(angle, angle, 0));
        mat4_translate(&model, vec3_init(0, 0, -3));

        mat4_multiply(&final_model, &model, &rotation);

        mat4_identity(&view);
        mat4_frustum_asymmetric(&proj, -1, 1, -1, 1, 1, 10);

        render_wireframe(canvas, vertices, edges, 8, 12, &final_model, &view, &proj);

        char filename[64];
        sprintf(filename, "build/frame_%03d.pgm", frame);
        canvas_to_pgm(canvas, filename);

        free_canvas(canvas);
        printf("✅ Rendered %s\n", filename);
    }

    return 0;
}
