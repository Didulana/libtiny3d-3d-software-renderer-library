#ifndef MATH3D_H
#define MATH3D_H

typedef struct { float x, y, z; float r, theta, phi; } vec3_t;
typedef struct { float w, x, y, z; } quat_t;
typedef struct { float m[16]; } mat4_t;

vec3_t vec3_init(float x, float y, float z);
vec3_t vec3_normalize_fast(vec3_t v);

quat_t quat_from_axis_angle(vec3_t axis, float angle);
quat_t quat_slerp(quat_t a, quat_t b, float t);
void quat_to_mat4(mat4_t* out, quat_t q);

void mat4_identity(mat4_t* m);
void mat4_translate(mat4_t* m, vec3_t v);
void mat4_multiply(mat4_t* out, mat4_t* a, mat4_t* b);
void mat4_frustum_asymmetric(mat4_t* m, float l, float r, float b, float t, float n, float f);

#endif
