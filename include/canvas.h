#ifndef CANVAS_H
#define CANVAS_H

typedef struct {
    int width, height;
    float *data;
} canvas_t;

canvas_t* canvas_create(int width, int height);
void canvas_clear(canvas_t* c, float value);
void canvas_save(canvas_t* c, int frame);
void canvas_destroy(canvas_t* c);

void set_pixel_f(canvas_t* c, float x, float y, float intensity);
void draw_line_f(canvas_t* c, float x0, float y0, float x1, float y1, float thickness);
void canvas_to_pgm(canvas_t* c, const char* filename);

#endif
