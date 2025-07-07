#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "canvas.h"
#include "math3d.h"
#include "renderer.h"
#include "lighting.h"
#include "soccerball.h"
#include "animation.h"

#define WIDTH 512
#define HEIGHT 512
#define FRAME_COUNT 120

// Cube primitive inline
#define CUBE_VERTEX_COUNT 8
#define CUBE_EDGE_COUNT 12
void generate_cube(vec3_t* vertices, int edges[][2]) {
    float s = 0.5f;
    vertices[0] = vec3_init(-s, -s, -s);
    vertices[1] = vec3_init( s, -s, -s);
    vertices[2] = vec3_init( s,  s, -s);
    vertices[3] = vec3_init(-s,  s, -s);
    vertices[4] = vec3_init(-s, -s,  s);
    vertices[5] = vec3_init( s, -s,  s);
    vertices[6] = vec3_init( s,  s,  s);
    vertices[7] = vec3_init(-s,  s,  s);

    int e[][2] = {
        {0,1}, {1,2}, {2,3}, {3,0},
        {4,5}, {5,6}, {6,7}, {7,4},
        {0,4}, {1,5}, {2,6}, {3,7}
    };
    for (int i = 0; i < CUBE_EDGE_COUNT; ++i) {
        edges[i][0] = e[i][0];
        edges[i][1] = e[i][1];
    }
}

int main() {
    vec3_t soccer_vertices[SOCCERBALL_VERTEX_COUNT];
    int soccer_edges[SOCCERBALL_EDGE_MAX][2];
    int soccer_edge_count = 0;
    generate_soccerball(soccer_vertices, soccer_edges, &soccer_edge_count);

    vec3_t cube_vertices[CUBE_VERTEX_COUNT];
    int cube_edges[CUBE_EDGE_COUNT][2];
    generate_cube(cube_vertices, cube_edges);

    float aspect = (float)WIDTH / HEIGHT;
    mat4_t view, proj;
    mat4_identity(&view);
    mat4_frustum_asymmetric(&proj, -aspect, aspect, -1, 1, 1, 10);

    vec3_t lights[2] = {
        vec3_normalize_fast(vec3_init(1, 1.5f, -1)),
        vec3_normalize_fast(vec3_init(-1, 1.0f, -1))
    };

    vec3_t p0 = vec3_init(-2, 0, -5), p1 = vec3_init(-2, 2, -5);
    vec3_t p2 = vec3_init(2, 2, -5),  p3 = vec3_init(-2, 0, -5);
    vec3_t q0 = vec3_init(2, 0, -5),  q1 = vec3_init(2, -2, -5);
    vec3_t q2 = vec3_init(-2, -2, -5), q3 = vec3_init(2, 0, -5);

    quat_t q_start = quat_from_axis_angle(vec3_init(1, 1, 0), 0.0f);
    quat_t q_end   = quat_from_axis_angle(vec3_init(1, 1, 0), 2.0f * M_PI);

    for (int frame = 0; frame < FRAME_COUNT; ++frame) {
        canvas_t* canvas = create_canvas(WIDTH, HEIGHT);

        float raw_t = (float)frame / (FRAME_COUNT - 1);
        float t = raw_t * raw_t * (3.0f - 2.0f * raw_t); // Smoothstep

        quat_t q_rot = quat_slerp(q_start, q_end, t);
        mat4_t rot;
        quat_to_mat4(&rot, q_rot);

        // Soccerball
        mat4_t model1, trans1;
        mat4_translate(&trans1, bezier(p0, p1, p2, p3, t));
        mat4_multiply(&model1, &trans1, &rot);
        render_wireframe(canvas, soccer_vertices, soccer_edges,
                         SOCCERBALL_VERTEX_COUNT, soccer_edge_count,
                         &model1, &view, &proj, lights[0]);

        // Cube
        mat4_t model2, trans2;
        mat4_translate(&trans2, bezier(q0, q1, q2, q3, t));
        mat4_multiply(&model2, &trans2, &rot);
        render_wireframe(canvas, cube_vertices, cube_edges,
                         CUBE_VERTEX_COUNT, CUBE_EDGE_COUNT,
                         &model2, &view, &proj, lights[1]);

        // Circular clipping
        float cx = WIDTH / 2.0f, cy = HEIGHT / 2.0f;
        float r = WIDTH / 2.0f;
        for (int y = 0; y < HEIGHT; ++y)
            for (int x = 0; x < WIDTH; ++x) {
                float dx = x + 0.5f - cx;
                float dy = y + 0.5f - cy;
                if (dx*dx + dy*dy > r*r)
                    canvas->data[y*WIDTH + x] = 0.0f;
            }

        char filename[64];
        sprintf(filename, "build/frame_%03d.pgm", frame);
        canvas_to_pgm(canvas, filename);
        printf("✅ Saved %s\n", filename);
        free_canvas(canvas);
    }

    printf("🎬 Synced animation: soccerball + cube complete.\n");
    return 0;
}
