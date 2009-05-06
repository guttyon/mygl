#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

// texsture
static mat44d stack_texsture[30];
static int cur_texsture = 0;

static mat22d ident22d = {
  1.0, 0.0,
  0.0, 1.0,
};
static mat33d ident33d = {
  1.0, 0.0, 0.0,
  0.0, 1.0, 0.0,
  0.0, 0.0, 1.0,
};
static mat44d ident44d = {
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
    memcpy(stack_world[cur_world], ident44d, sizeof(mat44d));
    break;
  case MATMODE_VIEW:
    memcpy(stack_view[cur_view], ident44d, sizeof(mat44d));
    break;
  case MATMODE_PROJ:
    memcpy(stack_proj[cur_proj], ident44d, sizeof(mat44d));
    break;
  case MATMODE_TEXTURE:
    memcpy(stack_texsture[cur_texsture], ident44d, sizeof(mat44d));
    break;
  }
}
void loadidentity(mat22d* m)
{
  memcpy(m, ident22d, sizeof(mat22d));
}
void loadidentity(mat33d* m)
{
  memcpy(m, ident33d, sizeof(mat33d));
}
void loadidentity(mat44d* m)
{
  memcpy(m, ident44d, sizeof(mat44d));
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
  case MATMODE_TEXTURE:
    memcpy(stack_texsture[cur_texsture + 1], stack_texsture[cur_texsture], sizeof(mat44d));
    ++cur_texsture;
    assert(cur_texsture < 30);
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
  case MATMODE_TEXTURE:
    assert(cur_texsture > 0);
    --cur_texsture;
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
  case MATMODE_TEXTURE:
    memcpy(stack_texsture[cur_texsture], mat, sizeof(mat44d));
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
  case MATMODE_TEXTURE:
    memcpy(mat, stack_texsture[cur_texsture], sizeof(mat44d));
    break;
  }
}

void getmodel2perspective(mat44d* m)
{
  mat44d tmp;
  mmul(&tmp, &stack_view[cur_view], &stack_world[cur_world]);
  mmul(m, &stack_proj[cur_proj], &tmp);
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

void mmul(mat33d* pdst, const mat33d* pa, const mat33d* pb)
{
  assert(pdst != pa && pdst != pb);
  double* dst = (double*)pdst;
  const double* a = (double*)pa;
  const double* b = (double*)pb;
  for(int i = 0; i < 3; ++i)
  {
    dst[ 0 + i] = a[ 0] * b[i + 0] + a[ 1] * b[i + 3] + a[ 2] * b[i + 6];
    dst[ 3 + i] = a[ 3] * b[i + 0] + a[ 4] * b[i + 3] + a[ 5] * b[i + 6];
    dst[ 6 + i] = a[ 6] * b[i + 0] + a[ 7] * b[i + 3] + a[ 8] * b[i + 6];
  }
  return;
}
void mmul(mat44d* pdst, const mat44d* pa, const mat44d* pb)
{
  assert(pdst != pa && pdst != pb);
  double* dst = (double*)pdst;
  const double* a = (double*)pa;
  const double* b = (double*)pb;
  for(int i = 0; i < 4; ++i)
  {
    dst[ 0 + i] = a[ 0] * b[i + 0] + a[ 1] * b[i + 4] + a[ 2] * b[i + 8] + a[ 3] * b[i + 12];
    dst[ 4 + i] = a[ 4] * b[i + 0] + a[ 5] * b[i + 4] + a[ 6] * b[i + 8] + a[ 7] * b[i + 12];
    dst[ 8 + i] = a[ 8] * b[i + 0] + a[ 9] * b[i + 4] + a[10] * b[i + 8] + a[11] * b[i + 12];
    dst[12 + i] = a[12] * b[i + 0] + a[13] * b[i + 4] + a[14] * b[i + 8] + a[15] * b[i + 12];
  }
  return;
}
void mvmul(v3d* pdst, const mat33d* pa, const v3d* pb)
{
  assert(pdst != pb);
  double* dst = (double*)pdst;
  const double* a = (double*)pa;
  const double* b = (double*)pb;
  dst[0] = a[ 0] * b[0] + a[ 1] * b[1] + a[ 2] * b[2];
  dst[1] = a[ 3] * b[0] + a[ 4] * b[1] + a[ 5] * b[2];
  dst[2] = a[ 6] * b[0] + a[ 7] * b[1] + a[ 8] * b[2];
  return;
}
void mvmul(v4d* pdst, const mat44d* pa, const v4d* pb)
{
  assert(pdst != pb);
  double* dst = (double*)pdst;
  const double* a = (double*)pa;
  const double* b = (double*)pb;
  dst[0] = a[ 0] * b[0] + a[ 1] * b[1] + a[ 2] * b[2] + a[ 3] * b[3];
  dst[1] = a[ 4] * b[0] + a[ 5] * b[1] + a[ 6] * b[2] + a[ 7] * b[3];
  dst[2] = a[ 8] * b[0] + a[ 9] * b[1] + a[10] * b[2] + a[11] * b[3];
  dst[3] = a[12] * b[0] + a[13] * b[1] + a[14] * b[2] + a[15] * b[3];
  return;
}

// 右にかけていく。
void mulmat(const mat44d* m)
{
  mat44d tmp;
  switch(cur_matmode)
  {
  case MATMODE_WORLD:
    mmul(&tmp, &stack_world[cur_world], m);
    memcpy(stack_world[cur_world], &tmp, sizeof(mat44d));
    break;
  case MATMODE_VIEW:
    mmul(&tmp, &stack_view[cur_view], m);
    memcpy(stack_view[cur_view], &tmp, sizeof(mat44d));
    break;
  case MATMODE_PROJ:
    mmul(&tmp, &stack_proj[cur_proj], m);
    memcpy(stack_proj[cur_proj], &tmp, sizeof(mat44d));
    break;
  case MATMODE_TEXTURE:
    mmul(&tmp, &stack_texsture[cur_texsture], m);
    memcpy(stack_texsture[cur_texsture], &tmp, sizeof(mat44d));
    break;
  }
}

// 転置
void transpose(mat22d* pdst, const mat22d* pa)
{
  assert(pdst != pa);
  double* dst = (double*)pdst;
  const double* a = (double*)pa;
  dst[0] = a[0];
  dst[1] = a[2];
  dst[2] = a[1];
  dst[3] = a[3];
}
void transpose(mat33d* pdst, const mat33d* pa)
{
  assert(pdst != pa);
  double* dst = (double*)pdst;
  const double* a = (double*)pa;
  for(int i = 0; i < 3; ++i)
  {
    int k = 3 * i;
    dst[ 0 + i] = a[ 0 + k];
    dst[ 3 + i] = a[ 1 + k];
    dst[ 6 + i] = a[ 2 + k];
  }
}
void transpose(mat44d* pdst, const mat44d* pa)
{
  assert(pdst != pa);
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
// 余因子行列
void cofactor(mat33d* pdst, const mat44d* pa, int i, int j)
{
  double* dst = (double*)pdst;
  const double* a = (double*)pa;
  int k = 0;
  while(k < 16)
  {
    if(
       k != 4*i + 0 &&
       k != 4*i + 1 &&
       k != 4*i + 2 &&
       k != 4*i + 3 &&
       k != 4*0 + j &&
       k != 4*1 + j &&
       k != 4*2 + j &&
       k != 4*3 + j
    )
    {
      *dst++ = a[k];
    }
    ++k;
  }
}

// ゲームプログラミングのための3Dグラフィックス数学
// p49
double determinant(const mat22d* pa)
{
  double* a = (double*)pa;
  return a[0] * a[3] - a[1] * a[2];
}

double determinant(const mat33d* pa)
{
  double* a = (double*)pa;
  return 
    a[0] * (a[4] * a[8] - a[5] * a[7]) 
    - a[1] * (a[3] * a[8] - a[5] * a[6])
    + a[2] * (a[3] * a[7] - a[4] * a[6]);
}

double determinant(const mat44d* pa)
{
  double* a = (double*)pa;
  double sum = 0.;
  mat33d tmp;
  const int j = 0; // 0 < j < 4であれば、なんでもよい。
  for(int i = 0; i < 4; ++i)
  {
    cofactor(&tmp, pa, i, j);
    double det = determinant(&tmp);
    double tmp2 = a[4*i+j] * det;
    if((i+j)&1)
    {
      sum -= tmp2;
    }
    else
    {
      sum += tmp2;
    }
  }
  return sum;
}
// p50
void inverse(mat22d* pdst, const mat22d* pa)
{
  assert(pdst != pa);
  double det = determinant(pa);
  double* dst = (double*)pdst;
  const double* a = (double*)pa;
  dst[0] = a[3] / det;
  dst[1] = -a[1] / det;
  dst[2] = -a[2] / det;
  dst[3] = a[0] / det;
}
void inverse(mat33d* pdst, const mat33d* pa)
{
  assert(pdst != pa);
  double det = determinant(pa);
  double* dst = (double*)pdst;
  const double* a = (double*)pa;
  dst[0] = (a[3* 1 + 1] * a[3* 2 + 2] - a[3* 1 + 2] * a[3* 2 + 1]) / det;
  dst[1] = (a[3* 0 + 2] * a[3* 2 + 1] - a[3* 0 + 1] * a[3* 2 + 2]) / det;
  dst[2] = (a[3* 0 + 1] * a[3* 1 + 2] - a[3* 0 + 2] * a[3* 1 + 1]) / det;

  dst[3] = (a[3* 1 + 2] * a[3* 2 + 0] - a[3* 1 + 0] * a[3* 2 + 2]) / det;
  dst[4] = (a[3* 0 + 0] * a[3* 2 + 2] - a[3* 0 + 2] * a[3* 2 + 0]) / det;
  dst[5] = (a[3* 0 + 2] * a[3* 1 + 0] - a[3* 0 + 0] * a[3* 1 + 2]) / det;

  dst[6] = (a[3* 1 + 0] * a[3* 2 + 1] - a[3* 1 + 1] * a[3* 2 + 0]) / det;
  dst[7] = (a[3* 0 + 1] * a[3* 2 + 0] - a[3* 0 + 0] * a[3* 2 + 1]) / det;
  dst[8] = (a[3* 0 + 0] * a[3* 1 + 1] - a[3* 0 + 1] * a[3* 1 + 0]) / det;
}
void inverse(mat44d* pdst, const mat44d* pa)
{
  assert(pdst != pa);
  double* dst = (double*)pdst;
  mat33d tmp33;
  double det0 = determinant(pa);
  for(int i = 0; i < 4; ++i)
  {
    for(int j = 0; j < 4; ++j)
    {
      cofactor(&tmp33, pa, j, i);
      double det1 = determinant(&tmp33);
      if((i+j)&1)
      {
        dst[4*i + j] = -det1 / det0;
      }
      else
      {
        dst[4*i + j] = det1 / det0;
      }
    }
  }  
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
// TODO:

void matprint(const mat22d& pa)
{
  const double* a = (double*)&pa;
  for(int i = 0; i < 2; ++i)
  {
    int k = 2 * i;
    printf("  [%d]:  %f, %f\n", i, a[k + 0], a[k + 1]);
  }
}
void matprint(const mat33d& pa)
{
  const double* a = (double*)&pa;
  for(int i = 0; i < 3; ++i)
  {
    int k = 3 * i;
    printf("  [%d]:  %f, %f, %f\n", i, a[k + 0], a[k + 1], a[k + 2]);
  }
}
void matprint(const mat44d& pa)
{
  const double* a = (double*)&pa;
  for(int i = 0; i < 4; ++i)
  {
    int k = 4 * i;
    printf("  [%d]:  %f, %f, %f, %f\n", i, a[k + 0], a[k + 1], a[k + 2], a[k + 3]);
  }
}
void matprint(E_MATMODE mode)
{
  switch(mode)
  {
  case MATMODE_WORLD:
    matprint(stack_world[cur_world]);
    break;
  case MATMODE_VIEW:
    matprint(stack_view[cur_view]);
    break;
  case MATMODE_PROJ:
    matprint(stack_proj[cur_proj]);
    break;
  case MATMODE_TEXTURE:
    matprint(stack_texsture[cur_texsture]);
    break;
  }
}
void matprint()
{
  switch(cur_matmode)
  {
  case MATMODE_WORLD:
    matprint(stack_world[cur_world]);
    break;
  case MATMODE_VIEW:
    matprint(stack_view[cur_view]);
    break;
  case MATMODE_PROJ:
    matprint(stack_proj[cur_proj]);
    break;
  case MATMODE_TEXTURE:
    matprint(stack_texsture[cur_texsture]);
    break;
  }
}

void randmat(mat33d* pa)
{
  double* a = (double*)pa;
  for(int i = 0; i < 9; ++i)
  {
    a[i] = 1.0 * rand() / RAND_MAX;
  }
}
void randmat(mat44d* pa)
{
  double* a = (double*)pa;
  for(int i = 0; i < 16; ++i)
  {
    a[i] = 1.0 * rand() / RAND_MAX;
  }
}

// p104
// (left, bottom, -near):ニアクリップの左下
// (right, top, -near):ニアクリップの右上の座標を示す
// カメラは-Zの方へ向いていることに注意。
void frustum(mat44d* pdst, double left, double right, double bottom, double top, double near, double far)
{
  double* dst = (double*)pdst;

  dst[0] = 2.*near / (right - left);
  dst[1] = 0.;
  dst[2] = (right + left) / (right - left);
  dst[3] = 0.;

  dst[4] = 0.;
  dst[5] = 2.*near / (top - bottom);
  dst[6] = (top + bottom) / (top - bottom);
  dst[7] = 0.;

  dst[8] = 0.;
  dst[9] = 0.;
  dst[10] = -(far + near) / (far - near);
  dst[11] = 2.*near / (far - near);

  dst[12] = 0.;
  dst[13] = 0.;
  dst[14] = -1.;
  dst[15] = 0.;

}

void frustum(double left, double right, double bottom, double top, double near, double far)
{
  switch(cur_matmode)
  {
  case MATMODE_WORLD:
    frustum(&stack_world[cur_world], left, right, bottom, top, near, far);
    break;
  case MATMODE_VIEW:
    frustum(&stack_view[cur_view], left, right, bottom, top, near, far);
    break;
  case MATMODE_PROJ:
    frustum(&stack_proj[cur_proj], left, right, bottom, top, near, far);
    break;
  case MATMODE_TEXTURE:
    frustum(&stack_texsture[cur_texsture], left, right, bottom, top, near, far);
    break;
  }
}

// OpenGL プログラミングガイド5版,p727
void ortho(mat44d* pdst, double left, double right, double bottom, double top, double near, double far)
{
  double* dst = (double*)pdst;
  dst[0] = 2.*near / (right - left);
  dst[1] = 0.;
  dst[2] = 0.;
  dst[3] = (right + left) / (right - left);

  dst[4] = 0.;
  dst[5] = 2.*near / (top - bottom);
  dst[6] = 0.;
  dst[7] = -(top + bottom) / (top - bottom);

  dst[8] = 0.;
  dst[9] = 0.;
  dst[10] = -2. / (far - near);
  dst[11] = (far + near) / (far - near);

  dst[12] = 0.;
  dst[13] = 0.;
  dst[14] = 0.;
  dst[15] = 1.;
}

void ortho(double left, double right, double bottom, double top, double near, double far)
{
  switch(cur_matmode)
  {
  case MATMODE_WORLD:
    ortho(&stack_world[cur_world], left, right, bottom, top, near, far);
    break;
  case MATMODE_VIEW:
    ortho(&stack_view[cur_view], left, right, bottom, top, near, far);
    break;
  case MATMODE_PROJ:
    ortho(&stack_proj[cur_proj], left, right, bottom, top, near, far);
    break;
  case MATMODE_TEXTURE:
    ortho(&stack_texsture[cur_texsture], left, right, bottom, top, near, far);
    break;
  }
}

// fovy [0.0 .. 180.0], aspect:w/h
// near, far: 視点からの距離
void perspective(mat44d* pdst, double fovy, double aspect, double near, double far)
{
  const double halfH = near * tan(fovy * 0.5);
  const double halfW = halfH * aspect; // aspect:w/h
  frustum(pdst, -halfW, halfW, -halfH, halfH, near, far);
}

void perspective(double fovy, double aspect, double near, double far)
{
  switch(cur_matmode)
  {
  case MATMODE_WORLD:
    perspective(&stack_world[cur_world], fovy, aspect, near, far);
    break;
  case MATMODE_VIEW:
    perspective(&stack_view[cur_view], fovy, aspect, near, far);
    break;
  case MATMODE_PROJ:
    perspective(&stack_proj[cur_proj], fovy, aspect, near, far);
    break;
  case MATMODE_TEXTURE:
    perspective(&stack_texsture[cur_texsture], fovy, aspect, near, far);
    break;
  }
}

// world2viewとして使う。
// OpenGLの神髄, p85
void lookat(mat44d* pdst, double eyeX, double eyeY, double eyeZ, double centerX,double centerY,double centerZ, double upX, double upY, double upZ)
{
  double* dst = (double*)pdst;
  v3d dir = {centerX - eyeX, centerY - eyeY, centerZ - eyeZ};
  normalize(&dir);
  v3d c;
  v3d u = {upX, upY, upZ};
  cross(&c, &dir, &u);
  cross(&u, &c, &dir);
  normalize(&u);
  normalize(&c);
  
  dst[0] = c[0];
  dst[1] = c[1];
  dst[2] = c[2];
  dst[3] = 0.;

  dst[4] = u[0];
  dst[5] = u[1];
  dst[6] = u[2];
  dst[7] = 0.;

  dst[8] = -dir[0];
  dst[9] = -dir[1];
  dst[10] = -dir[2];
  dst[11] = 0.;

  dst[12] = 0.;
  dst[13] = 0.;
  dst[14] = 0.;
  dst[15] = 1.;
}

// TODO:
// 出力画面の位置を設定する。
// 正規化され、x, y[-1.0 .. -1.0], z [0.0 .. 1.0]にあるが、これを画面座標に変換する。
// wとhの比をperspectiveの比にあわせないと、画面が歪むことに注意。
void viewport(int x, int y, int w, int h)
{


}


double abs(const v3d* pv)
{
  double* v = (double*)pv;
  return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}
double abs2(const v3d* pv)
{
  double* v = (double*)pv;
  return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}
void normalize(v3d* pdst, const v3d* pv)
{
  double* dst = (double*)pdst;
  double* v = (double*)pv;
  double tmp = abs(pv);
  dst[0] = v[0] / tmp;
  dst[1] = v[1] / tmp;
  dst[2] = v[2] / tmp;
}
void normalize(v3d* pio)
{
  double* io = (double*)pio;
  double tmp = abs(pio);
  io[0] = io[0] / tmp;
  io[1] = io[1] / tmp;
  io[2] = io[2] / tmp;
}
void add(v3d* pdst, const v3d* pa, const v3d* pb)
{
  double* dst = (double*)pdst;
  double* a = (double*)pa;
  double* b = (double*)pb;
  dst[0] = a[0] + b[0];
  dst[1] = a[1] + b[1];
  dst[2] = a[2] + b[2];
}
void sub(v3d* pdst, const v3d* pa, const v3d* pb)
{
  double* dst = (double*)pdst;
  double* a = (double*)pa;
  double* b = (double*)pb;
  dst[0] = a[0] - b[0];
  dst[1] = a[1] - b[1];
  dst[2] = a[2] - b[2];
}
double dot(const v3d* pa, const v3d* pb)
{
  double* a = (double*)pa;
  double* b = (double*)pb;
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
void cross(v3d* pdst, const v3d* pa, const v3d* pb)
{
  double* dst = (double*)pdst;
  double* a = (double*)pa;
  double* b = (double*)pb;
  dst[0] = a[1] * b[2] - a[2] * b[1];
  dst[1] = a[2] * b[0] - a[0] * b[2];
  dst[2] = a[0] * b[1] - a[1] * b[0];
}

