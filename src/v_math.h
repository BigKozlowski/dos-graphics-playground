#ifndef V_MATH
#define V_MATH

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 200

typedef struct {int x, y;} vector2;

typedef struct {vector2 v1, v2, v3;} triangle2;

typedef struct {vector2 start, end;} line2;

typedef struct {float x, y, z, w;} vec4;
typedef struct {float x, y, z;} vec3;
typedef struct {int x, y; float z;} screen_vertex;
typedef float matrix4[16];

int line2_calc_length(line2 line);

int sqr(int x);

int abs(int);

screen_vertex project_vertex(vec4 v, const matrix4 proj);

triangle2 make_screen_triangle(screen_vertex v1, screen_vertex v2, screen_vertex v3);

void make_perspective(matrix4 m, float fov_deg, float aspect, float znear, float zfar);

#endif