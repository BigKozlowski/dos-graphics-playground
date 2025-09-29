#include <dpmi.h>
#include <go32.h>
#include <sys/farptr.h>
#include <stdio.h>
#include <conio.h>
#include <sys/movedata.h>
#include <pc.h>

#include "v_math.h"
#include "drawing.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define VIDEO_SEG 0xA000

void set_color(int color, int red, int green, int blue)
{
    outportb(0x3C8, color);
    outportb(0x3C9, red);
    outportb(0x3C9, green);
    outportb(0x3C9, blue);
}

int main() {
    int x, y;
    set_video_mode(0x13); // 320x200x256

    draw_frame();

    getch(); // ждём клавишу
    set_video_mode(0x03); // вернуть текстовый режим
    return 0;
}
