#ifndef RENDERER_H
#define RENDERER_H

#include "math3d.h"
#include "canvas.h"

// Transforms a vertex through Model, View, and Projection matrices
vec3_t project_vertex(vec3_t point, mat4_t *model, mat4_t *view, mat4_t *proj);

// Checks if a point lies inside a circular viewport
int clip_to_circular_viewport(canvas_t *canvas, float x, float y);

// Draws the wireframe of an object from a list of vertices and edges
void render_wireframe(canvas_t *canvas, vec3_t *vertices, int (*edges)[2],
                      int vertex_count, int edge_count,
                      mat4_t *model, mat4_t *view, mat4_t *proj);

#endif
