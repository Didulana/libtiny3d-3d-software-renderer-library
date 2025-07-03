#ifndef MATH3D_H
#define MATH3D_H

typedef struct {
    float x, y, z;         // Cartesian
    float r, theta, phi;   // Spherical
} vec3_t;

vec3_t vec3_init(float x, float y, float z);
vec3_t vec3_from_spherical(float r, float theta, float phi);
vec3_t vec3_normalize_fast(vec3_t v);
vec3_t vec3_slerp(vec3_t a, vec3_t b, float t);

typedef struct {
    float m[4][4];
} mat4_t;

void mat4_identity(mat4_t* out);
void mat4_translate(mat4_t* out, vec3_t v);
void mat4_rotate_xyz(mat4_t* out, vec3_t angles);  // angles in radians
void mat4_scale(mat4_t* out, vec3_t scale_factors);
void mat4_frustum_asymmetric(mat4_t* out, float l, float r, float b, float t, float n, float f);

vec3_t vec3_project(vec3_t p, mat4_t* m);

#endif
