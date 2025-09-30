#include "v_math.h"

mat4 mat4_identity(void) {
    mat4 m = {0};
    m.m[0][0] = m.m[1][1] = m.m[2][2] = m.m[3][3] = fix16_from_int(1);
    return m;
}

mat4 mat4_translation(fix16_t tx, fix16_t ty, fix16_t tz) {
    mat4 m = mat4_identity();
    m.m[0][3] = tx;
    m.m[1][3] = ty;
    m.m[2][3] = tz;
    return m;
}

mat4 mat4_scale(fix16_t sx, fix16_t sy, fix16_t sz) {
    mat4 m = {0};
    m.m[0][0] = sx;
    m.m[1][1] = sy;
    m.m[2][2] = sz;
    m.m[3][3] = fix16_from_int(1);
    return m;
}

mat4 mat4_rotation_x(fix16_t angle) {
    mat4 m = mat4_identity();
    fix16_t c = fix16_cos(angle);
    fix16_t s = fix16_sin(angle);
    m.m[1][1] = c; m.m[1][2] = -s;
    m.m[2][1] = s; m.m[2][2] = c;
    return m;
}

mat4 mat4_rotation_y(fix16_t angle) {
    mat4 m = mat4_identity();
    fix16_t c = fix16_cos(angle);
    fix16_t s = fix16_sin(angle);
    m.m[0][0] = c;  m.m[0][2] = s;
    m.m[2][0] = -s; m.m[2][2] = c;
    return m;
}

mat4 mat4_rotation_z(fix16_t angle) {
    mat4 m = mat4_identity();
    fix16_t c = fix16_cos(angle);
    fix16_t s = fix16_sin(angle);
    m.m[0][0] = c; m.m[0][1] = -s;
    m.m[1][0] = s; m.m[1][1] = c;
    return m;
}

mat4 mat4_mul(mat4 a, mat4 b) {
    mat4 r = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            fix16_t sum = 0;
            for (int k = 0; k < 4; k++) {
                sum = fix16_add(sum, fix16_mul(a.m[i][k], b.m[k][j]));
            }
            r.m[i][j] = sum;
        }
    }
    return r;
}

vec3 mat4_mul_vec3(mat4 m, vec3 v) {
    fix16_t x = fix16_add(
        fix16_add(fix16_mul(m.m[0][0], v.x), fix16_mul(m.m[0][1], v.y)),
        fix16_add(fix16_mul(m.m[0][2], v.z), m.m[0][3]));
    fix16_t y = fix16_add(
        fix16_add(fix16_mul(m.m[1][0], v.x), fix16_mul(m.m[1][1], v.y)),
        fix16_add(fix16_mul(m.m[1][2], v.z), m.m[1][3]));
    fix16_t z = fix16_add(
        fix16_add(fix16_mul(m.m[2][0], v.x), fix16_mul(m.m[2][1], v.y)),
        fix16_add(fix16_mul(m.m[2][2], v.z), m.m[2][3]));
    return vec3_new(x, y, z);
}

mat4 mat4_perspective(fix16_t fov, fix16_t aspect, fix16_t znear, fix16_t zfar) {
    mat4 m = {0};
    fix16_t f = fix16_div(fix16_tan(fix16_div(fov, fix16_from_int(2))), fix16_from_int(1));
    m.m[0][0] = fix16_div(f, aspect);
    m.m[1][1] = f;
    m.m[2][2] = fix16_div(fix16_add(zfar, znear), fix16_sub(znear, zfar));
    m.m[2][3] = fix16_div(fix16_mul(fix16_from_int(2), fix16_mul(zfar, znear)),
                          fix16_sub(znear, zfar));
    m.m[3][2] = fix16_from_int(-1);
    return m;
}

mat4 mat4_lookat(vec3 eye, vec3 center, vec3 up) {
    vec3 f = vec3_normalize(vec3_sub(center, eye));
    vec3 s = vec3_normalize(vec3_cross(f, up));
    vec3 u = vec3_cross(s, f);

    mat4 m = mat4_identity();
    m.m[0][0] = s.x; m.m[0][1] = s.y; m.m[0][2] = s.z;
    m.m[1][0] = u.x; m.m[1][1] = u.y; m.m[1][2] = u.z;
    m.m[2][0] = -f.x; m.m[2][1] = -f.y; m.m[2][2] = -f.z;

    m.m[0][3] = -vec3_dot(s, eye);
    m.m[1][3] = -vec3_dot(u, eye);
    m.m[2][3] = vec3_dot(f, eye);
    return m;
}
