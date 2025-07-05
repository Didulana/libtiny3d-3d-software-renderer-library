#include <stdio.h>
#include <math.h>
#include "math3d.h"

#define WIDTH 640
#define HEIGHT 480

int main() {
    // Step 1: Define a unit cube centered at the origin
    vec3_t cube[8] = {
        vec3_init(-0.5f, -0.5f, -0.5f),
        vec3_init( 0.5f, -0.5f, -0.5f),
        vec3_init( 0.5f,  0.5f, -0.5f),
        vec3_init(-0.5f,  0.5f, -0.5f),
        vec3_init(-0.5f, -0.5f,  0.5f),
        vec3_init( 0.5f, -0.5f,  0.5f),
        vec3_init( 0.5f,  0.5f,  0.5f),
        vec3_init(-0.5f,  0.5f,  0.5f)
    };

    // Step 2: Model transform → move the cube forward in Z
    mat4_t model, proj;
    mat4_translate(&model, vec3_init(0.0f, 0.0f, -3.0f)); // move cube away from camera

    // Step 3: Perspective projection
    mat4_frustum_asymmetric(&proj, -1, 1, -0.75f, 0.75f, 1, 10);

    // Step 4: Project each vertex and print result
    printf("=== Cube Projection Demo ===\n");
    for (int i = 0; i < 8; i++) {
        // Apply model transform
        vec3_t world = vec3_project(cube[i], &model);

        // Apply projection
        vec3_t ndc = vec3_project(world, &proj);

        // Convert NDC to screen coordinates
        int sx = (int)((ndc.x * 0.5f + 0.5f) * WIDTH);
        int sy = (int)((1.0f - (ndc.y * 0.5f + 0.5f)) * HEIGHT);

        printf("Vertex %d → Screen: (%d, %d)\n", i, sx, sy);
    }

    return 0;
}
