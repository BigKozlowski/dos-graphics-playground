#ifndef FIX16
#define FIX16

#include <stdint.h>

typedef int32_t fix16_t;

#define FIX16_ONE   ((fix16_t)0x00010000)   /* 1.0 в формате 16.16 */
#define FIX16_HALF  ((fix16_t)0x00008000)   /* 0.5 */
#define FIX16_PI    ((fix16_t)205887)       /* ~3.1415926 * 65536 */
#define FIX16_TWO_PI ((fix16_t)411775)      /* 2π */

/* Конвертация */
static inline fix16_t fix16_from_int(int32_t i) { return i << 16; }
static inline int32_t fix16_to_int(fix16_t f)   { return f >> 16; }
static inline fix16_t fix16_from_float(float x) { return (fix16_t)(x * 65536.0f); }
static inline float   fix16_to_float(fix16_t f) { return (float)f / 65536.0f; }

/* Арифметика */
static inline fix16_t fix16_add(fix16_t a, fix16_t b) { return a + b; }
static inline fix16_t fix16_sub(fix16_t a, fix16_t b) { return a - b; }
static inline fix16_t fix16_mul(fix16_t a, fix16_t b) { return (fix16_t)(((int64_t)a * (int64_t)b) >> 16); }
static inline fix16_t fix16_div(fix16_t a, fix16_t b) { return (fix16_t)(((int64_t)a << 16) / b); }

/* Сравнение */
static inline int fix16_eq(fix16_t a, fix16_t b)  { return a == b; }
static inline int fix16_lt(fix16_t a, fix16_t b)  { return a < b; }
static inline int fix16_gt(fix16_t a, fix16_t b)  { return a > b; }

/* Тригонометрия */
fix16_t fix16_sqrt(fix16_t x);
fix16_t fix16_sin(fix16_t x);
fix16_t fix16_cos(fix16_t x);
fix16_t fix16_tan(fix16_t x);

#endif
