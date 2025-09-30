#include "v_math.h"

/* Создание */
quat quat_new(fix16_t x, fix16_t y, fix16_t z, fix16_t w) {
    quat q = {x, y, z, w};
    return q;
}

quat quat_identity(void) {
    return quat_new(0, 0, 0, FIX16_ONE);
}

quat quat_from_axis_angle(vec3 axis, fix16_t angle) {
    vec3 n = vec3_normalize(axis);
    fix16_t half = fix16_div(angle, fix16_from_int(2));
    fix16_t s = fix16_sin(half);
    fix16_t c = fix16_cos(half);
    return quat_new(
        fix16_mul(n.x, s),
        fix16_mul(n.y, s),
        fix16_mul(n.z, s),
        c
    );
}

/* Умножение (сочетание вращений) */
quat quat_mul(quat a, quat b) {
    return quat_new(
        fix16_add(
            fix16_add(fix16_mul(a.w, b.x), fix16_mul(a.x, b.w)),
            fix16_sub(fix16_mul(a.y, b.z), fix16_mul(a.z, b.y))
        ),
        fix16_add(
            fix16_add(fix16_mul(a.w, b.y), fix16_mul(a.y, b.w)),
            fix16_sub(fix16_mul(a.z, b.x), fix16_mul(a.x, b.z))
        ),
        fix16_add(
            fix16_add(fix16_mul(a.w, b.z), fix16_mul(a.z, b.w)),
            fix16_sub(fix16_mul(a.x, b.y), fix16_mul(a.y, b.x))
        ),
        fix16_sub(
            fix16_sub(
                fix16_sub(fix16_mul(a.w, b.w), fix16_mul(a.x, b.x)),
                fix16_mul(a.y, b.y)),
            fix16_mul(a.z, b.z)
        )
    );
}

/* Нормализация */
quat quat_normalize(quat q) {
    fix16_t mag = fix16_sqrt(
        fix16_add(
            fix16_add(fix16_mul(q.x, q.x), fix16_mul(q.y, q.y)),
            fix16_add(fix16_mul(q.z, q.z), fix16_mul(q.w, q.w))
        )
    );
    if (mag == 0) return quat_identity();
    return quat_new(
        fix16_div(q.x, mag),
        fix16_div(q.y, mag),
        fix16_div(q.z, mag),
        fix16_div(q.w, mag)
    );
}

/* Вращение вектора */
vec3 quat_rotate_vec3(quat q, vec3 v) {
    quat p = quat_new(v.x, v.y, v.z, 0);
    quat q_conj = quat_new(-q.x, -q.y, -q.z, q.w);
    quat r = quat_mul(quat_mul(q, p), q_conj);
    return vec3_new(r.x, r.y, r.z);
}

/* Конвертация в матрицу 4x4 */
mat4 quat_to_mat4(quat q) {
    q = quat_normalize(q);
    fix16_t xx = fix16_mul(q.x, q.x);
    fix16_t yy = fix16_mul(q.y, q.y);
    fix16_t zz = fix16_mul(q.z, q.z);
    fix16_t xy = fix16_mul(q.x, q.y);
    fix16_t xz = fix16_mul(q.x, q.z);
    fix16_t yz = fix16_mul(q.y, q.z);
    fix16_t wx = fix16_mul(q.w, q.x);
    fix16_t wy = fix16_mul(q.w, q.y);
    fix16_t wz = fix16_mul(q.w, q.z);

    mat4 m = mat4_identity();
    m.m[0][0] = FIX16_ONE - fix16_mul(fix16_from_int(2), fix16_add(yy, zz));
    m.m[0][1] = fix16_mul(fix16_from_int(2), fix16_sub(xy, wz));
    m.m[0][2] = fix16_mul(fix16_from_int(2), fix16_add(xz, wy));

    m.m[1][0] = fix16_mul(fix16_from_int(2), fix16_add(xy, wz));
    m.m[1][1] = FIX16_ONE - fix16_mul(fix16_from_int(2), fix16_add(xx, zz));
    m.m[1][2] = fix16_mul(fix16_from_int(2), fix16_sub(yz, wx));

    m.m[2][0] = fix16_mul(fix16_from_int(2), fix16_sub(xz, wy));
    m.m[2][1] = fix16_mul(fix16_from_int(2), fix16_add(yz, wx));
    m.m[2][2] = FIX16_ONE - fix16_mul(fix16_from_int(2), fix16_add(xx, yy));

    return m;
}
