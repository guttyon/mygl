#include <stdio.h>
#include "basetype.h"
#include "vec.h"
#include "mat.h"
#include "intersect.h"


Vec2f intersect_line_line(const Vec2f& v0, const Vec2f& v1, const Edge2f& boundary)
{
  Vec2f tmp0 = v1 - v0;
  Vec2f tmp1 = boundary[1] - boundary[0];
  tmp0.normalize();
  tmp1.normalize();
  float dot0 = tmp0.dot(tmp1);
  float m[4] = {1, dot0, dot0, 1};
  Vec2f tmp3((boundary[0] - v0).dot(tmp0), (v0 - boundary[0]).dot(tmp1));
  Vec2f param = mul_m22_v2(m, tmp3)/(1 - dot0 * dot0);
  return v0 + param.x * tmp0;
}

// 水平線との交点までの、線分v0v1に対するパラメータt[0.0 ... 1.0]を返す。
// 交点があることを前提とする。
// 
float intersect_line_hline(const Vec2f& v0, const Vec2f& v1, const float y)
{
  //Vec2f tmp0 = v1 - v0;
  float t = (y - v0.y) / (v1.y - v0.y);
  return t;
}

Vec2f intersect_line_plane(const Vec2f& v0, const Vec2f& v1, const Edge2f& boundary)
{
  Vec2f tmp0 = v1 - v0;
  Vec2f tmp1 = boundary[1] - boundary[0];
  tmp0.normalize();
  tmp1.normalize();
  float dot0 = tmp0.dot(tmp1);
  float m[4] = {1, dot0, dot0, 1};
  Vec2f tmp3((boundary[0] - v0).dot(tmp0), (v0 - boundary[0]).dot(tmp1));
  Vec2f param = mul_m22_v2(m, tmp3)/(1 - dot0 * dot0);
  return v0 + param.x * tmp0;
}
