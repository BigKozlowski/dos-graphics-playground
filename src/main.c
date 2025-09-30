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

screen_vertex project_vertex(vec4 v)
{
    vec4 clip;
    if (v.w != 0)
    {
        clip.x = fix16_div(v.x, v.w);
        clip.y = fix16_div(v.y, v.w);
        clip.z = fix16_div(v.z, v.w);
    }
    else
    {
        clip = v;
    }

    // Map NDC [-1..1] to screen coordinates
    screen_vertex sv;
    sv.x = fix16_to_int(fix16_mul(clip.x + FIX16_ONE, fix16_from_float(SCREEN_WIDTH / 2.0f)));
    sv.y = fix16_to_int(fix16_mul(FIX16_ONE - clip.y, fix16_from_float(SCREEN_HEIGHT / 2.0f)));
    sv.z = clip.z; // keep depth in fix16

    return sv;
}

int main() {
    int x, y;
    // set_video_mode(0x13); // 320x200x256

    // draw_frame();


    // set_video_mode(0x03); // вернуть текстовый режим

    float angle = 0.5;
    mat4 model, view, proj, mvp, temp;
    model = mat4_rotation_y(fix16_from_float(angle));
    view  = mat4_lookat(
                (vec3){fix16_from_float(0),fix16_from_float(0),fix16_from_float(5)}, 
                (vec3){0,0,0}, 
                (vec3){0,fix16_from_float(1),0});
    proj  = mat4_perspective(
                fix16_from_float(60.0f),
                fix16_from_float((float)SCREEN_WIDTH/SCREEN_HEIGHT),
                fix16_from_float(0.1f),
                fix16_from_float(100.0f));
    temp = mat4_mul(view, model);
    mvp  = mat4_mul(proj, temp);

    vec4 v = vec4_new(fix16_from_int(0), fix16_from_int(0), fix16_from_int(1), FIX16_ONE);
    screen_vertex sv = project_vertex(v);
    printf("x: %d, y: %d, z: %.3f\n", sv.x, sv.y, fix16_to_float(sv.z));
    v = vec4_new(fix16_from_int(-1), fix16_from_int(1), fix16_from_float(0.5f), FIX16_ONE);
    sv = project_vertex(v);
    printf("x: %d, y: %d, z: %.3f\n", sv.x, sv.y, fix16_to_float(sv.z));
    v = vec4_new(fix16_from_int(1), fix16_from_int(-1), fix16_from_float(0.5f), FIX16_ONE);
    sv = project_vertex(v);
    printf("x: %d, y: %d, z: %.3f\n", sv.x, sv.y, fix16_to_float(sv.z));
    v = vec4_new(fix16_from_float(0.5f), fix16_from_float(0.5f), fix16_from_float(0.25f), FIX16_ONE);
    sv = project_vertex(v);
    printf("x: %d, y: %d, z: %.3f\n", sv.x, sv.y, fix16_to_float(sv.z));


    // screen_vertex sv = project_vertex(mat4_mul_vec4(mvp, v));
    getch();
    
    return 0;
}
