// v_math.h
#ifndef V_MATH_H
#define V_MATH_H

#include "fix16.h"

typedef struct {
    fix16_t m[4][4];
} mat4;
typedef struct {
    fix16_t x, y;
} vec2;
typedef struct {
    fix16_t x, y, z;
} vec3;
typedef struct {
    fix16_t x, y, z, w;
} vec4;
typedef struct {
    fix16_t x, y, z, w;
} quat;

/* mat4 */
// Конструкторы
mat4 mat4_identity(void);
mat4 mat4_translation(fix16_t tx, fix16_t ty, fix16_t tz);
mat4 mat4_scale(fix16_t sx, fix16_t sy, fix16_t sz);
mat4 mat4_rotation_x(fix16_t angle);
mat4 mat4_rotation_y(fix16_t angle);
mat4 mat4_rotation_z(fix16_t angle);

// Операции
mat4 mat4_mul(mat4 a, mat4 b);
vec3 mat4_mul_vec3(mat4 m, vec3 v);
vec4 mat4_mul_vec4(mat4 m, vec4 v);

// Камера и проекция
mat4 mat4_perspective(fix16_t fov, fix16_t aspect, fix16_t znear, fix16_t zfar);
mat4 mat4_lookat(vec3 eye, vec3 center, vec3 up);

/* vec2 */
/* Конструкторы */
vec2 vec2_new(fix16_t x, fix16_t y);
vec2 vec2_zero(void);
vec2 vec2_unit_x(void);
vec2 vec2_unit_y(void);

/* Арифметика */
vec2 vec2_add(vec2 a, vec2 b);
vec2 vec2_sub(vec2 a, vec2 b);
vec2 vec2_scale(vec2 v, fix16_t s);

/* Векторные операции */
fix16_t vec2_dot(vec2 a, vec2 b);
fix16_t vec2_cross(vec2 a, vec2 b); /* псевдоскаляр */
fix16_t vec2_length(vec2 v);
vec2 vec2_normalize(vec2 v);

/* vec3 */ 
// Базовые конструкторы
vec3 vec3_new(fix16_t x, fix16_t y, fix16_t z);

// Арифметика
vec3 vec3_add(vec3 a, vec3 b);
vec3 vec3_sub(vec3 a, vec3 b);
vec3 vec3_scale(vec3 v, fix16_t s);

// Векторные операции
fix16_t vec3_dot(vec3 a, vec3 b);
vec3 vec3_cross(vec3 a, vec3 b);

// Длина и нормализация
fix16_t vec3_length(vec3 v);
vec3 vec3_normalize(vec3 v);

/* vec4 */
/* Конструкторы */
vec4 vec4_new(fix16_t x, fix16_t y, fix16_t z, fix16_t w);
vec4 vec4_zero(void);
vec4 vec4_unit_x(void);
vec4 vec4_unit_y(void);
vec4 vec4_unit_z(void);
vec4 vec4_unit_w(void);

/* Арифметика */
vec4 vec4_add(vec4 a, vec4 b);
vec4 vec4_sub(vec4 a, vec4 b);
vec4 vec4_scale(vec4 v, fix16_t s);

/* Векторные операции */
fix16_t vec4_dot(vec4 a, vec4 b);
vec4 vec4_normalize(vec4 v);

/* Умножение на матрицу */
vec4 mat4_mul_vec4(mat4 m, vec4 v);

/* quat */
/* Конструкторы */
quat quat_new(fix16_t x, fix16_t y, fix16_t z, fix16_t w);
quat quat_identity(void);
quat quat_from_axis_angle(vec3 axis, fix16_t angle);

/* Операции */
quat quat_mul(quat a, quat b);
quat quat_normalize(quat q);

/* Применение */
vec3 quat_rotate_vec3(quat q, vec3 v);
mat4 quat_to_mat4(quat q);

#endif
