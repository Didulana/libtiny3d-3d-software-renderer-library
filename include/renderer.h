#ifndef RENDERER_H
#define RENDERER_H

#include "canvas.h"
#include "math3d.h"
#include <stdbool.h>

typedef struct {
    vec3_t* vertices;
    int vertex_count;
    int (*edges)[2];
    int edge_count;
} mesh_t;

void project_vertex(vec3_t v, vec3_t* out, transform_t* tf);
bool clip_to_circular_viewport(canvas_t* c, int x, int y);

#endif
