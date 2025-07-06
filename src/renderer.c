#include "renderer.h"
#include <math.h>

vec3_t project_vertex(vec3_t point, mat4_t *model, mat4_t *view, mat4_t *proj) {
    vec3_t world = vec3_project(point, model);
    vec3_t camera = vec3_project(world, view);
    return vec3_project(camera, proj);
}

int clip_to_circular_viewport(canvas_t *canvas, float x, float y) {
    float cx = canvas->width / 2.0f;
    float cy = canvas->height / 2.0f;
    float dx = x - cx;
    float dy = y - cy;
    float radius = fminf(cx, cy);
    return (dx * dx + dy * dy <= radius * radius);
}

void render_wireframe(canvas_t *canvas, vec3_t *vertices, int (*edges)[2],
                      int vertex_count, int edge_count,
                      mat4_t *model, mat4_t *view, mat4_t *proj) {
    for (int i = 0; i < edge_count; i++) {
        vec3_t p1 = project_vertex(vertices[edges[i][0]], model, view, proj);
        vec3_t p2 = project_vertex(vertices[edges[i][1]], model, view, proj);

        float x0 = (p1.x * 0.5f + 0.5f) * canvas->width;
        float y0 = (1.0f - (p1.y * 0.5f + 0.5f)) * canvas->height;
        float x1 = (p2.x * 0.5f + 0.5f) * canvas->width;
        float y1 = (1.0f - (p2.y * 0.5f + 0.5f)) * canvas->height;

        if (clip_to_circular_viewport(canvas, x0, y0) || clip_to_circular_viewport(canvas, x1, y1)) {
            draw_line_f(canvas, x0, y0, x1, y1, 1.5f);
        }
    }
}
