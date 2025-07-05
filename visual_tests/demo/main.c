#include <stdio.h>
#include <math.h>
#include "canvas.h"
#include "math3d.h"
#include "renderer.h"
#include "lighting.h"

#define WIDTH 512
#define HEIGHT 512
#define FRAME_COUNT 120

// Bézier curve
vec3_t bezier(vec3_t p0, vec3_t p1, vec3_t p2, vec3_t p3, float t) {
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;
    return (vec3_t){
        uuu*p0.x + 3*uu*t*p1.x + 3*u*tt*p2.x + ttt*p3.x,
        uuu*p0.y + 3*uu*t*p1.y + 3*u*tt*p2.y + ttt*p3.y,
        uuu*p0.z + 3*uu*t*p1.z + 3*u*tt*p2.z + ttt*p3.z,
        0,0,0
    };
}

// Soccer ball mesh
vec3_t ball_vertices[] = {
    {  0.0f, 0.0f, -1.0f, 0,0,0 },
    {  0.894f, 0.0f, -0.447f, 0,0,0 },
    {  0.276f, 0.851f, -0.447f, 0,0,0 },
    { -0.724f, 0.526f, -0.447f, 0,0,0 },
    { -0.724f,-0.526f, -0.447f, 0,0,0 },
    {  0.276f,-0.851f, -0.447f, 0,0,0 },
    {  0.724f, 0.526f,  0.447f, 0,0,0 },
    { -0.276f, 0.851f,  0.447f, 0,0,0 },
    { -0.894f, 0.0f,    0.447f, 0,0,0 },
    { -0.276f,-0.851f,  0.447f, 0,0,0 },
    {  0.724f,-0.526f,  0.447f, 0,0,0 },
    {  0.0f,   0.0f,    1.0f,   0,0,0 }
};

int ball_edges[][2] = {
    {0,1},{0,2},{0,3},{0,4},{0,5},
    {1,2},{2,3},{3,4},{4,5},{5,1},
    {1,6},{2,6},{2,7},{3,7},{3,8},{4,8},{4,9},{5,9},{5,10},{1,10},
    {6,7},{7,8},{8,9},{9,10},{10,6},
    {6,11},{7,11},{8,11},{9,11},{10,11}
};

// Cube mesh
vec3_t cube_vertices[] = {
    {-0.5f,-0.5f,-0.5f,0,0,0}, {0.5f,-0.5f,-0.5f,0,0,0},
    {0.5f, 0.5f,-0.5f,0,0,0}, {-0.5f, 0.5f,-0.5f,0,0,0},
    {-0.5f,-0.5f, 0.5f,0,0,0}, {0.5f,-0.5f, 0.5f,0,0,0},
    {0.5f, 0.5f, 0.5f,0,0,0}, {-0.5f, 0.5f, 0.5f,0,0,0}
};

int cube_edges[][2] = {
    {0,1},{1,2},{2,3},{3,0},
    {4,5},{5,6},{6,7},{7,4},
    {0,4},{1,5},{2,6},{3,7},
    {0,6},{1,7},{2,4},{3,5}
};

int main() {
    canvas_t canvas;
    canvas_init(&canvas, WIDTH, HEIGHT);

    mesh_t cube = {
        .vertices = cube_vertices,
        .vertex_count = 8,
        .edges = cube_edges,
        .edge_count = sizeof(cube_edges) / sizeof(cube_edges[0])
    };

    mesh_t ball = {
        .vertices = ball_vertices,
        .vertex_count = sizeof(ball_vertices)/sizeof(vec3_t),
        .edges = ball_edges,
        .edge_count = sizeof(ball_edges)/sizeof(ball_edges[0])
    };

    // Bézier path for cube
    vec3_t p0 = {-1.5f, 0.0f, -4.0f, 0,0,0};
    vec3_t p1 = {-1.5f, 1.0f, -4.5f, 0,0,0};
    vec3_t p2 = { 1.5f, 1.0f, -4.5f, 0,0,0};
    vec3_t p3 = { 1.5f, 0.0f, -4.0f, 0,0,0};

    for (int frame = 0; frame < FRAME_COUNT; frame++) {
        float t = frame / (float)FRAME_COUNT;
        canvas_clear(&canvas, 0.0f);

        // Moving light sources
        vec3_t light1 = vec3_from_spherical(1.0f, 1.5f, t * 2 * M_PI);
        vec3_t light2 = vec3_from_spherical(1.0f, 1.2f, t * 4 * M_PI);

        // Shared projection + view
        mat4_t view;
        mat4_identity(&view);
        mat4_t proj;
        mat4_frustum_asymmetric(&proj, -1, 1, -1, 1, 1.2f, 10.0f);

        // === Cube ===
        transform_t tf_cube;
        mat4_identity(&tf_cube.model);
        mat4_rotate_xyz(&tf_cube.model, (vec3_t){ t * 2*M_PI, t*M_PI, t*0.5f*M_PI, 0,0,0 });
        mat4_translate(&tf_cube.model, bezier(p0, p1, p2, p3, t));
        tf_cube.view = view;
        tf_cube.proj = proj;

        for (int i = 0; i < cube.edge_count; i++) {
            vec3_t a = cube.vertices[cube.edges[i][0]];
            vec3_t b = cube.vertices[cube.edges[i][1]];
            vec3_t dir = { b.x - a.x, b.y - a.y, b.z - a.z, 0,0,0 };
            float i1 = lambert_intensity(dir, light1);
            float i2 = lambert_intensity(dir, light2);
            float intensity = fminf(1.0f, (i1 + i2) / 1.5f);
            vec3_t pa, pb;
            project_vertex(a, &pa, &tf_cube);
            project_vertex(b, &pb, &tf_cube);

            int x0 = (int)((pa.x * 0.5f + 0.5f) * WIDTH);
            int y0 = (int)((1.0f - (pa.y * 0.5f + 0.5f)) * HEIGHT);
            int x1 = (int)((pb.x * 0.5f + 0.5f) * WIDTH);
            int y1 = (int)((1.0f - (pb.y * 0.5f + 0.5f)) * HEIGHT);

            if (clip_to_circular_viewport(&canvas, x0, y0) || clip_to_circular_viewport(&canvas, x1, y1))
                draw_line_f(&canvas, x0, y0, x1, y1, intensity * 1.5f);
        }

        // === Ball ===
        transform_t tf_ball;
        mat4_identity(&tf_ball.model);
        mat4_rotate_xyz(&tf_ball.model, (vec3_t){
            t * 1.2f * 2*M_PI, t * 0.7f * 2*M_PI, t * 0.5f * 2*M_PI,
            0,0,0
        });
        
        mat4_translate(&tf_ball.model, (vec3_t){ 0.0f, 0.0f, -3.5f, 0,0,0 });
        tf_ball.view = view;
        tf_ball.proj = proj;

        for (int i = 0; i < ball.edge_count; i++) {
            vec3_t a = ball.vertices[ball.edges[i][0]];
            vec3_t b = ball.vertices[ball.edges[i][1]];
            vec3_t dir = { b.x - a.x, b.y - a.y, b.z - a.z, 0,0,0 };

            float i1 = lambert_intensity(dir, light1);
            float i2 = lambert_intensity(dir, light2);
            float intensity = fminf(1.0f, (i1 + i2) / 1.5f);

            vec3_t pa, pb;
            project_vertex(a, &pa, &tf_ball);
            project_vertex(b, &pb, &tf_ball);

            int x0 = (int)((pa.x * 0.5f + 0.5f) * WIDTH);
            int y0 = (int)((1.0f - (pa.y * 0.5f + 0.5f)) * HEIGHT);
            int x1 = (int)((pb.x * 0.5f + 0.5f) * WIDTH);
            int y1 = (int)((1.0f - (pb.y * 0.5f + 0.5f)) * HEIGHT);

            if (clip_to_circular_viewport(&canvas, x0, y0) || clip_to_circular_viewport(&canvas, x1, y1)) {
                draw_line_f(&canvas, x0, y0, x1, y1, intensity * 1.5f);
            }
        }

        // === Save this frame ===
        char fname[64];
        sprintf(fname, "build/frame_%03d.pgm", frame);
        canvas_write_to_pgm(&canvas, fname);
    }

    canvas_free(&canvas);
    printf("✅ %d frames rendered (animated cube + spinning ball with lighting)\n", FRAME_COUNT);
    return 0;
}
