#include <dos.h>
#include <math.h>
#include <stdint.h>
#include <dpmi.h>
#include <go32.h>
#include <string.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

uint8_t sine_table[256];
uint8_t plasma[SCREEN_HEIGHT][SCREEN_WIDTH];

void set_video_mode(short mode) {
    __dpmi_regs regs = {0};
    regs.x.ax = mode;
    __dpmi_int(0x10, &regs);
}

void init_sine_table() {
    for(int i=0; i<256; i++) {
        sine_table[i] = (uint8_t)((sin(i * 2 * PI / 256) + 1.0) * 31);
    }
}

void generate_plasma(int frame) {
    for(int y=0; y<SCREEN_HEIGHT; y++) {
        for(int x=0; x<SCREEN_WIDTH; x++) {
            plasma[y][x] = (uint8_t)(
                (128 + (128 * sin(x*0.0625 + frame*0.1))) +
                (128 + (128 * sin(y*0.03125 + frame*0.1))) +
                (128 + (128 * sin((x+y)*0.05 + frame*0.1)))
            ) / 3;
        }
    }
}

void set_plasma_palette(int frame) {
    int i;
    uint8_t r, g, b;
    for(i = 0; i < 256; i++) {
        r = sine_table[(i + frame) & 255];
        g = sine_table[(i + frame + 85) & 255];
        b = sine_table[(i + frame + 170) & 255];


        outportb(0x3c8, i);
        outportb(0x3c9, r);
        outportb(0x3c9, g);
        outportb(0x3c9, b);
    }
}

void draw_plasma() {
    static char framebuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

    memset(framebuffer, 0, sizeof(framebuffer));
    for(int y = 0; y < SCREEN_HEIGHT; y++) {
        for(int x = 0; x < SCREEN_WIDTH; x++) {
            framebuffer[y*SCREEN_WIDTH + x] = plasma[y][x];
        }
    }

    dosmemput(framebuffer, SCREEN_WIDTH * SCREEN_HEIGHT, 0xA0000);
}

int main(void) {
    int frame = 0;
    init_sine_table();
    set_video_mode(0x13);
    while(!kbhit()) {
        generate_plasma(frame);
        set_plasma_palette(frame);
        draw_plasma();
        
        frame = (frame + 1) & 255;
    }
    
    set_video_mode(0x03);
    return 0;
}