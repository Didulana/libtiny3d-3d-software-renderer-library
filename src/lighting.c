#include "lighting.h"

float lambert_intensity(vec3_t edge_dir, vec3_t light_dir) {
    vec3_t d = vec3_normalize_fast(edge_dir);
    vec3_t l = vec3_normalize_fast(light_dir);
    float dot = d.x * l.x + d.y * l.y + d.z * l.z;
    return dot < 0.0f ? 0.0f : dot;
}
