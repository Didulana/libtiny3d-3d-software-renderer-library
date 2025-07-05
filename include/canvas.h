#ifndef CANVAS_H
#define CANVAS_H

typedef struct {
    int width;
    int height;
    float* pixels;
} canvas_t;

void canvas_init(canvas_t* c, int width, int height);
void canvas_clear(canvas_t* c, float val);
void canvas_write_to_pgm(canvas_t* c, const char* filename);
void canvas_free(canvas_t* c);
void draw_line_f(canvas_t* c, float x0, float y0, float x1, float y1, float thickness);

#endif
