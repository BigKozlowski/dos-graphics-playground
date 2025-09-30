#include "v_math.h"

/* Конструкторы */
vec2 vec2_new(fix16_t x, fix16_t y) {
    vec2 v = {x, y};
    return v;
}

vec2 vec2_zero(void) {
    return vec2_new(0, 0);
}

vec2 vec2_unit_x(void) { return vec2_new(FIX16_ONE, 0); }
vec2 vec2_unit_y(void) { return vec2_new(0, FIX16_ONE); }

/* Арифметика */
vec2 vec2_add(vec2 a, vec2 b) {
    return vec2_new(fix16_add(a.x, b.x), fix16_add(a.y, b.y));
}

vec2 vec2_sub(vec2 a, vec2 b) {
    return vec2_new(fix16_sub(a.x, b.x), fix16_sub(a.y, b.y));
}

vec2 vec2_scale(vec2 v, fix16_t s) {
    return vec2_new(fix16_mul(v.x, s), fix16_mul(v.y, s));
}

/* Векторные операции */
fix16_t vec2_dot(vec2 a, vec2 b) {
    return fix16_add(fix16_mul(a.x, b.x), fix16_mul(a.y, b.y));
}

fix16_t vec2_cross(vec2 a, vec2 b) {
    return fix16_sub(fix16_mul(a.x, b.y), fix16_mul(a.y, b.x));
}

fix16_t vec2_length(vec2 v) {
    return fix16_sqrt(vec2_dot(v, v));
}

vec2 vec2_normalize(vec2 v) {
    fix16_t len = vec2_length(v);
    if (len == 0) return vec2_zero();
    return vec2_new(fix16_div(v.x, len), fix16_div(v.y, len));
}
