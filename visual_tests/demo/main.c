#include <stdio.h>
#include <math.h>
#include "canvas.h"
#include "math3d.h"
#include "renderer.h"
#include "lighting.h"

#define WIDTH 512
#define HEIGHT 512

vec3_t bezier(vec3_t p0, vec3_t p1, vec3_t p2, vec3_t p3, float t) {
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    return (vec3_t){
        uuu * p0.x + 3 * uu * t * p1.x + 3 * u * tt * p2.x + ttt * p3.x,
        uuu * p0.y + 3 * uu * t * p1.y + 3 * u * tt * p2.y + ttt * p3.y,
        uuu * p0.z + 3 * uu * t * p1.z + 3 * u * tt * p2.z + ttt * p3.z,
        0, 0, 0
    };
}

vec3_t cube_vertices[] = {
    { -0.5f, -0.5f, -0.5f, 0,0,0 }, {  0.5f, -0.5f, -0.5f, 0,0,0 },
    {  0.5f,  0.5f, -0.5f, 0,0,0 }, { -0.5f,  0.5f, -0.5f, 0,0,0 },
    { -0.5f, -0.5f,  0.5f, 0,0,0 }, {  0.5f, -0.5f,  0.5f, 0,0,0 },
    {  0.5f,  0.5f,  0.5f, 0,0,0 }, { -0.5f,  0.5f,  0.5f, 0,0,0 }
};

int cube_edges[][2] = {
    {0,1},{1,2},{2,3},{3,0},
    {4,5},{5,6},{6,7},{7,4},
    {0,4},{1,5},{2,6},{3,7}
};

int main() {
    canvas_t canvas;
    canvas_init(&canvas, WIDTH, HEIGHT);

    mesh_t cube = {
        .vertices = cube_vertices,
        .vertex_count = 8,
        .edges = cube_edges,
        .edge_count = 12
    };

    vec3_t light_dir = { -1, -1, -1, 0, 0, 0 };
    vec3_t p0 = { -1.5f, 0, -3.5f, 0,0,0 };
    vec3_t p1 = { -1.5f, 1, -4.5f, 0,0,0 };
    vec3_t p2 = {  1.5f, 1, -4.5f, 0,0,0 };
    vec3_t p3 = {  1.5f, 0, -3.5f, 0,0,0 };

    for (int frame = 0; frame < 60; frame++) {
        float t = frame / 60.0f;
        vec3_t pos = bezier(p0, p1, p2, p3, t);

        canvas_clear(&canvas, 0.0f);

        transform_t tf;
        mat4_identity(&tf.model);
        mat4_rotate_xyz(&tf.model, (vec3_t){ t * 6.28f, t * 3.14f, 0, 0,0,0 });
        mat4_translate(&tf.model, pos);

        mat4_identity(&tf.view);
        mat4_frustum_asymmetric(&tf.proj, -1, 1, -1, 1, 1.2f, 10.0f);

        for (int i = 0; i < cube.edge_count; i++) {
            vec3_t a = cube.vertices[cube.edges[i][0]];
            vec3_t b = cube.vertices[cube.edges[i][1]];
            vec3_t dir = { b.x - a.x, b.y - a.y, b.z - a.z, 0, 0, 0 };
            float intensity = lambert_intensity(dir, light_dir);

            vec3_t pa, pb;
            project_vertex(a, &pa, &tf);
            project_vertex(b, &pb, &tf);

            int x0 = (int)((pa.x * 0.5f + 0.5f) * WIDTH);
            int y0 = (int)((1.0f - (pa.y * 0.5f + 0.5f)) * HEIGHT);
            int x1 = (int)((pb.x * 0.5f + 0.5f) * WIDTH);
            int y1 = (int)((1.0f - (pb.y * 0.5f + 0.5f)) * HEIGHT);

            if (clip_to_circular_viewport(&canvas, x0, y0) || clip_to_circular_viewport(&canvas, x1, y1)) {
                draw_line_f(&canvas, x0, y0, x1, y1, intensity);
            }
        }

        char fname[64];
        sprintf(fname, "frame_%02d.pgm", frame);
        canvas_write_to_pgm(&canvas, fname);
    }

    canvas_free(&canvas);
    printf("✅ All 60 frames generated\n");
    return 0;
}
