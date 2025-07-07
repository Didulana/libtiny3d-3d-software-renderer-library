#include <math.h>
#include "math3d.h"

// Cross product
vec3_t vec3_cross(vec3_t a, vec3_t b) {
    vec3_t r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    r.r = r.theta = r.phi = 0;
    return r;
}

// Subtract
vec3_t vec3_sub(vec3_t a, vec3_t b) {
    vec3_t r = { a.x - b.x, a.y - b.y, a.z - b.z, 0, 0, 0 };
    return r;
}

// Dot product
float vec3_dot(vec3_t a, vec3_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Normalize
vec3_t vec3_normalize(vec3_t v) {
    float len = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
    if (len == 0.0f) return v;
    vec3_t r = { v.x/len, v.y/len, v.z/len, 0, 0, 0 };
    return r;
}

// Fast normalize
vec3_t vec3_normalize_fast(vec3_t v) {
    return vec3_normalize(v);
}

// Init
vec3_t vec3_init(float x, float y, float z) {
    return (vec3_t){ x, y, z, 0, 0, 0 };
}

// Matrix identity
void mat4_identity(mat4_t* m) {
    *m = (mat4_t){{
        1,0,0,0, 0,1,0,0,
        0,0,1,0, 0,0,0,1
    }};
}

// Translate
void mat4_translate(mat4_t* m, vec3_t v) {
    mat4_identity(m);
    m->m[12] = v.x;
    m->m[13] = v.y;
    m->m[14] = v.z;
}

// Multiply matrices
void mat4_multiply(mat4_t* out, mat4_t* a, mat4_t* b) {
    mat4_t r = {0};
    for (int row = 0; row < 4; ++row)
        for (int col = 0; col < 4; ++col)
            for (int k = 0; k < 4; ++k)
                r.m[row + col*4] += a->m[row + k*4] * b->m[k + col*4];
    *out = r;
}

// Quaternion -> matrix
void quat_to_mat4(mat4_t* m, quat_t q) {
    float x = q.x, y = q.y, z = q.z, w = q.w;
    *m = (mat4_t){{
        1-2*y*y-2*z*z, 2*x*y-2*z*w,   2*x*z+2*y*w,   0,
        2*x*y+2*z*w,   1-2*x*x-2*z*z, 2*y*z-2*x*w,   0,
        2*x*z-2*y*w,   2*y*z+2*x*w,   1-2*x*x-2*y*y, 0,
        0,             0,             0,             1
    }};
}

// Quaternion multiply
void quat_multiply(quat_t* out, const quat_t* a, const quat_t* b) {
    out->w = a->w*b->w - a->x*b->x - a->y*b->y - a->z*b->z;
    out->x = a->w*b->x + a->x*b->w + a->y*b->z - a->z*b->y;
    out->y = a->w*b->y - a->x*b->z + a->y*b->w + a->z*b->x;
    out->z = a->w*b->z + a->x*b->y - a->y*b->x + a->z*b->w;
}

// Quaternion from axis-angle
quat_t quat_from_axis_angle(vec3_t axis, float angle) {
    float s = sinf(angle / 2);
    return (quat_t){
        .w = cosf(angle / 2),
        .x = axis.x * s,
        .y = axis.y * s,
        .z = axis.z * s
    };
}

// Perspective matrix
void mat4_perspective(mat4_t* m, float fov, float aspect, float near, float far) {
    float f = 1.0f / tanf(fov / 2.0f);
    *m = (mat4_t){{
        f / aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (far + near) / (near - far), -1,
        0, 0, (2 * far * near) / (near - far), 0
    }};
}

// Look-at matrix
void mat4_lookat(mat4_t* out, vec3_t eye, vec3_t center, vec3_t up) {
    vec3_t f = vec3_normalize(vec3_sub(center, eye));
    vec3_t s = vec3_normalize(vec3_cross(f, up));
    vec3_t u = vec3_cross(s, f);

    *out = (mat4_t){{
        s.x, u.x, -f.x, 0,
        s.y, u.y, -f.y, 0,
        s.z, u.z, -f.z, 0,
        -vec3_dot(s, eye), -vec3_dot(u, eye), vec3_dot(f, eye), 1
    }};
}

quat_t quat_slerp(quat_t q1, quat_t q2, float t) {
    // Clamp t to [0, 1]
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    float cos_theta = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

    // If dot product is negative, slerp the opposite quaternion
    if (cos_theta < 0.0f) {
        q2.x = -q2.x;
        q2.y = -q2.y;
        q2.z = -q2.z;
        q2.w = -q2.w;
        cos_theta = -cos_theta;
    }

    float k0, k1;
    if (cos_theta > 0.9995f) {
        // Linear interpolation for small angles
        k0 = 1.0f - t;
        k1 = t;
    } else {
        float theta = acosf(cos_theta);
        float sin_theta = sinf(theta);
        k0 = sinf((1.0f - t) * theta) / sin_theta;
        k1 = sinf(t * theta) / sin_theta;
    }

    quat_t result;
    result.x = k0 * q1.x + k1 * q2.x;
    result.y = k0 * q1.y + k1 * q2.y;
    result.z = k0 * q1.z + k1 * q2.z;
    result.w = k0 * q1.w + k1 * q2.w;
    return result;
}
