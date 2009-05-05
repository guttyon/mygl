#include <stdio.h>
#include "basetype.h"
#include "setting.h"
#include "vec.h"
#include "hodge.h"
#include "intersect.h"

bool inside(const Vec2f& v, const Edge2f& boundary)
{
  Vec2f tmp0 = v - boundary[0];
  Vec2f tmp1 = boundary[1] - boundary[0];
  return tmp0.x * tmp1.y - tmp0.y * tmp1.x >= 0;
}



int hodge0(const Vec2f* src, int vnum, const Edge2f& boundary, Vec2f* dst)
{
  Vec2f s, p;
  s = src[vnum - 1];
  int outnum = 0;
  for(int i = 0; i < vnum; ++i)
  {
    p = src[i];
    if(inside(p, boundary))
    {
      if(inside(s, boundary))
      {
        dst[outnum++] = p;
      }
      else
      {
        dst[outnum++] = intersect_line_plane(s, p, boundary);
        dst[outnum++] = p;
      }
    }
    else
    {
      if(inside(s, boundary))
      {
        dst[outnum++] = intersect_line_plane(s, p, boundary);
      }
    }
    s = p;
  }
  return outnum;
}

// input Vec2f * 3, clips(left, right, top, bottom)
// output return dst_n, Vec2f * dst_n
int hodge(const Vec2f* v, float* clips, Vec2f* dst)
{
  int vnum = 3;
  Vec2f buf[2][30];
  Edge2f top = {Vec2f(B_RIGHT, B_TOP), Vec2f(B_LEFT, B_TOP)};
  Edge2f left = {Vec2f(B_LEFT, B_TOP), Vec2f(B_LEFT, B_BOTTOM)};
  Edge2f bottom = {Vec2f(B_LEFT, B_BOTTOM), Vec2f(B_RIGHT, B_BOTTOM)};
  Edge2f right = {Vec2f(B_RIGHT, B_BOTTOM), Vec2f(B_RIGHT, B_TOP)};
  vnum = hodge0(v, vnum, bottom, buf[0]);
  vnum = hodge0(buf[0], vnum, right, buf[1]);
  vnum = hodge0(buf[1], vnum, top, buf[0]);
  vnum = hodge0(buf[0], vnum, left, dst);
  // TODO: near, far クリップも追加すること。
  return vnum;
}

