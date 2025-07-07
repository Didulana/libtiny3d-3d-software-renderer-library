#ifndef LIGHTING_H
#define LIGHTING_H
#include "math3d.h"

float lambert(vec3_t edge_dir, vec3_t light_dir);
float lambert_multi(vec3_t edge_dir, vec3_t* lights, int light_count);

#endif
