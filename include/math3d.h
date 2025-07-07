#ifndef MATH3D_H
#define MATH3D_H

typedef struct { float x, y, z; float r, theta, phi; } vec3_t;
typedef struct { float w, x, y, z; } quat_t;
typedef struct { float m[16]; } mat4_t;

float vec3_dot(vec3_t a, vec3_t b);
vec3_t vec3_sub(vec3_t a, vec3_t b);
vec3_t vec3_cross(vec3_t a, vec3_t b);
vec3_t vec3_init(float x, float y, float z);
vec3_t vec3_normalize(vec3_t v);
vec3_t vec3_normalize_fast(vec3_t v);

quat_t quat_from_axis_angle(vec3_t axis, float angle);
// In include/math3d.h — add this line:
quat_t quat_slerp(quat_t q1, quat_t q2, float t);

void quat_to_mat4(mat4_t* out, quat_t q);
void quat_multiply(quat_t* out, const quat_t* q1, const quat_t* q2);

void mat4_identity(mat4_t* m);
void mat4_translate(mat4_t* m, vec3_t v);
void mat4_multiply(mat4_t* out, mat4_t* a, mat4_t* b);
void mat4_perspective(mat4_t* out, float fov, float aspect, float near, float far);
void mat4_lookat(mat4_t* out, vec3_t eye, vec3_t center, vec3_t up);

#endif
