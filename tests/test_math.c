#include <stdio.h>
#include <math.h>
#include "math3d.h"

int main() {
    // Test 1: Fast normalization
    vec3_t v = vec3_init(3.0f, 4.0f, 0.0f);
    vec3_t n = vec3_normalize_fast(v);
    float len = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);

    printf("=== Normalization Test ===\n");
    printf("Original:    (%.2f, %.2f, %.2f)\n", v.x, v.y, v.z);
    printf("Normalized:  (%.2f, %.2f, %.2f)\n", n.x, n.y, n.z);
    printf("Length: %.4f\n\n", len);

    // Test 2: Slerp from X to Y axis
    vec3_t a = vec3_normalize_fast(vec3_init(1.0f, 0.0f, 0.0f));
    vec3_t b = vec3_normalize_fast(vec3_init(0.0f, 1.0f, 0.0f));

    printf("=== Slerp X → Y ===\n");
    for (int i = 0; i <= 10; i++) {
        float t = i / 10.0f;
        vec3_t s = vec3_slerp(a, b, t);
        printf("t=%.1f → (%.3f, %.3f, %.3f)\n", t, s.x, s.y, s.z);
    }

    // Test 3: Identity matrix
    mat4_t id;
    mat4_identity(&id);

    printf("\n=== Identity Matrix ===\n");
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            printf("%6.2f ", id.m[row][col]);
        }
        printf("\n");
    }

    // Test 4: Translation matrix
    vec3_t offset = vec3_init(10.0f, -5.0f, 2.5f);
    mat4_t trans;
    mat4_translate(&trans, offset);

    printf("\n=== Translation Matrix (10, -5, 2.5) ===\n");
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            printf("%6.2f ", trans.m[row][col]);
        }
        printf("\n");
    }

    // Test 5: Rotation matrix (90° around Z)
    mat4_t rot;
    float angle_deg = 90.0f;
    float angle_rad = angle_deg * (float)M_PI / 180.0f;
    mat4_rotate_xyz(&rot, vec3_init(0.0f, 0.0f, angle_rad));

    printf("\n=== Rotation Matrix (90° Z) ===\n");
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            printf("%6.2f ", rot.m[row][col]);
        }
        printf("\n");
    }

    // Test 6: Scale Matrix
    mat4_t scale;
    mat4_scale(&scale, vec3_init(2.0f, 0.5f, -1.0f));
    printf("\n=== Scale Matrix (2.0, 0.5, -1.0) ===\n");
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            printf("%6.2f ", scale.m[r][c]);
        }
        printf("\n");
    }

    // Test 6: Asymmetric Frustum Projection Matrix
    mat4_t proj;
    mat4_frustum_asymmetric(&proj, -1.0f, 1.0f, -0.75f, 0.75f, 1.0f, 10.0f);

    printf("\n=== Frustum Projection Matrix ===\n");
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            printf("%8.4f ", proj.m[r][c]);
        }
        printf("\n");
    }
    
    // Test 7: Project a 3D point using the frustum matrix
    vec3_t point = vec3_init(0.0f, 0.0f, -2.0f); // In front of the camera
    vec3_t projected = vec3_project(point, &proj);

    printf("\n=== Projected Point (0, 0, -2) ===\n");
    printf("→ (%.4f, %.4f, %.4f)\n", projected.x, projected.y, projected.z);

    return 0;
}
