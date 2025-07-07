#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "canvas.h"
#include "math3d.h"
#include "renderer.h"
#include "lighting.h"
#include "soccerball.h"

#define WIDTH 512
#define HEIGHT 512
#define FRAME_COUNT 120

int main() {
    // Load soccer ball mesh
    vec3_t soccer_vertices[SOCCERBALL_VERTEX_COUNT];
    int soccer_edges[SOCCERBALL_EDGE_MAX][2];
    int soccer_edge_count = 0;
    generate_soccerball(soccer_vertices, soccer_edges, &soccer_edge_count);

    // Set up view and projection
    mat4_t view, proj;
    mat4_identity(&view);
    mat4_frustum_asymmetric(&proj, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 10.0f);

    // Light direction
    vec3_t light_dir = vec3_normalize_fast(vec3_init(1.0f, 1.5f, -1.0f));

    // Rotation setup
    quat_t q_start = quat_from_axis_angle(vec3_init(1, 1, 0), 0.0f);
    quat_t q_end   = quat_from_axis_angle(vec3_init(1, 1, 0), 2.0f * M_PI);

    // Orbit radius
    float radius = 2.0f;
    float center_x = 0.0f;
    float center_y = 0.0f;
    float z_plane = -4.0f;

    for (int frame = 0; frame < FRAME_COUNT; ++frame) {
        canvas_t* canvas = create_canvas(WIDTH, HEIGHT);

        float t = (float)frame / (FRAME_COUNT - 1);
        float angle = 2.0f * M_PI * t;

        // Soccer ball position around circle
        vec3_t pos = vec3_init(center_x + radius * cosf(angle),
                               center_y + radius * sinf(angle),
                               z_plane);

        // Quaternion rotation
        quat_t q_rot = quat_slerp(q_start, q_end, t);
        mat4_t rot;
        quat_to_mat4(&rot, q_rot);

        // Translation matrix
        mat4_t trans;
        mat4_translate(&trans, pos);

        // Model = T × R
        mat4_t model;
        mat4_multiply(&model, &trans, &rot);

        // Render ball
        render_wireframe(canvas, soccer_vertices, soccer_edges,
                         SOCCERBALL_VERTEX_COUNT, soccer_edge_count,
                         &model, &view, &proj, light_dir);

        // Circular clipping
        float cx = WIDTH / 2.0f, cy = HEIGHT / 2.0f;
        float clip_r = (WIDTH < HEIGHT ? WIDTH : HEIGHT) / 2.0f;
        for (int y = 0; y < HEIGHT; ++y)
            for (int x = 0; x < WIDTH; ++x) {
                float dx = x + 0.5f - cx;
                float dy = y + 0.5f - cy;
                if (dx * dx + dy * dy > clip_r * clip_r)
                    canvas->data[y * WIDTH + x] = 0.0f;
            }

        // Export frame
        char filename[64];
        sprintf(filename, "build/frame_%03d.pgm", frame);
        canvas_to_pgm(canvas, filename);
        printf("✅ Saved %s\n", filename);

        free_canvas(canvas);
    }

    printf("\n🌐 Complete circular orbit animation generated across %d frames!\n", FRAME_COUNT);
    return 0;
}
