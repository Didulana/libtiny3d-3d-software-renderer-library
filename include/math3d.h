#ifndef MATH3D_H
#define MATH3D_H

typedef struct {
    float x, y, z;
    float r, theta, phi;
} vec3_t;

typedef struct {
    float m[4][4];
} mat4_t;

typedef struct {
    mat4_t model;
    mat4_t view;
    mat4_t proj;
} transform_t;

vec3_t vec3_init(float x, float y, float z);
vec3_t vec3_normalize_fast(vec3_t v);
vec3_t vec3_from_spherical(float r, float theta, float phi);
vec3_t vec3_slerp(vec3_t a, vec3_t b, float t);
vec3_t vec3_project(vec3_t p, mat4_t* m);

void mat4_identity(mat4_t* m);
void mat4_rotate_xyz(mat4_t* m, vec3_t angles);
void mat4_translate(mat4_t* m, vec3_t t);
void mat4_frustum_asymmetric(mat4_t* m, float l, float r, float b, float t, float n, float f);

#endif
