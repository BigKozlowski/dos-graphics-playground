#include "fix16.h"
#include <math.h>

/* ======= Квадратный корень (метод Ньютона) ======= */
fix16_t fix16_sqrt(fix16_t x) {
    if (x <= 0) return 0;
    uint32_t num = (uint32_t)x;
    uint32_t res = 0;
    uint32_t bit = 1u << 30; /* самое большое число <= 2^30 */

    while (bit > num)
        bit >>= 2;

    while (bit != 0) {
        if (num >= res + bit) {
            num -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return (fix16_t)(res << 8); /* поправка для 16.16 */
}

/* ======= Таблица синуса (1024 шага на 2π) ======= */
#define SIN_TABLE_SIZE 1024
static fix16_t sin_table[SIN_TABLE_SIZE];
static int sin_table_init = 0;

static void init_sin_table(void) {
    for (int i = 0; i < SIN_TABLE_SIZE; i++) {
        float angle = (float)i * (2.0f * 3.14159265359f / SIN_TABLE_SIZE);
        sin_table[i] = fix16_from_float(sinf(angle));
    }
    sin_table_init = 1;
}

fix16_t fix16_sin(fix16_t x) {
    if (!sin_table_init) init_sin_table();
    int idx = ((int64_t)x * SIN_TABLE_SIZE / FIX16_TWO_PI) & (SIN_TABLE_SIZE - 1);
    return sin_table[idx];
}

fix16_t fix16_cos(fix16_t x) {
    return fix16_sin(x + (FIX16_PI >> 1));
}

fix16_t fix16_tan(fix16_t x) {
    return fix16_div(fix16_sin(x), fix16_cos(x));
}
