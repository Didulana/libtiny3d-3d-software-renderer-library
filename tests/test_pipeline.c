#include <stdio.h>
#include <math.h>
#include "math3d.h"

#define WIDTH  640
#define HEIGHT 480

// Framebuffer: 1 byte per pixel, initialized to black (0)
unsigned char framebuffer[HEIGHT][WIDTH];

// Set a pixel in the framebuffer to white
void put_pixel(int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        framebuffer[y][x] = 255;
}

// Edge function: returns signed area of triangle edge (used for barycentric test)
float edge_func(int x0, int y0, int x1, int y1, int x, int y) {
    return (x - x0) * (y1 - y0) - (y - y0) * (x1 - x0);
}

// Rasterize a filled triangle using edge functions (barycentric method)
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2) {
    // Compute screen-space bounding box of triangle
    int min_x = fmaxf(0, fminf(fminf(x0, x1), x2));
    int max_x = fminf(WIDTH - 1, fmaxf(fmaxf(x0, x1), x2));
    int min_y = fmaxf(0, fminf(fminf(y0, y1), y2));
    int max_y = fminf(HEIGHT - 1, fmaxf(fmaxf(y0, y1), y2));

    // Early exit if degenerate triangle
    float area = edge_func(x0, y0, x1, y1, x2, y2);
    if (area == 0.0f) return;

    // For each pixel inside bounding box
    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            // Compute edge functions for the current pixel
            float w0 = edge_func(x1, y1, x2, y2, x, y);
            float w1 = edge_func(x2, y2, x0, y0, x, y);
            float w2 = edge_func(x0, y0, x1, y1, x, y);

            // If inside or on all edges, fill pixel
            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                put_pixel(x, y);
            }
        }
    }
}

int main() {
    // Define a triangle in 3D world space (all z = -2 means it's in front of the camera)
    vec3_t vertices[3] = {
        vec3_init(-0.5f, -0.5f, -2.0f),  // Bottom-left
        vec3_init( 0.5f, -0.5f, -2.0f),  // Bottom-right
        vec3_init( 0.0f,  0.5f, -2.0f)   // Top-center
    };

    // Create a projection matrix (asymmetric frustum: simulates a lens)
    mat4_t projection;
    mat4_frustum_asymmetric(&projection, -1, 1, -0.75f, 0.75f, 1, 10);

    // Project 3D vertices into normalized device coordinates (NDC), then screen space
    int sx[3], sy[3];
    printf("=== Projected Triangle (Clip Space & Screen Space) ===\n");
    for (int i = 0; i < 3; i++) {
        // Apply projection matrix (including perspective divide)
        vec3_t ndc = vec3_project(vertices[i], &projection);

        // Convert NDC (-1 to 1) → screen coordinates (0 to WIDTH/HEIGHT)
        sx[i] = (int)((ndc.x * 0.5f + 0.5f) * WIDTH);
        sy[i] = (int)((1.0f - (ndc.y * 0.5f + 0.5f)) * HEIGHT);  // Invert Y axis for image

        printf("Vertex %d → Clip: (%.4f, %.4f, %.4f) → Screen: (%d, %d)\n",
               i, ndc.x, ndc.y, ndc.z, sx[i], sy[i]);
    }

    // Fill triangle in screen space
    draw_triangle(sx[0], sy[0], sx[1], sy[1], sx[2], sy[2]);

    // Save framebuffer as a .pgm grayscale image
    FILE* out = fopen("triangle.pgm", "wb");
    fprintf(out, "P5\n%d %d\n255\n", WIDTH, HEIGHT);
    fwrite(framebuffer, sizeof(framebuffer), 1, out);
    fclose(out);

    printf("\n✅ Render complete: triangle.pgm created. View it with any image viewer.\n");
    return 0;
}
