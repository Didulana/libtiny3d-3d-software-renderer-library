#include "lighting.h"

float lambert(vec3_t edge_dir, vec3_t light_dir) {
    float d = edge_dir.x * light_dir.x + edge_dir.y * light_dir.y + edge_dir.z * light_dir.z;
    return d > 0.0f ? d : 0.0f;
}

float lambert_multi(vec3_t edge_dir, vec3_t* lights, int light_count) {
    float total = 0.0f;
    for (int i = 0; i < light_count; ++i) {
        float dot = edge_dir.x * lights[i].x +
                    edge_dir.y * lights[i].y +
                    edge_dir.z * lights[i].z;
        total += dot > 0.0f ? dot : 0.0f;
    }
    float avg = total / light_count;
    return avg > 1.0f ? 1.0f : avg;
}
