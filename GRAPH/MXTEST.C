#include <dos.h>
#include <conio.h>
#include <mem.h>
#include <alloc.h>

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 200
#define VIDEO_SEGMENT 0xA000

unsigned char far* video = (unsigned char far*)MK_FP(VIDEO_SEGMENT, 0);
unsigned char far* buffer; /* Offscreen backbuffer */

/* Draw a frame to the backbuffer */
void draw_frame(int frame_number) {
    int x, y;
    unsigned char color;

    for (y = 0; y < SCREEN_HEIGHT; y++) {
        for (x = 0; x < SCREEN_WIDTH; x++) {
            color = ((x + y + frame_number) % 256);
            buffer[y * SCREEN_WIDTH + x] = color;
        }
    }
}

/* Copy buffer to VRAM */
void present() {
    memcpy(video, buffer, SCREEN_WIDTH * SCREEN_HEIGHT);
}

void main() {
    union REGS regs;
    int frame = 0;
    unsigned long buffer_size;

    buffer_size = SCREEN_WIDTH * SCREEN_HEIGHT;
    buffer = (unsigned char far*)farmalloc(buffer_size);

    if (buffer == NULL) {
        textmode(C80);
        cprintf("Error: couldn't allocate backbuffer.\r\n");
        return;
    }

    /* Set video mode 13h (320x200x256) */
    regs.h.ah = 0x00;
    regs.h.al = 0x13;
    int86(0x10, &regs, &regs);

    while (!kbhit()) {
        draw_frame(frame);
        present();
        delay(30);
        frame++;
    }

    getch();

    /* Free buffer and return to text mode */
    farfree(buffer);
    regs.h.ah = 0x00;
    regs.h.al = 0x03;
    int86(0x10, &regs, &regs);
}
