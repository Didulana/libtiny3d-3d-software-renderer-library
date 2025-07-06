#include "math3d.h"
#include <math.h>
#include <stdint.h>

vec3_t vec3_init(float x, float y, float z) {
    return (vec3_t){ x, y, z, 0.0f, 0.0f, 0.0f };
}

vec3_t vec3_from_spherical(float r, float theta, float phi) {
    return (vec3_t){
        r * sinf(theta) * cosf(phi),
        r * sinf(theta) * sinf(phi),
        r * cosf(theta),
        r, theta, phi
    };
}

vec3_t vec3_normalize_fast(vec3_t v) {
    float len_squared = v.x * v.x + v.y * v.y + v.z * v.z;
    float xhalf = 0.5f * len_squared;
    int32_t i = *(int32_t*)&len_squared;
    i = 0x5f3759df - (i >> 1);  // Fast inverse square root
    float inv_sqrt = *(float*)&i;
    inv_sqrt *= 1.5f - xhalf * inv_sqrt * inv_sqrt;

    v.x *= inv_sqrt;
    v.y *= inv_sqrt;
    v.z *= inv_sqrt;
    return v;
}

vec3_t vec3_slerp(vec3_t a, vec3_t b, float t) {
    float dot = fmaxf(fminf(a.x * b.x + a.y * b.y + a.z * b.z, 1.0f), -1.0f);
    float theta = acosf(dot) * t;

    vec3_t r = {
        b.x - a.x * dot,
        b.y - a.y * dot,
        b.z - a.z * dot,
        0, 0, 0
    };

    float len = sqrtf(r.x*r.x + r.y*r.y + r.z*r.z);
    if (len > 0.0f) {
        r.x /= len;
        r.y /= len;
        r.z /= len;
    }

    return (vec3_t){
        a.x * cosf(theta) + r.x * sinf(theta),
        a.y * cosf(theta) + r.y * sinf(theta),
        a.z * cosf(theta) + r.z * sinf(theta),
        0, 0, 0
    };
}

vec3_t vec3_project(vec3_t p, mat4_t* m) {
    float xp = m->m[0][0]*p.x + m->m[0][1]*p.y + m->m[0][2]*p.z + m->m[0][3];
    float yp = m->m[1][0]*p.x + m->m[1][1]*p.y + m->m[1][2]*p.z + m->m[1][3];
    float zp = m->m[2][0]*p.x + m->m[2][1]*p.y + m->m[2][2]*p.z + m->m[2][3];
    float wp = m->m[3][0]*p.x + m->m[3][1]*p.y + m->m[3][2]*p.z + m->m[3][3];

    if (wp != 0.0f) {
        xp /= wp;
        yp /= wp;
        zp /= wp;
    }

    return vec3_init(xp, yp, zp);
}

void mat4_identity(mat4_t* out) {
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            out->m[r][c] = (r == c) ? 1.0f : 0.0f;
}

void mat4_translate(mat4_t* out, vec3_t v) {
    mat4_identity(out);
    out->m[0][3] = v.x;
    out->m[1][3] = v.y;
    out->m[2][3] = v.z;
}

void mat4_scale(mat4_t* out, vec3_t s) {
    mat4_identity(out);
    out->m[0][0] = s.x;
    out->m[1][1] = s.y;
    out->m[2][2] = s.z;
}

void mat4_rotate_xyz(mat4_t* out, vec3_t angles) {
    float sx = sinf(angles.x), cx = cosf(angles.x);
    float sy = sinf(angles.y), cy = cosf(angles.y);
    float sz = sinf(angles.z), cz = cosf(angles.z);

    out->m[0][0] = cy * cz;
    out->m[0][1] = -cy * sz;
    out->m[0][2] = sy;
    out->m[0][3] = 0;

    out->m[1][0] = sx * sy * cz + cx * sz;
    out->m[1][1] = -sx * sy * sz + cx * cz;
    out->m[1][2] = -sx * cy;
    out->m[1][3] = 0;

    out->m[2][0] = -cx * sy * cz + sx * sz;
    out->m[2][1] = cx * sy * sz + sx * cz;
    out->m[2][2] = cx * cy;
    out->m[2][3] = 0;

    out->m[3][0] = 0;
    out->m[3][1] = 0;
    out->m[3][2] = 0;
    out->m[3][3] = 1;
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
