#include <math.h>
#include "canvas.h"
#include "math3d.h"
#include "renderer.h"

#define WIDTH 512
#define HEIGHT 512

int main() {
    canvas_t *canvas = create_canvas(WIDTH, HEIGHT);

    // Define cube vertices
    vec3_t vertices[8] = {
        vec3_init(-0.5, -0.5, -0.5), vec3_init(0.5, -0.5, -0.5),
        vec3_init(0.5,  0.5, -0.5), vec3_init(-0.5, 0.5, -0.5),
        vec3_init(-0.5, -0.5,  0.5), vec3_init(0.5, -0.5,  0.5),
        vec3_init(0.5,  0.5,  0.5), vec3_init(-0.5, 0.5,  0.5)
    };

    // Define edges (pairs of vertex indices)
    int edges[12][2] = {
        {0,1}, {1,2}, {2,3}, {3,0}, // back face
        {4,5}, {5,6}, {6,7}, {7,4}, // front face
        {0,4}, {1,5}, {2,6}, {3,7}  // connecting lines
    };

    // Transformation matrices
    mat4_t model, view, proj;
    mat4_identity(&view);
    mat4_translate(&model, vec3_init(0, 0, -3));
    mat4_frustum_asymmetric(&proj, -1, 1, -1, 1, 1, 10);

    render_wireframe(canvas, vertices, edges, 8, 12, &model, &view, &proj);
    canvas_to_pgm(canvas, "wireframe.pgm");

    free_canvas(canvas);
    return 0;
}
