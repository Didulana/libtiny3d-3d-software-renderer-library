#include "canvas.h"
#include "math3d.h"
#include "animation.h"
#include "renderer.h"
#include "lighting.h"
#include "soccerball.h"
#include <math.h>
#include <stdio.h>

#define WIDTH 512
#define HEIGHT 512
#define FRAME_COUNT 120

// Cube vertices and edges
static const vec3_t cube_vertices[] = {
    {-0.5f,-0.5f,-0.5f,0,0,0}, {0.5f,-0.5f,-0.5f,0,0,0},
    {0.5f,0.5f,-0.5f,0,0,0}, {-0.5f,0.5f,-0.5f,0,0,0},
    {-0.5f,-0.5f,0.5f,0,0,0}, {0.5f,-0.5f,0.5f,0,0,0},
    {0.5f,0.5f,0.5f,0,0,0}, {-0.5f,0.5f,0.5f,0,0,0}
};
static const int cube_edges[][2] = {
    {0,1},{1,2},{2,3},{3,0},
    {4,5},{5,6},{6,7},{7,4},
    {0,4},{1,5},{2,6},{3,7}
};
#define CUBE_VERTEX_COUNT (sizeof(cube_vertices) / sizeof(vec3_t))
#define CUBE_EDGE_COUNT   (sizeof(cube_edges)   / sizeof(cube_edges[0]))

// Pyramidal object (6-vertex fallback)
static const vec3_t pyramid_vertices[] = {
    { 0.0f,  0.5f,  0.0f, 0,0,0},
    {-0.5f,  0.0f,  0.0f, 0,0,0},
    { 0.0f, -0.5f,  0.0f, 0,0,0},
    { 0.5f,  0.0f,  0.0f, 0,0,0},
    { 0.0f,  0.0f,  0.5f, 0,0,0},
    { 0.0f,  0.0f, -0.5f, 0,0,0}
};
static const int pyramid_edges[][2] = {
    {0,1},{1,2},{2,3},{3,0},
    {0,4},{1,4},{2,4},{3,4},
    {0,5},{1,5},{2,5},{3,5}
};
#define PYRAMID_VERTEX_COUNT (sizeof(pyramid_vertices) / sizeof(vec3_t))
#define PYRAMID_EDGE_COUNT   (sizeof(pyramid_edges)   / sizeof(pyramid_edges[0]))

int main(void) {
    canvas_t* canvas = canvas_create(WIDTH, HEIGHT);

    mat4_t proj, view;
    mat4_perspective(&proj, M_PI / 3.0f, (float)WIDTH / HEIGHT, 0.1f, 100.0f);
    mat4_lookat(&view, vec3_init(0, 0, 6), vec3_init(0, 0, 0), vec3_init(0, 1, 0));

    vec3_t soccer_vertices[SOCCERBALL_VERTEX_COUNT];
    int soccer_edges[SOCCERBALL_EDGE_MAX][2];
    int soccer_edge_count = 0;
    generate_soccerball(soccer_vertices, soccer_edges, &soccer_edge_count);

    for (int frame = 0; frame < FRAME_COUNT; ++frame) {
        float t = (float)frame / (FRAME_COUNT - 1);  // ✅ ensures start == end pose
        float theta = 2.0f * M_PI * t;

        canvas_clear(canvas, 0.0f);

        // 🔆 Light orbiting overhead
        vec3_t light = vec3_normalize(vec3_init(cosf(theta), 1.0f, sinf(theta)));
        vec3_t lights[1] = { light };

        // 🧊 Cube (spin + orbit)
        // ⬅️ Cube Bezier path (left loop)
        vec3_t c0 = vec3_init(-1.6f, -0.5f, -5.0f);
        vec3_t c1 = vec3_init(-1.6f,  0.6f, -5.0f);
        vec3_t c2 = vec3_init(-0.8f,  0.6f, -5.0f);
        vec3_t c3 = vec3_init(-1.6f, -0.5f, -5.0f);
        vec3_t cube_pos = bezier(c0, c1, c2, c3, t);
        quat_t qx = quat_from_axis_angle(vec3_init(1, 0, 0), 2.0f * M_PI * t);
        quat_t qy = quat_from_axis_angle(vec3_init(0, 1, 0), 4.0f * M_PI * t);
        quat_t qz = quat_from_axis_angle(vec3_init(0, 0, 1), 1.5f * M_PI * t);
        quat_t q_xy, q_total;
        quat_multiply(&q_xy, &qy, &qx);
        quat_multiply(&q_total, &qz, &q_xy);
        mat4_t cube_rot, cube_trans, cube_model;
        quat_to_mat4(&cube_rot, q_total);
        mat4_translate(&cube_trans, cube_pos);
        mat4_multiply(&cube_model, &cube_rot, &cube_trans);
        render_wireframe_lit(canvas, cube_vertices, cube_edges, CUBE_VERTEX_COUNT, CUBE_EDGE_COUNT,
                             &cube_model, &view, &proj, lights, 1, 0.2f);

        // 🔺 Pyramid (counter-orbit + spin)
        // ➡️ Pyramid Bezier path (right loop)
        vec3_t pyr_p0 = vec3_init(1.6f, 0.6f, -5.0f);
        vec3_t pyr_p1 = vec3_init(1.6f, -0.6f, -5.0f);
        vec3_t pyr_p2 = vec3_init(0.8f, -0.6f, -5.0f);
        vec3_t pyr_p3 = vec3_init(1.6f, 0.6f, -5.0f);
        vec3_t pyr_pos = bezier(pyr_p0, pyr_p1, pyr_p2, pyr_p3, t);

        quat_t q_pyr = quat_from_axis_angle(vec3_init(1, 0, 0), theta * 2.0f);
        mat4_t pyr_rot, pyr_trans, pyr_model;
        quat_to_mat4(&pyr_rot, q_pyr);
        mat4_translate(&pyr_trans, pyr_pos);
        mat4_multiply(&pyr_model, &pyr_rot, &pyr_trans);
        render_wireframe_lit(canvas, pyramid_vertices, pyramid_edges, PYRAMID_VERTEX_COUNT, PYRAMID_EDGE_COUNT,
                             &pyr_model, &view, &proj, lights, 1, 0.2f);

        // ⚽ Soccerball — Bezier orbit + smooth spin
        vec3_t p0 = vec3_init(-1.2f, 0.0f, -5.0f);
        vec3_t p1 = vec3_init(-1.2f, 0.8f, -5.0f);
        vec3_t p2 = vec3_init(1.2f, 0.8f, -5.0f);
        vec3_t p3 = vec3_init(-1.2f, 0.0f, -5.0f);
        vec3_t ball_pos = bezier(p0, p1, p2, p3, t);

        // Proper soccerball transformation
        quat_t q_ball = quat_from_axis_angle(vec3_init(0, 1, 0), 2.0f * M_PI * t);
        mat4_t ball_rot, ball_trans, ball_model;
        quat_to_mat4(&ball_rot, q_ball);
        mat4_translate(&ball_trans, ball_pos);
        mat4_multiply(&ball_model, &ball_rot, &ball_trans);

        // ⚽ soccer_vertices and soccer_edges already filled before the loop
        render_wireframe_lit(canvas, soccer_vertices, soccer_edges,
                            SOCCERBALL_VERTEX_COUNT, soccer_edge_count,
                            &ball_model, &view, &proj, lights, 1, 0.2f);

        // ☀️ Center sun crosshair
        draw_line_f(canvas, WIDTH/2 - 3, HEIGHT/2, WIDTH/2 + 3, HEIGHT/2, 1.0f);
        draw_line_f(canvas, WIDTH/2, HEIGHT/2 - 3, WIDTH/2, HEIGHT/2 + 3, 1.0f);

        // 💾 Save frame into build/
        char filename[64];
        snprintf(filename, sizeof(filename), "build/frame_%03d.pgm", frame);
        canvas_to_pgm(canvas, filename);
        printf("✅ Saved %s\n", filename);
    }

    canvas_destroy(canvas);
    return 0;
}
