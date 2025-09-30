#include "v_math.h"

vec4 vec4_new(fix16_t x, fix16_t y, fix16_t z, fix16_t w) {
    vec4 v = {x, y, z, w};
    return v;
}

vec4 vec4_zero(void) {
    return vec4_new(0, 0, 0, 0);
}

vec4 vec4_unit_x(void) { return vec4_new(FIX16_ONE, 0, 0, 0); }
vec4 vec4_unit_y(void) { return vec4_new(0, FIX16_ONE, 0, 0); }
vec4 vec4_unit_z(void) { return vec4_new(0, 0, FIX16_ONE, 0); }
vec4 vec4_unit_w(void) { return vec4_new(0, 0, 0, FIX16_ONE); }

/* Арифметика */
vec4 vec4_add(vec4 a, vec4 b) {
    return vec4_new(
        fix16_add(a.x, b.x),
        fix16_add(a.y, b.y),
        fix16_add(a.z, b.z),
        fix16_add(a.w, b.w)
    );
}

vec4 vec4_sub(vec4 a, vec4 b) {
    return vec4_new(
        fix16_sub(a.x, b.x),
        fix16_sub(a.y, b.y),
        fix16_sub(a.z, b.z),
        fix16_sub(a.w, b.w)
    );
}

vec4 vec4_scale(vec4 v, fix16_t s) {
    return vec4_new(
        fix16_mul(v.x, s),
        fix16_mul(v.y, s),
        fix16_mul(v.z, s),
        fix16_mul(v.w, s)
    );
}

/* Векторные операции */
fix16_t vec4_dot(vec4 a, vec4 b) {
    return fix16_add(
        fix16_add(fix16_mul(a.x, b.x), fix16_mul(a.y, b.y)),
        fix16_add(fix16_mul(a.z, b.z), fix16_mul(a.w, b.w))
    );
}

vec4 vec4_normalize(vec4 v) {
    fix16_t len = fix16_sqrt(vec4_dot(v, v));
    if (len == 0) return vec4_zero();
    return vec4_new(
        fix16_div(v.x, len),
        fix16_div(v.y, len),
        fix16_div(v.z, len),
        fix16_div(v.w, len)
    );
}

/* Умножение на матрицу 4×4 */
vec4 mat4_mul_vec4(mat4 m, vec4 v) {
    return vec4_new(
        fix16_add(
            fix16_add(
                fix16_add(fix16_mul(m.m[0][0], v.x), fix16_mul(m.m[0][1], v.y)),
                fix16_add(fix16_mul(m.m[0][2], v.z), fix16_mul(m.m[0][3], v.w))
            ), 0
        ),
        fix16_add(
            fix16_add(
                fix16_add(fix16_mul(m.m[1][0], v.x), fix16_mul(m.m[1][1], v.y)),
                fix16_add(fix16_mul(m.m[1][2], v.z), fix16_mul(m.m[1][3], v.w))
            ), 0
        ),
        fix16_add(
            fix16_add(
                fix16_add(fix16_mul(m.m[2][0], v.x), fix16_mul(m.m[2][1], v.y)),
                fix16_add(fix16_mul(m.m[2][2], v.z), fix16_mul(m.m[2][3], v.w))
            ), 0
        ),
        fix16_add(
            fix16_add(
                fix16_add(fix16_mul(m.m[3][0], v.x), fix16_mul(m.m[3][1], v.y)),
                fix16_add(fix16_mul(m.m[3][2], v.z), fix16_mul(m.m[3][3], v.w))
            ), 0
        )
    );
}