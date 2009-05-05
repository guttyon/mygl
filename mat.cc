#include <stdio.h>
#include "basetype.h"
#include "vec.h"
#include "mat.h"

Vec2f mul_m22_v2(float* m, Vec2f& v)
{
  Vec2f tmp;
  tmp.x = m[0] * v.x + m[1] * v.y;
  tmp.y = m[2] * v.x + m[3] * v.y;
  return tmp;
}
Vec4f mul_m44_v4(float* m, Vec4f& v)
{
  Vec4f tmp;
  tmp.x = m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3] * v.w;
  tmp.y = m[4] * v.x + m[5] * v.y + m[6] * v.z + m[7] * v.w;
  tmp.z = m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11] * v.w;
  tmp.w = m[12] * v.x + m[13] * v.y + m[14] * v.z + m[15] * v.w;
  return tmp;
}
