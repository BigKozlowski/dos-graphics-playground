#include <dos.h>
#include <conio.h>
#include <math.h>
#include <mem.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define FOCAL_LENGTH 256

#define NUM_POINTS 8
#define NUM_EDGES 12

typedef struct { int x, y, z; } Point3D;
typedef struct { int x, y; } Point2D;

/* Cube vertex and edge definitions */
Point3D cube[NUM_POINTS] = {
    {-50, -50, -50}, { 50, -50, -50},
    { 50,  50, -50}, {-50,  50, -50},
    {-50, -50,  50}, { 50, -50,  50},
    { 50,  50,  50}, {-50,  50,  50}
};

int edges[NUM_EDGES][2] = {
    {0,1},{1,2},{2,3},{3,0},
    {4,5},{5,6},{6,7},{7,4},
    {0,4},{1,5},{2,6},{3,7}
};

/* VGA I/O ports */
#define VGA_SEQ_INDEX  0x3C4
#define VGA_SEQ_DATA   0x3C5
#define VGA_GC_INDEX   0x3CE
#define VGA_GC_DATA    0x3CF
#define VGA_CRTC_INDEX 0x3D4
#define VGA_CRTC_DATA  0x3D5

unsigned int page_offsets[2] = {0, 76800};
int current_page = 0;

void set_modeX() {
    union REGS regs;

    /* Set mode 13h first */
    regs.h.ah = 0x00;
    regs.h.al = 0x13;
    int86(0x10, &regs, &regs);

    /* Disable chain-4 (bitplane mode) */
    outportb(VGA_SEQ_INDEX, 0x04);
    outportb(VGA_SEQ_DATA, 0x06);

    outportb(VGA_SEQ_INDEX, 0x02);
    outportb(VGA_SEQ_DATA, 0x0F); /* Enable all 4 planes */

    outportb(VGA_GC_INDEX, 0x05);
    outportb(VGA_GC_DATA, 0x40); /* Disable odd/even mode */

    outportb(VGA_GC_INDEX, 0x04);
    outportb(VGA_GC_DATA, 0x00); /* Use plane 0 */

    /* Set 240 scanlines */
    outportb(VGA_CRTC_INDEX, 0x09);
    outportb(VGA_CRTC_DATA, 0x40); /* bit 6 = double scan */
}

void set_plane(int plane) {
    outportb(VGA_SEQ_INDEX, 0x02);
    outportb(VGA_SEQ_DATA, 1 << plane);
}

void putpixel_modeX(int x, int y, unsigned char color, unsigned int page) {
    unsigned long offset;
    int plane;
    unsigned char far* vram;

    offset = page + ((y * SCREEN_WIDTH + x) / 4);
    plane = x % 4;
    set_plane(plane);
    vram = (unsigned char far*)MK_FP(0xA000, offset);
    *vram = color;
}

void line_modeX(int x0, int y0, int x1, int y1, unsigned char color, unsigned int page) {
    int dx, dy, sx, sy, err, e2;

    dx = abs(x1 - x0);
    sx = x0 < x1 ? 1 : -1;
    dy = -abs(y1 - y0);
    sy = y0 < y1 ? 1 : -1;
    err = dx + dy;

    while (1) {
        putpixel_modeX(x0, y0, color, page);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void clear_page(unsigned char color, unsigned int page) {
    int plane;
    unsigned char far* vram;

    for (plane = 0; plane < 4; plane++) {
        set_plane(plane);
        vram = (unsigned char far*)MK_FP(0xA000, page / 4);
	memset(vram, color, 76800 / 4);
    }
}

void flip_page(unsigned int page) {
    unsigned int offset;

    offset = page / 4;

    outportb(VGA_CRTC_INDEX, 0x0C);
    outportb(VGA_CRTC_DATA, (offset >> 8) & 0xFF);
    outportb(VGA_CRTC_INDEX, 0x0D);
    outportb(VGA_CRTC_DATA, offset & 0xFF);
}

void project_point(Point3D p, Point2D* out) {
    if (p.z == 0) p.z = 1;
    out->x = (p.x * FOCAL_LENGTH) / (p.z + 200) + SCREEN_WIDTH / 2;
    out->y = (p.y * FOCAL_LENGTH) / (p.z + 200) + SCREEN_HEIGHT / 2;
}

void rotate_y(Point3D* p, int angle) {
    int s, c;
    int x, z;

    s = (int)(sin(angle * 3.14159 / 180.0) * 1024);
    c = (int)(cos(angle * 3.14159 / 180.0) * 1024);

    x = (p->x * c - p->z * s) / 1024;
    z = (p->x * s + p->z * c) / 1024;

    p->x = x;
    p->z = z;
}

void main() {
    union REGS regs;
    int angle;
    unsigned int draw_page;
    Point3D transformed[NUM_POINTS];
    Point2D projected[NUM_POINTS];
    int i, a, b;

    set_modeX();
    angle = 0;

    while (!kbhit()) {
        draw_page = page_offsets[current_page];
        clear_page(0, draw_page);

        for (i = 0; i < NUM_POINTS; i++) {
            transformed[i] = cube[i];
            rotate_y(&transformed[i], angle);
            project_point(transformed[i], &projected[i]);
        }

        for (i = 0; i < NUM_EDGES; i++) {
            a = edges[i][0];
            b = edges[i][1];
            line_modeX(projected[a].x, projected[a].y,
                       projected[b].x, projected[b].y,
                       15, draw_page);
        }

        flip_page(draw_page);
        current_page ^= 1;
        angle = (angle + 5) % 360;
        delay(30);
    }

    getch();

    regs.h.ah = 0x00;
    regs.h.al = 0x03; /* Return to text mode */
    int86(0x10, &regs, &regs);
}
