#include "renderer.h"
#include "lighting.h"
#include <math.h>

static void mat4_apply(const mat4_t* m, const float in[4], float out[4]) {
    for (int i = 0; i < 4; ++i) {
        out[i] = m->m[0*4 + i] * in[0]
               + m->m[1*4 + i] * in[1]
               + m->m[2*4 + i] * in[2]
               + m->m[3*4 + i] * in[3];
    }
}

static void project_vertex(canvas_t* c, vec3_t v,
                           mat4_t* model, mat4_t* view, mat4_t* proj,
                           float* x, float* y, float* z_out) {
    float p[4] = { v.x, v.y, v.z, 1.0f };
    float t1[4], t2[4], t3[4];

    mat4_apply(model, p, t1);
    mat4_apply(view, t1, t2);
    mat4_apply(proj, t2, t3);

    if (t3[3] != 0.0f) {
        float ndc_x = t3[0] / t3[3];
        float ndc_y = t3[1] / t3[3];
        *x = (ndc_x * 0.5f + 0.5f) * c->width;
        *y = (1.0f - (ndc_y * 0.5f + 0.5f)) * c->height;
        *z_out = t3[2] / t3[3];
    } else {
        *x = *y = *z_out = 0.0f;
    }
}

void render_wireframe(canvas_t* c, vec3_t* vertices, int edges[][2],
                      int vcount, int ecount,
                      mat4_t* model, mat4_t* view, mat4_t* proj,
                      vec3_t light_dir) {
    for (int i = 0; i < ecount; ++i) {
        int a = edges[i][0];
        int b = edges[i][1];

        float x0, y0, z0;
        float x1, y1, z1;

        project_vertex(c, vertices[a], model, view, proj, &x0, &y0, &z0);
        project_vertex(c, vertices[b], model, view, proj, &x1, &y1, &z1);

        vec3_t dir = vec3_init(vertices[b].x - vertices[a].x,
                               vertices[b].y - vertices[a].y,
                               vertices[b].z - vertices[a].z);
        dir = vec3_normalize_fast(dir);

        float intensity = lambert(dir, light_dir);
        if (intensity < 0.15f) intensity = 0.15f;

        draw_line_f(c, x0, y0, x1, y1, 1.5f * intensity);
    }
}
