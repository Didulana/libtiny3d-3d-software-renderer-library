#include <math.h>
#include <stdint.h>
#include "math3d.h"

vec3_t vec3_init(float x, float y, float z) {
    vec3_t v;
    v.x = x;
    v.y = y;
    v.z = z;
    v.r = 0.0f;
    v.theta = 0.0f;
    v.phi = 0.0f;
    return v;
}

vec3_t vec3_from_spherical(float r, float theta, float phi) {
    vec3_t v;
    v.r = r;
    v.theta = theta;
    v.phi = phi;

    v.x = r * sinf(theta) * cosf(phi);
    v.y = r * sinf(theta) * sinf(phi);
    v.z = r * cosf(theta);
    return v;
}

vec3_t vec3_normalize_fast(vec3_t v) {
    float len_squared = v.x * v.x + v.y * v.y + v.z * v.z;
    float xhalf = 0.5f * len_squared;
    int32_t i = *(int32_t*)&len_squared;
    i = 0x5f3759df - (i >> 1);
    float inv_sqrt = *(float*)&i;
    inv_sqrt = inv_sqrt * (1.5f - xhalf * inv_sqrt * inv_sqrt);

    v.x *= inv_sqrt;
    v.y *= inv_sqrt;
    v.z *= inv_sqrt;

    return v;
}

vec3_t vec3_slerp(vec3_t a, vec3_t b, float t) {
    float dot = a.x * b.x + a.y * b.y + a.z * b.z;
    dot = fmaxf(fminf(dot, 1.0f), -1.0f);
    float theta = acosf(dot) * t;

    vec3_t r;
    r.x = b.x - a.x * dot;
    r.y = b.y - a.y * dot;
    r.z = b.z - a.z * dot;

    float len = sqrtf(r.x * r.x + r.y * r.y + r.z * r.z);
    if (len > 0.0f) {
        r.x /= len;
        r.y /= len;
        r.z /= len;
    }

    vec3_t result;
    result.x = a.x * cosf(theta) + r.x * sinf(theta);
    result.y = a.y * cosf(theta) + r.y * sinf(theta);
    result.z = a.z * cosf(theta) + r.z * sinf(theta);
    result.r = 0.0f;
    result.theta = 0.0f;
    result.phi = 0.0f;
    return result;
}

void mat4_identity(mat4_t* out) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            out->m[row][col] = (row == col) ? 1.0f : 0.0f;
        }
    }
}

void mat4_translate(mat4_t* out, vec3_t v) {
    mat4_identity(out);
    out->m[0][3] = v.x;
    out->m[1][3] = v.y;
    out->m[2][3] = v.z;
}

void mat4_rotate_xyz(mat4_t* out, vec3_t angles) {
    float sx = sinf(angles.x), cx = cosf(angles.x);
    float sy = sinf(angles.y), cy = cosf(angles.y);
    float sz = sinf(angles.z), cz = cosf(angles.z);

    // Combined rotation: Rz * Ry * Rx (Z applied last)
    out->m[0][0] = cy * cz;
    out->m[0][1] = -cy * sz;
    out->m[0][2] = sy;
    out->m[0][3] = 0.0f;

    out->m[1][0] = sx * sy * cz + cx * sz;
    out->m[1][1] = -sx * sy * sz + cx * cz;
    out->m[1][2] = -sx * cy;
    out->m[1][3] = 0.0f;

    out->m[2][0] = -cx * sy * cz + sx * sz;
    out->m[2][1] = cx * sy * sz + sx * cz;
    out->m[2][2] = cx * cy;
    out->m[2][3] = 0.0f;

    out->m[3][0] = 0.0f;
    out->m[3][1] = 0.0f;
    out->m[3][2] = 0.0f;
    out->m[3][3] = 1.0f;
}

void mat4_scale(mat4_t* out, vec3_t s) {
    mat4_identity(out);
    out->m[0][0] = s.x;
    out->m[1][1] = s.y;
    out->m[2][2] = s.z;
}

void mat4_frustum_asymmetric(mat4_t* out, float l, float r, float b, float t, float n, float f) {
    mat4_identity(out);

    out->m[0][0] = (2.0f * n) / (r - l);
    out->m[0][2] = (r + l) / (r - l);

    out->m[1][1] = (2.0f * n) / (t - b);
    out->m[1][2] = (t + b) / (t - b);

    out->m[2][2] = -(f + n) / (f - n);
    out->m[2][3] = -(2.0f * f * n) / (f - n);

    out->m[3][2] = -1.0f;
    out->m[3][3] = 0.0f;
}

vec3_t vec3_project(vec3_t p, mat4_t* m) {
    float x = p.x, y = p.y, z = p.z;

    float xp = m->m[0][0]*x + m->m[0][1]*y + m->m[0][2]*z + m->m[0][3];
    float yp = m->m[1][0]*x + m->m[1][1]*y + m->m[1][2]*z + m->m[1][3];
    float zp = m->m[2][0]*x + m->m[2][1]*y + m->m[2][2]*z + m->m[2][3];
    float wp = m->m[3][0]*x + m->m[3][1]*y + m->m[3][2]*z + m->m[3][3];

    if (wp != 0.0f) {
        xp /= wp;
        yp /= wp;
        zp /= wp;
    }

    return vec3_init(xp, yp, zp);
}

