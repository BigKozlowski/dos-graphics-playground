#ifndef V_MATH
#define V_MATH

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 200

typedef struct {int x, y;} vector2;
typedef struct {vector2 v1, v2, v3;} triangle2;
typedef struct {vector2 start, end;} line2;

typedef struct {float x, y, z;} vec3;
typedef struct {float x, y, z, w;} vec4;

typedef struct {int x, y; float z;} screen_vertex;

typedef float matrix4[16];

/* ---- vector math ---- */
float vec3_dot(vec3 a, vec3 b);
vec3  vec3_cross(vec3 a, vec3 b);
vec3  vec3_normalize(vec3 v);
vec3  vec3_lerp(vec3 a, vec3 b, float t);

/* ---- vec4 helpers ---- */
vec3  vec4_homogenize(vec4 v);

/* ---- matrix math ---- */
void  matrix4_identity(matrix4 m);
void  matrix4_mul(matrix4 out, const matrix4 a, const matrix4 b);
vec4  matrix4_mul_vec4(const matrix4 m, vec4 v);

void  matrix4_translation(matrix4 m, float tx, float ty, float tz);
void  matrix4_scale(matrix4 m, float sx, float sy, float sz);
void  matrix4_rotation_x(matrix4 m, float angle_rad);
void  matrix4_rotation_y(matrix4 m, float angle_rad);
void  matrix4_rotation_z(matrix4 m, float angle_rad);

void  matrix4_lookat(matrix4 m, vec3 eye, vec3 center, vec3 up);
void  make_perspective(matrix4 m, float fov_deg, float aspect, float znear, float zfar);

/* ---- geometry ---- */
int        sqr(int x);
float      line2_calc_length(line2 line);
vector2    lerp(line2 line, float t);
triangle2  make_screen_triangle(screen_vertex v1, screen_vertex v2, screen_vertex v3);
line2      make_screen_line(screen_vertex v1, screen_vertex v2);
screen_vertex project_vertex(vec4 v, const matrix4 proj);

#endif
