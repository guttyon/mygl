#include <string.h>
#include <assert.h>
#include <math.h>
#include "basetype.h"
#include "matstack.h"

// model2world
static mat44d stack_world[30];
static int cur_world = 0;

// world2view
static mat44d stack_view[30];
static int cur_view = 0;

// view2proj
static mat44d stack_proj[30];
static int cur_proj = 0;

static mat44d ident = {
  1.0, 0.0, 0.0, 0.0,
  0.0, 1.0, 0.0, 0.0,
  0.0, 0.0, 1.0, 0.0,
  0.0, 0.0, 0.0, 1.0,
};

static E_MATMODE cur_matmode = MATMODE_WORLD;


void matmode(E_MATMODE mode)
{
  cur_matmode = mode;
}

void loadidentity()
{
  switch(cur_matmode)
    {
    case MATMODE_WORLD:
      memcpy(stack_world[cur_world], ident, sizeof(mat44d));
      break;
    case MATMODE_VIEW:
      memcpy(stack_view[cur_view], ident, sizeof(mat44d));
      break;
    case MATMODE_PROJ:
      memcpy(stack_proj[cur_proj], ident, sizeof(mat44d));
      break;
    }
}

void pushmat()
{
  switch(cur_matmode)
    {
    case MATMODE_WORLD:
      memcpy(stack_world[cur_world + 1], stack_world[cur_world], sizeof(mat44d));
      ++cur_world;
      assert(cur_world < 30);
      break;
    case MATMODE_VIEW:
      memcpy(stack_view[cur_view + 1], stack_view[cur_view], sizeof(mat44d));
      ++cur_view;
      assert(cur_view < 30);
      break;
    case MATMODE_PROJ:
      memcpy(stack_proj[cur_proj + 1], stack_proj[cur_proj], sizeof(mat44d));
      ++cur_proj;
      assert(cur_proj < 30);
      break;
    }
}

void popmat()
{
  switch(cur_matmode)
    {
    case MATMODE_WORLD:
      assert(cur_world > 0);
      --cur_world;
      break;
    case MATMODE_VIEW:
      assert(cur_view > 0);
      --cur_view;
      break;
    case MATMODE_PROJ:
      assert(cur_proj > 0);
      --cur_proj;
      break;
    }
}

void setmat(const mat44d* mat)
{
  switch(cur_matmode)
    {
    case MATMODE_WORLD:
      memcpy(stack_world[cur_world], mat, sizeof(mat44d));
      break;
    case MATMODE_VIEW:
      memcpy(stack_view[cur_view], mat, sizeof(mat44d));
      break;
    case MATMODE_PROJ:
      memcpy(stack_proj[cur_proj], mat, sizeof(mat44d));
      break;
    }
}

void getmat(mat44d* mat)
{
  switch(cur_matmode)
    {
    case MATMODE_WORLD:
      memcpy(mat, stack_world[cur_world], sizeof(mat44d));
      break;
    case MATMODE_VIEW:
      memcpy(mat, stack_view[cur_view], sizeof(mat44d));
      break;
    case MATMODE_PROJ:
      memcpy(mat, stack_proj[cur_proj], sizeof(mat44d));
      break;
    }
}

void add_m33_m33(mat33d* pdst, const mat33d* pa, const mat33d* pb)
{
  double* dst = (double*)pdst;
  const double* a = (double*)pa;
  const double* b = (double*)pb;
  for(int i = 0; i < 9; ++i)
    {
      dst[i] = a[i] + b[i];
    }
  return;
}
void add_m44_m44(mat44d* pdst, const mat44d* pa, const mat44d* pb)
{
  double* dst = (double*)pdst;
  const double* a = (double*)pa;
  const double* b = (double*)pb;
  for(int i = 0; i < 16; ++i)
    {
      dst[i] = a[i] + b[i];
    }
  return;
}
void sub_m33_m33(mat33d* pdst, const mat33d* pa, const mat33d* pb)
{
  double* dst = (double*)pdst;
  const double* a = (double*)pa;
  const double* b = (double*)pb;
  for(int i = 0; i < 9; ++i)
    {
      dst[i] = a[i] - b[i];
    }
  return;
}
void sub_m44_m44(mat44d* pdst, const mat44d* pa, const mat44d* pb)
{
  double* dst = (double*)pdst;
  const double* a = (double*)pa;
  const double* b = (double*)pb;
  for(int i = 0; i < 16; ++i)
    {
      dst[i] = a[i] - b[i];
    }
  return;
}


void mul_m33_m33(mat33d* pdst, const mat33d* pa, const mat33d* pb)
{
  double* dst = (double*)pdst;
  const double* a = (double*)pa;
  const double* b = (double*)pb;
  for(int i = 0; i < 3; ++i)
    {
      int k = 3 * i;
      dst[ 0 + i] = a[ 0] * b[k + 0] + a[ 1] * b[k + 1] + a[ 2] * b[k + 2];
      dst[ 3 + i] = a[ 3] * b[k + 0] + a[ 4] * b[k + 1] + a[ 5] * b[k + 2];
      dst[ 6 + i] = a[ 6] * b[k + 0] + a[ 7] * b[k + 1] + a[ 8] * b[k + 2];
    }
  return;
}
void mul_m44_m44(mat44d* pdst, const mat44d* pa, const mat44d* pb)
{
  double* dst = (double*)pdst;
  const double* a = (double*)pa;
  const double* b = (double*)pb;
  for(int i = 0; i < 4; ++i)
    {
      int k = 4 * i;
      dst[ 0 + i] = a[ 0] * b[k + 0] + a[ 1] * b[k + 1] + a[ 2] * b[k + 2] + a[ 3] * b[k + 3];
      dst[ 4 + i] = a[ 4] * b[k + 0] + a[ 5] * b[k + 1] + a[ 6] * b[k + 2] + a[ 7] * b[k + 3];
      dst[ 8 + i] = a[ 8] * b[k + 0] + a[ 9] * b[k + 1] + a[10] * b[k + 2] + a[11] * b[k + 3];
      dst[12 + i] = a[12] * b[k + 0] + a[13] * b[k + 1] + a[14] * b[k + 2] + a[15] * b[k + 3];
    }
  return;
}

// 右にかけていく。
void mulmat(const mat44d* m)
{
  mat44d tmp;
  switch(cur_matmode)
    {
    case MATMODE_WORLD:
      mul_m44_m44(&tmp, &stack_world[cur_world], m);
      memcpy(stack_world[cur_world], &tmp, sizeof(mat44d));
      break;
    case MATMODE_VIEW:
      mul_m44_m44(&tmp, &stack_view[cur_view], m);
      memcpy(stack_view[cur_view], &tmp, sizeof(mat44d));
      break;
    case MATMODE_PROJ:
      mul_m44_m44(&tmp, &stack_proj[cur_proj], m);
      memcpy(stack_proj[cur_proj], &tmp, sizeof(mat44d));
      break;
    }
}

void transpose(mat44d* pdst, const mat44d* pa)
{
  double* dst = (double*)pdst;
  const double* a = (double*)pa;
  for(int i = 0; i < 4; ++i)
    {
      int k = 4 * i;
      dst[ 0 + i] = a[ 0 + k];
      dst[ 4 + i] = a[ 1 + k];
      dst[ 8 + i] = a[ 2 + k];
      dst[12 + i] = a[ 3 + k];
    }
}

// ゲームプログラミングのための3Dグラフィックス数学
// p49
double determinant(const mat22d* pa)
{
  double* a = (double*)pa;
  return a[0] * a[3] - a[1] * a[2];
}

// p49
double determinant(const mat33d* pa)
{
  double* a = (double*)pa;
  return 
    a[0] * (a[4] * a[8] - a[5] * a[7]) 
    - a[1] * (a[3] * a[8] - a[5] * a[6])
    + a[2] * (a[3] * a[7] - a[4] * a[6]);
}
// p50
void inverse(mat22d* pdst, const mat22d* pa)
{
  double det = determinant(pa);
  double* dst = (double*)pdst;
  const double* a = (double*)pa;
  dst[0] = a[3] / det;
  dst[1] = -a[1] / det;
  dst[2] = -a[2] / det;
  dst[3] = a[0] / det;

}
// p50
void inverse(mat33d* pdst, const mat33d* pa)
{
  double det = determinant(pa);
  double* dst = (double*)pdst;
  const double* a = (double*)pa;
  dst[0] = (a[4* 1 + 1] * a[4* 2 + 2] - a[4* 1 + 2] * a[4* 2 + 1]) / det;
  dst[1] = (a[4* 0 + 2] * a[4* 2 + 1] - a[4* 0 + 1] * a[4* 2 + 2]) / det;
  dst[2] = (a[4* 0 + 1] * a[4* 1 + 2] - a[4* 0 + 2] * a[4* 1 + 1]) / det;

  dst[4] = (a[4* 1 + 2] * a[4* 2 + 0] - a[4* 1 + 0] * a[4* 2 + 2]) / det;
  dst[5] = (a[4* 0 + 0] * a[4* 2 + 2] - a[4* 0 + 2] * a[4* 2 + 0]) / det;
  dst[6] = (a[4* 0 + 2] * a[4* 1 + 0] - a[4* 0 + 0] * a[4* 1 + 2]) / det;

  dst[7] = (a[4* 1 + 0] * a[4* 2 + 1] - a[4* 1 + 1] * a[4* 2 + 0]) / det;
  dst[8] = (a[4* 0 + 1] * a[4* 2 + 0] - a[4* 0 + 0] * a[4* 2 + 1]) / det;
  dst[9] = (a[4* 0 + 0] * a[4* 1 + 1] - a[4* 0 + 1] * a[4* 1 + 0]) / det;
}
// p63, p89
// 一般の4x4ではなく、モデル変換（移動、拡大、回転）だけの4x4の変換。
// 平面の変換に使う。
// p67
// 法線の変換は、w=0にして、(M^-1)^Tで変換。
void world_inverse()
{
  // TODO:
}

// p77
// 位置ベクトルはw=1
// p64
// 方向ベクトルはw=0
// p83, 85
// 平面の表現は、ax + by + cz + d = 0とし、(a, b, c, d)とする。


// p76
void scale(float x, float y, float z)
{
  mat44d m = {
    (double)x, 0., 0., 0.,
    0., (double)y, 0., 0.,
    0., 0., (double)z, 0.,
    0., 0., 0., 1.,
  };
  mulmat(&m);
}

void translate(float x, float y, float z)
{
  mat44d m = {
    1., 0., 0., (double)x,
    0., 1., 0., (double)y,
    0., 0., 1., (double)z,
    0., 0., 0., 1.,
  };
  mulmat(&m);
}

void rotate_x(float theta)
{
  double s = sin(theta);
  double c = cos(theta);
  mat44d m = {
    1., 0., 0., 0.,
    0., c, -s,  0.,
    0., s,  c,  0.,
    0., 0., 0., 1.,
  };
  mulmat(&m);
}
void rotate_y(float theta)
{
  double s = sin(theta);
  double c = cos(theta);
  mat44d m = {
    c,  0., s,  0.,
    0., 1., 0., 0.,
    -s, 0., c,  0.,
    0., 0., 0., 1.,
  };
  mulmat(&m);
}
void rotate_z(float theta)
{
  double s = sin(theta);
  double c = cos(theta);
  mat44d m = {
    c, -s,  0., 0.,
    s, c,  0., 0.,
    0., 0., 1., 0.,
    0., 0., 0., 1.,
  };
  mulmat(&m);
}

// p76
void rotate(float x, float y, float z, float theta)
{
  double s = sin(theta);
  double c = cos(theta);
  double xy = x * y;
  double yz = y * z;
  double zx = z * x;
  double xx = x * x;
  double yy = y * y;
  double zz = z * z;

  double xs = x * s;
  double ys = y * s;
  double zs = z * s;

  double ic = 1. - c;

  double t0 = xy * ic;
  double t1 = zx * ic;
  double t2 = yz * ic;

  mat44d m = {
    c + xx * ic, t0 - zs,     t1 + ys,     0.,
    t0 + zs,     c + yy * ic, t2 - xs,     0.,
    t1 - yz,     t2 + xs,     c + zz * ic, 0.,
    0.,          0.,          0.,          1.,
  };
  mulmat(&m);
}

// quartenion
// p77


