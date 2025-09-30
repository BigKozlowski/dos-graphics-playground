#include "v_math.h"
#include <math.h>

float line2_calc_length(line2 line)
{
    int square_length = sqr(line.end.x - line.start.x) + sqr(line.end.y - line.start.y);
    float length = sqrtf(square_length);
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

vec4 matrix4_mul_vec4(const matrix4 m, vec4 v)
{
    vec4 r;
    r.x = m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * v.w;
    r.y = m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * v.w;
    r.z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w;
    r.w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w;
    return r;
}

void make_perspective(matrix4 m, float fov_deg, float aspect, float znear, float zfar)
{
    float fov_rad = fov_deg * (3.14159265f / 180.0f);
    float f = 1.0f / tanf(fov_rad / 2.0f);

    // column-major
    m[0] = (f / aspect);
    m[1] = 0;
    m[2] = 0;
    m[3] = 0;

    m[4] = 0;
    m[5] = f;
    m[6] = 0;
    m[7] = 0;

    m[8] = 0;
    m[9] = 0;
    m[10] = ((zfar + znear) / (znear - zfar));
    m[11] = -1;

    m[12] = 0;
    m[13] = 0;
    m[14] = ((2 * zfar * znear) / (znear - zfar));
    m[15] = 0;
}

screen_vertex project_vertex(vec4 v, const matrix4 proj)
{
    vec4 clip = v;

    if (clip.w != 0.0f)
    {
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

line2 make_screen_line(screen_vertex v1, screen_vertex v2)
{
    line2 l;
    l.start.x = v1.x, l.start.y = v1.y;
    l.end.x = v2.x, l.end.y = v2.y;
    return l;
}

float vec3_dot(vec3 a, vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 vec3_cross(vec3 a, vec3 b)
{
    vec3 r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}

vec3 vec3_normalize(vec3 v)
{
    float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len > 0.0f)
    {
        float inv = 1.0f / len;
        v.x *= inv;
        v.y *= inv;
        v.z *= inv;
    }
    return v;
}

vec3 vec4_homogenize(vec4 v)
{
    vec3 r;
    if (v.w != 0.0f)
    {
        float inv_w = 1.0f / v.w;
        r.x = v.x * inv_w;
        r.y = v.y * inv_w;
        r.z = v.z * inv_w;
    }
    else
    {
        r.x = v.x;
        r.y = v.y;
        r.z = v.z;
    }
    return r;
}

void matrix4_mul(matrix4 out, const matrix4 a, const matrix4 b)
{
    matrix4 r;
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            r[col * 4 + row] =
                a[0 * 4 + row] * b[col * 4 + 0] +
                a[1 * 4 + row] * b[col * 4 + 1] +
                a[2 * 4 + row] * b[col * 4 + 2] +
                a[3 * 4 + row] * b[col * 4 + 3];
        }
    }
    for (int i = 0; i < 16; ++i)
        out[i] = r[i];
}

void matrix4_identity(matrix4 m) {
    for (int i = 0; i < 16; i++) m[i] = 0;
    m[0] = m[5] = m[10] = m[15] = 1;
}

void matrix4_translation(matrix4 m, float tx, float ty, float tz) {
    matrix4_identity(m);
    m[12] = tx;
    m[13] = ty;
    m[14] = tz;
}

void matrix4_scale(matrix4 m, float sx, float sy, float sz) {
    matrix4_identity(m);
    m[0] = sx; m[5] = sy; m[10] = sz;
}

void matrix4_rotation_x(matrix4 m, float a) {
    matrix4_identity(m);
    float c = cosf(a), s = sinf(a);
    m[5] = c;  m[6] = s;
    m[9] = -s; m[10] = c;
}

void matrix4_rotation_y(matrix4 m, float a) {
    matrix4_identity(m);
    float c = cosf(a), s = sinf(a);
    m[0] = c;  m[2] = -s;
    m[8] = s;  m[10] = c;
}

void matrix4_rotation_z(matrix4 m, float a) {
    matrix4_identity(m);
    float c = cosf(a), s = sinf(a);
    m[0] = c;  m[1] = s;
    m[4] = -s; m[5] = c;
}

void matrix4_lookat(matrix4 m, vec3 eye, vec3 center, vec3 up) {
    vec3 f = vec3_normalize((vec3){center.x - eye.x, center.y - eye.y, center.z - eye.z});
    vec3 s = vec3_normalize(vec3_cross(f, up));
    vec3 u = vec3_cross(s, f);

    matrix4_identity(m);

    m[0] = s.x; m[4] = s.y; m[8]  = s.z;
    m[1] = u.x; m[5] = u.y; m[9]  = u.z;
    m[2] = -f.x; m[6] = -f.y; m[10] = -f.z;

    m[12] = -vec3_dot(s, eye);
    m[13] = -vec3_dot(u, eye);
    m[14] =  vec3_dot(f, eye);
}
