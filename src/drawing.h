#ifndef DRAWING
#define DRAWING
#include "v_math.h"

void draw_w_line(line2 line, int color, char *framebuffer);

void set_video_mode(short mode);

void putpixel_13h(int x, int y, int color);

void rectangle_13h(int x, int y, int w, int h, int color);

void draw_lines(char *framebuffer);

void draw_line(line2 line, int color, char *framebuffer);

void draw_w_line(line2 line, int color, char *framebuffer);

void draw_frame();

#endif