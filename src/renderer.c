#include "renderer.h"
#include <stdbool.h>

bool clip_to_circular_viewport(canvas_t* canvas, int x, int y) {
    float cx = canvas->width / 2.0f;
    float cy = canvas->height / 2.0f;
    float dx = x - cx;
    float dy = y - cy;
    float dist_sq = dx*dx + dy*dy;
    float r = (canvas->width < canvas->height ? canvas->width : canvas->height) / 2.0f;
    return dist_sq < r * r;
}


void project_vertex(vec3_t in, vec3_t* out, transform_t* xf) {
    vec3_t world = vec3_project(in, &xf->model);
    vec3_t ndc   = vec3_project(world, &xf->proj);
    *out = ndc;
}

void render_wireframe(canvas_t* canvas, mesh_t* mesh, transform_t* xf, float thickness) {
    for (int i = 0; i < mesh->edge_count; i++) {
        vec3_t a = mesh->vertices[mesh->edges[i][0]];
        vec3_t b = mesh->vertices[mesh->edges[i][1]];

        vec3_t pa, pb;
        project_vertex(a, &pa, xf);
        project_vertex(b, &pb, xf);

        int x0 = (int)((pa.x * 0.5f + 0.5f) * canvas->width);
        int y0 = (int)((1.0f - (pa.y * 0.5f + 0.5f)) * canvas->height);
        int x1 = (int)((pb.x * 0.5f + 0.5f) * canvas->width);
        int y1 = (int)((1.0f - (pb.y * 0.5f + 0.5f)) * canvas->height);

        if (clip_to_circular_viewport(canvas, x0, y0) || clip_to_circular_viewport(canvas, x1, y1)) {
            draw_line_f(canvas, x0, y0, x1, y1, thickness);
        }
    }
}

