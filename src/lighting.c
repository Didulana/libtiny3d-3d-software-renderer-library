#include "lighting.h"
#include "math3d.h"
#include <math.h>

float lambert(vec3_t edge_dir, vec3_t light_dir) {
    float dot = vec3_dot(edge_dir, light_dir);
    return dot > 0.0f ? dot : 0.0f;
}

float lambert_multi(vec3_t edge_dir, const vec3_t* lights, int count) {
    float total = 0.0f;
    for (int i = 0; i < count; ++i) {
        float dot = vec3_dot(edge_dir, lights[i]);
        total += dot > 0.0f ? dot : 0.0f;
    }
    return fminf(1.0f, total / count);
}
