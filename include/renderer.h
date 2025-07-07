#ifndef RENDERER_H
#define RENDERER_H

#include "canvas.h"
#include "math3d.h"

// Basic unlit wireframe renderer
void render_wireframe(canvas_t* canvas, vec3_t* vertices, int edges[][2],
                      int vcount, int ecount,
                      mat4_t* model, mat4_t* view, mat4_t* proj,
                      vec3_t light_dir);

// Lighting-based wireframe renderer (Lambert shading)
void render_wireframe_lit(canvas_t* canvas,
                          const vec3_t* vertices, const int edges[][2],
                          int vcount, int ecount,
                          const mat4_t* model, const mat4_t* view, const mat4_t* proj,
                          const vec3_t* lights, int light_count,
                          float ambient);

// Computes per-edge brightness from lighting
float edge_brightness(vec3_t v0, vec3_t v1, const mat4_t* model,
                      const mat4_t* view, const mat4_t* proj,
                      const vec3_t* lights, int light_count, float ambient);

// Matrix/vector utilities used in rendering
vec3_t vec3_transform(mat4_t m, vec3_t v);
vec3_t project(const mat4_t* proj, const mat4_t* view, const mat4_t* model, vec3_t v);

#endif
