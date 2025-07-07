#ifndef SOCCERBALL_H
#define SOCCERBALL_H

#include "math3d.h"

#define SOCCERBALL_VERTEX_COUNT 60
#define SOCCERBALL_EDGE_MAX 180

void generate_soccerball(vec3_t vertices[SOCCERBALL_VERTEX_COUNT],
                         int edges[SOCCERBALL_EDGE_MAX][2],
                         int* edge_count);

#endif
