#ifndef DRAWING
#define DRAWING

#include "fix16.h"

typedef struct {
    fix16_t x0, y0;
    fix16_t x1, y1;
} line2;

typedef struct {
    line2 edges[3];
} triangle2;

typedef struct {
    int x, y;
    fix16_t z; // глубина
} screen_vertex;

void draw_w_line(line2 line, int color, char *framebuffer);

void set_video_mode(short mode);

void putpixel_13h(int x, int y, int color);

void rectangle_13h(int x, int y, int w, int h, int color);

void draw_lines(char *framebuffer);

void draw_line(line2 line, int color, char *framebuffer);

void draw_w_line(line2 line, int color, char *framebuffer);

void draw_frame();

#endif