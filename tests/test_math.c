#include <stdio.h>
#include "math3d.h"

#define WIDTH 640
#define HEIGHT 480

int main() {
    vec3_t cube[8] = {
        vec3_init(-0.5, -0.5, -0.5), vec3_init(0.5, -0.5, -0.5),
        vec3_init(0.5, 0.5, -0.5),   vec3_init(-0.5, 0.5, -0.5),
        vec3_init(-0.5, -0.5, 0.5),  vec3_init(0.5, -0.5, 0.5),
        vec3_init(0.5, 0.5, 0.5),    vec3_init(-0.5, 0.5, 0.5)
    };

    mat4_t model, proj;
    mat4_translate(&model, vec3_init(0, 0, -3));
    mat4_frustum_asymmetric(&proj, -1, 1, -0.75f, 0.75f, 1, 10);

    printf("=== Cube Projection ===\n");
    for (int i = 0; i < 8; i++) {
        vec3_t world = vec3_project(cube[i], &model);
        vec3_t ndc   = vec3_project(world, &proj);
        int sx = (int)((ndc.x * 0.5f + 0.5f) * WIDTH);
        int sy = (int)((1.0f - (ndc.y * 0.5f + 0.5f)) * HEIGHT);
        printf("Vertex %d → Screen: (%d, %d)\n", i, sx, sy);
    }

    return 0;
}
