#include "lighting.h"

float lambert(vec3_t edge_dir, vec3_t light_dir) {
    float d = edge_dir.x * light_dir.x + edge_dir.y * light_dir.y + edge_dir.z * light_dir.z;
    return d > 0 ? d : 0;
}
