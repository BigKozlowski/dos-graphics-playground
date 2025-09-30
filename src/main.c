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
    // set_video_mode(0x13); // 320x200x256

    // draw_frame();


    // set_video_mode(0x03); // вернуть текстовый режим

    float angle = 0.5;
    matrix4 model, view, proj, mvp;
    matrix4_rotation_y(model, angle);
    matrix4_lookat(view, (vec3){0,0,5}, (vec3){0,0,0}, (vec3){0,1,0});
    make_perspective(proj, 60.0f, (float)SCREEN_WIDTH/SCREEN_HEIGHT, 0.1f, 100.0f);

    matrix4 temp;
    matrix4_mul(temp, view, model);  // view * model
    matrix4_mul(mvp, proj, temp);    // proj * view * model

    vec4 v = {1, 1, 1, 1};
    screen_vertex sv = project_vertex(matrix4_mul_vec4(mvp, v), proj);

    printf("x: %d, y: %d, z: %.3f\n", sv.x, sv.y, sv.z);
    getch();

    return 0;
}
