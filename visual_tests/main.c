#include <stdio.h>
#include <math.h>
#include "canvas.h"
#include "math3d.h"
#include "renderer.h"

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

int main() {
    canvas_t canvas;
    canvas_init(&canvas, 512, 512);
    canvas_clear(&canvas, 0.0f);

    mesh_t ball = {
        .vertices = ball_vertices,
        .vertex_count = sizeof(ball_vertices)/sizeof(vec3_t),
        .edges = ball_edges,
        .edge_count = sizeof(ball_edges)/sizeof(ball_edges[0])
    };

    transform_t xf;
    mat4_identity(&xf.model);
    mat4_rotate_xyz(&xf.model, (vec3_t){ 0.7f, 0.4f, 0.2f, 0,0,0 });
    mat4_translate(&xf.model, (vec3_t){ 0.0f, 0.0f, -3.5f, 0,0,0 });

    mat4_identity(&xf.view);
    mat4_frustum_asymmetric(&xf.proj, -1, 1, -1, 1, 1.5f, 10.0f);

    render_wireframe(&canvas, &ball, &xf, 1.0f);
    canvas_write_to_pgm(&canvas, "soccer.pgm");

    printf("Soccer ball wireframe written to soccer.pgm\n");
    canvas_free(&canvas);
    return 0;
}
