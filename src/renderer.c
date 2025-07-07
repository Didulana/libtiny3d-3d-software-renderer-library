#include "renderer.h"
#include "lighting.h"
#include "math3d.h"
#include <math.h>

// Apply a 4x4 matrix to a 3D point (assumes w = 1.0)
vec3_t vec3_transform(mat4_t m, vec3_t v) {
    vec3_t r;
    r.x = m.m[0]*v.x + m.m[4]*v.y + m.m[8]*v.z + m.m[12];
    r.y = m.m[1]*v.x + m.m[5]*v.y + m.m[9]*v.z + m.m[13];
    r.z = m.m[2]*v.x + m.m[6]*v.y + m.m[10]*v.z + m.m[14];
    r.r = r.theta = r.phi = 0;
    return r;
}

// Full transform pipeline: model → view → proj → NDC
vec3_t project(const mat4_t* proj, const mat4_t* view, const mat4_t* model, vec3_t v) {
    vec3_t p = vec3_transform(*model, v);
    p = vec3_transform(*view, p);
    vec3_t clip;
    float x = p.x, y = p.y, z = p.z;
    float w = proj->m[3]*x + proj->m[7]*y + proj->m[11]*z + proj->m[15];
    if (w == 0.0f) w = 1.0f;
    clip.x = (proj->m[0]*x + proj->m[4]*y + proj->m[8]*z + proj->m[12]) / w;
    clip.y = (proj->m[1]*x + proj->m[5]*y + proj->m[9]*z + proj->m[13]) / w;
    clip.z = (proj->m[2]*x + proj->m[6]*y + proj->m[10]*z + proj->m[14]) / w;
    clip.r = clip.theta = clip.phi = 0;
    return clip;
}


static void mat4_apply(const mat4_t* m, const float in[4], float out[4]) {
    for (int i = 0; i < 4; ++i) {
        out[i] = m->m[0*4 + i]*in[0] + m->m[1*4 + i]*in[1] +
                 m->m[2*4 + i]*in[2] + m->m[3*4 + i]*in[3];
    }
}

static void project_vertex(canvas_t* c, vec3_t v,
                           const mat4_t* model, const mat4_t* view, const mat4_t* proj,
                           float* x, float* y, float* z_out) {
    float p[4] = { v.x, v.y, v.z, 1.0f }, t1[4], t2[4], t3[4];
    mat4_apply(model, p, t1);
    mat4_apply(view,  t1, t2);
    mat4_apply(proj,  t2, t3);

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
    (void)vcount;
    for (int i = 0; i < ecount; ++i) {
        int a = edges[i][0], b = edges[i][1];
        float x0, y0, z0, x1, y1, z1;
        project_vertex(c, vertices[a], model, view, proj, &x0, &y0, &z0);
        project_vertex(c, vertices[b], model, view, proj, &x1, &y1, &z1);

        vec3_t dir = vec3_normalize_fast(vec3_sub(vertices[b], vertices[a]));
        float intensity = lambert(dir, light_dir);
        intensity = fmaxf(intensity, 0.15f);

        draw_line_f(c, x0, y0, x1, y1, 1.5f * intensity);
    }
}

void render_wireframe_lit(canvas_t* canvas,
                          const vec3_t* vertices, const int edges[][2],
                          int vcount, int ecount,
                          const mat4_t* model, const mat4_t* view, const mat4_t* proj,
                          const vec3_t* lights, int light_count,
                          float ambient) {
    (void)vcount;
    for (int i = 0; i < ecount; ++i) {
        int a = edges[i][0], b = edges[i][1];
        float x0, y0, z0, x1, y1, z1;

        project_vertex(canvas, vertices[a], model, view, proj, &x0, &y0, &z0);
        project_vertex(canvas, vertices[b], model, view, proj, &x1, &y1, &z1);

        float pa[4] = { vertices[a].x, vertices[a].y, vertices[a].z, 1.0f };
        float pb[4] = { vertices[b].x, vertices[b].y, vertices[b].z, 1.0f };
        float wa[4], wb[4];

        mat4_apply(model, pa, wa);
        mat4_apply(model, pb, wb);

        vec3_t dir = vec3_normalize_fast(vec3_init(wb[0] - wa[0], wb[1] - wa[1], wb[2] - wa[2]));
        float shade = lambert_multi(dir, lights, light_count);
        float dz = (z0 + z1) * 0.5f;                         // average projected depth
        float attenuation = 1.0f / (1.0f + 0.5f * dz * dz);   // tweak strength as needed
        float final = (ambient + (1.0f - ambient) * shade) * attenuation;

        draw_line_f(canvas, x0, y0, x1, y1, final);
    }
}
