#include "v_math.h"
#include <math.h>

int abs(int num)
{
    return num > 0 ? num : -num;
}

int line2_calc_length(line2 line)
{
    int square_length = sqr(line.end.x - line.start.x) + sqr(line.end.y - line.start.y);
    int length = sqrt(square_length);
    return length;
}

int sqr(int x)
{
    return x * x;
}

vector2 lerp(line2 line, float t)
{
    vector2 point;
    point.x = line.start.x + (line.end.x - line.start.x) * t;
    point.y = line.start.y + (line.end.y - line.start.y) * t;
    return point;
}


vec4 mat4_mul_vec4(const matrix4 m, vec4 v)
{
    vec4 r;
    r.x = m[0]*v.x + m[4]*v.y + m[8]*v.z + m[12]*v.w;
    r.y = m[1]*v.x + m[5]*v.y + m[9]*v.z + m[13]*v.w;
    r.z = m[2]*v.x + m[6]*v.y + m[10]*v.z + m[14]*v.w;
    r.w = m[3]*v.x + m[7]*v.y + m[11]*v.z + m[15]*v.w;
    return r;
}

void make_perspective(matrix4 m, float fov_deg, float aspect, float znear, float zfar) {
    float fov_rad = fov_deg * (3.14159265f / 180.0f);
    float f = 1.0f / tanf(fov_rad / 2.0f);

    // column-major
    m[0]  = (f / aspect);
    m[1]  = 0; m[2]  = 0; m[3]  = 0;

    m[4]  = 0; m[5]  = f; m[6]  = 0; m[7]  = 0;

    m[8]  = 0; m[9]  = 0;
    m[10] = ((zfar+znear)/(znear-zfar));
    m[11] = -1;

    m[12] = 0; m[13] = 0;
    m[14] = ((2*zfar*znear)/(znear-zfar));
    m[15] = 0;
}

screen_vertex project_vertex(vec4 v, const matrix4 proj) {
    // Умножаем на матрицу
    vec4 clip = mat4_mul_vec4(proj, v);

    // Деление на w
    if (clip.w != 0.0f) {
        clip.x /= clip.w;
        clip.y /= clip.w;
        clip.z /= clip.w;
    }

    // NDC [-1..1] -> экранные координаты
    screen_vertex sv;
    sv.x = (int)((clip.x + 1.0f) * 0.5f * SCREEN_WIDTH);
    sv.y = (int)((1.0f - clip.y) * 0.5f * SCREEN_HEIGHT);
    sv.z = (clip.z + 1.0f) * 0.5f; // глубина 0..1
    return sv;
}

triangle2 make_screen_triangle(screen_vertex v1, screen_vertex v2, screen_vertex v3)
{
    triangle2 tri;
    tri.v1.x = v1.x, tri.v2.x = v2.x, tri.v3.x = v3.x;
    tri.v1.y = v1.y, tri.v2.y = v2.y, tri.v3.y = v3.y;
    return tri;
}


