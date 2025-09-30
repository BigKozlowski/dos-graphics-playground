#include "v_math.h"

vec3 vec3_new(fix16_t x, fix16_t y, fix16_t z) {
    vec3 v = {x, y, z};
    return v;
}

vec3 vec3_add(vec3 a, vec3 b) {
    return vec3_new(a.x + b.x, a.y + b.y, a.z + b.z);
}

vec3 vec3_sub(vec3 a, vec3 b) {
    return vec3_new(a.x - b.x, a.y - b.y, a.z - b.z);
}

vec3 vec3_scale(vec3 v, fix16_t s) {
    return vec3_new(fix16_mul(v.x, s),
                    fix16_mul(v.y, s),
                    fix16_mul(v.z, s));
}

fix16_t vec3_dot(vec3 a, vec3 b) {
    return fix16_add(
        fix16_add(fix16_mul(a.x, b.x), fix16_mul(a.y, b.y)),
        fix16_mul(a.z, b.z));
}

vec3 vec3_cross(vec3 a, vec3 b) {
    return vec3_new(
        fix16_sub(fix16_mul(a.y, b.z), fix16_mul(a.z, b.y)),
        fix16_sub(fix16_mul(a.z, b.x), fix16_mul(a.x, b.z)),
        fix16_sub(fix16_mul(a.x, b.y), fix16_mul(a.y, b.x))
    );
}

fix16_t vec3_length(vec3 v) {
    return fix16_sqrt(vec3_dot(v, v));
}

vec3 vec3_normalize(vec3 v) {
    fix16_t len = vec3_length(v);
    if (len == 0) return v;
    return vec3_scale(v, fix16_div(fix16_from_int(1), len));
}