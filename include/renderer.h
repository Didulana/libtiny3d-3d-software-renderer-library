#ifndef RENDERER_H
#define RENDERER_H

#include "canvas.h"
#include "math3d.h"

void render_wireframe(canvas_t* c, vec3_t* vertices, int edges[][2], int vcount, int ecount,
                      mat4_t* model, mat4_t* view, mat4_t* proj, vec3_t light_dir);

#endif
