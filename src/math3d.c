#include "math3d.h"
#include <math.h>

vec3_t vec3_init(float x, float y, float z) {
    vec3_t v = { x, y, z, 0, 0, 0 };
    return v;
}

vec3_t vec3_normalize_fast(vec3_t v) {
    float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len > 1e-6f) {
        v.x /= len;
        v.y /= len;
        v.z /= len;
    }
    return v;
}

quat_t quat_from_axis_angle(vec3_t axis, float angle) {
    float half = angle * 0.5f;
    float s = sinf(half);
    return (quat_t){ cosf(half), axis.x * s, axis.y * s, axis.z * s };
}

quat_t quat_slerp(quat_t a, quat_t b, float t) {
    float dot = a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z;
    if (dot < 0.0f) {
        b.w = -b.w; b.x = -b.x; b.y = -b.y; b.z = -b.z;
        dot = -dot;
    }

    if (dot > 0.9995f) {
        quat_t r = {
            a.w + t*(b.w - a.w),
            a.x + t*(b.x - a.x),
            a.y + t*(b.y - a.y),
            a.z + t*(b.z - a.z)
        };
        float len = sqrtf(r.w*r.w + r.x*r.x + r.y*r.y + r.z*r.z);
        r.w /= len; r.x /= len; r.y /= len; r.z /= len;
        return r;
    }

    float theta_0 = acosf(dot);
    float theta = theta_0 * t;
    float sin_theta = sinf(theta);
    float sin_theta_0 = sinf(theta_0);

    float s0 = cosf(theta) - dot * sin_theta / sin_theta_0;
    float s1 = sin_theta / sin_theta_0;

    return (quat_t){
        a.w * s0 + b.w * s1,
        a.x * s0 + b.x * s1,
        a.y * s0 + b.y * s1,
        a.z * s0 + b.z * s1
    };
}

void quat_to_mat4(mat4_t* out, quat_t q) {
    float xx = q.x * q.x, yy = q.y * q.y, zz = q.z * q.z;
    float xy = q.x * q.y, xz = q.x * q.z, yz = q.y * q.z;
    float wx = q.w * q.x, wy = q.w * q.y, wz = q.w * q.z;

    mat4_identity(out);
    out->m[0]  = 1.0f - 2.0f * (yy + zz);
    out->m[1]  = 2.0f * (xy - wz);
    out->m[2]  = 2.0f * (xz + wy);
    out->m[4]  = 2.0f * (xy + wz);
    out->m[5]  = 1.0f - 2.0f * (xx + zz);
    out->m[6]  = 2.0f * (yz - wx);
    out->m[8]  = 2.0f * (xz - wy);
    out->m[9]  = 2.0f * (yz + wx);
    out->m[10] = 1.0f - 2.0f * (xx + yy);
}

void mat4_identity(mat4_t* m) {
    for (int i = 0; i < 16; ++i) m->m[i] = 0;
    m->m[0] = m->m[5] = m->m[10] = m->m[15] = 1.0f;
}

void mat4_translate(mat4_t* m, vec3_t v) {
    mat4_identity(m);
    m->m[12] = v.x;
    m->m[13] = v.y;
    m->m[14] = v.z;
}

void mat4_multiply(mat4_t* out, mat4_t* a, mat4_t* b) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            out->m[i*4 + j] = 0;
            for (int k = 0; k < 4; ++k)
                out->m[i*4 + j] += a->m[i*4 + k] * b->m[k*4 + j];
        }
}

void mat4_frustum_asymmetric(mat4_t* m, float l, float r, float b, float t, float n, float f) {
    for (int i = 0; i < 16; ++i) m->m[i] = 0;
    m->m[0] = 2*n/(r - l);
    m->m[5] = 2*n/(t - b);
    m->m[8] = (r + l)/(r - l);
    m->m[9] = (t + b)/(t - b);
    m->m[10] = -(f + n)/(f - n);
    m->m[11] = -1;
    m->m[14] = -(2*f*n)/(f - n);
}
