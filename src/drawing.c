#include "v_math.h"
#include <dpmi.h>
#include <go32.h>
#include <dos.h>
#include <string.h>
#include <stdlib.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

void set_video_mode(short mode) {
    __dpmi_regs regs = {0};
    regs.x.ax = mode;
    __dpmi_int(0x10, &regs);
}

void putpixel(int x, int y, int color, char *framebuffer) {
    if(x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
    framebuffer[y * SCREEN_WIDTH + x] = color;
}

void draw_rect(int x, int y, int w, int h, int color, char *framebuffer) {
    int i, j;
    for(i = y; i < y + h; i++) {
        if(i < 0 || i >= SCREEN_HEIGHT) continue;
        for(j = x; j < x + w; j++) {
            if(j < 0 || j >= SCREEN_WIDTH) continue;
            framebuffer[i * SCREEN_WIDTH + j] = color;
        }
    }
}

// Bresenham line drawing
void draw_line(line2 line, int color, char *framebuffer) {
    int x0 = line.start.x;
    int y0 = line.start.y;
    int x1 = line.end.x;
    int y1 = line.end.y;

    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy; // err = dx + dy

    while(1) {
        putpixel(x0, y0, color, framebuffer);
        if(x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if(e2 >= dy) { err += dy; x0 += sx; }
        if(e2 <= dx) { err += dx; y0 += sy; }
    }
}

void draw_triangle(triangle2 tri, int color, char* framebuffer)
{
    line2 l1 = {.start = tri.v1, .end = tri.v2};
    line2 l2 = {.start = tri.v2, .end = tri.v3};
    line2 l3 = {.start = tri.v3, .end = tri.v1};
    draw_line(l1, color, framebuffer);
    draw_line(l2, color, framebuffer);
    draw_line(l3, color, framebuffer);
}

void swap_int(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
}

// Sort vertices by y
void sort_triangle_vertices(triangle2 *t) {
    if (t->v1.y > t->v2.y) { swap_int(&t->v1.y, &t->v2.y); swap_int(&t->v1.x, &t->v2.x); }
    if (t->v2.y > t->v3.y) { swap_int(&t->v2.y, &t->v3.y); swap_int(&t->v2.x, &t->v3.x); }
    if (t->v1.y > t->v2.y) { swap_int(&t->v1.y, &t->v2.y); swap_int(&t->v1.x, &t->v2.x); }
}

// Draw horizontal line with clipping
void hline(int x0, int x1, int y, int color, char* framebuffer) {
    if(y < 0 || y >= SCREEN_HEIGHT) return;
    if(x0 > x1) swap_int(&x0, &x1);
    if(x1 < 0 || x0 >= SCREEN_WIDTH) return;
    if(x0 < 0) x0 = 0;
    if(x1 >= SCREEN_WIDTH) x1 = SCREEN_WIDTH - 1;
    for(int x = x0; x <= x1; x++) {
        framebuffer[y * SCREEN_WIDTH + x] = color;
    }
}

void draw_filled_triangle(triangle2 *tri, int color, char* framebuffer)
{
    sort_triangle_vertices(tri);

    int x1 = tri->v1.x, y1 = tri->v1.y;
    int x2 = tri->v2.x, y2 = tri->v2.y;
    int x3 = tri->v3.x, y3 = tri->v3.y;

    float dx1 = (y2 - y1) ? (float)(x2 - x1) / (y2 - y1) : 0;
    float dx2 = (y3 - y1) ? (float)(x3 - x1) / (y3 - y1) : 0;
    float dx3 = (y3 - y2) ? (float)(x3 - x2) / (y3 - y2) : 0;

    float sx = (float)x1, ex = (float)x1;

    // Top half
    for(int y = y1; y <= y2; y++) {
        hline((int)sx, (int)ex, y, color, framebuffer);
        sx += dx1; ex += dx2;
    }

    sx = (float)x2;
    // Bottom half
    for(int y = y2; y <= y3; y++) {
        hline((int)sx, (int)ex, y, color, framebuffer);
        sx += dx3; ex += dx2;
    }
}

void draw_frame() {
    matrix4 proj;
    make_perspective(proj, 90.0f, (float)SCREEN_WIDTH/SCREEN_HEIGHT, 0.1f, 100.0f);
    static char framebuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

    memset(framebuffer, 0, sizeof(framebuffer));

    vec4 v1 = { 1, 0, -10, 1 };
    vec4 v2 = { 0, 0, -10, 1 };
    vec4 v3 = { 0, 1, -10, 1 };

    screen_vertex s1 = project_vertex(v1, proj);
    screen_vertex s2 = project_vertex(v2, proj);
    screen_vertex s3 = project_vertex(v3, proj);

    triangle2 tri = make_screen_triangle(s1, s2, s3);

    draw_filled_triangle(&tri, 12, framebuffer);

    dosmemput(framebuffer, SCREEN_WIDTH * SCREEN_HEIGHT, 0xA0000);
}
