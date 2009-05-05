#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <SDL/SDL.h>

#include "basetype.h"
#include "setting.h"
#include "vec.h"
#include "sdluser.h"
#include "util.h"
#include "drawline.h"
#include "intersect.h"
#include "hodge.h"
#include "matstack.h"




struct TriEdgePair
{
// egde_pair(left_egde(x, dx), right_egde(x, dx))
// and y0, y1.
  Vec2f left;
  Vec2f right;
  float bottom;
  float top;
};


// input: vertex, 
// output: 
// egde_pair(left_egde(x, dx), right_egde(x, dx))
// and y0, y1.
// return TriEdgePair num
int triangle_setup(const Vec2f* v, TriEdgePair* dst)
{
  int min, mid, max;
  // search middle y
  {
    if(v[0].y < v[1].y)
    {
      if(v[1].y < v[2].y)
      {
        mid = 1;
        max = 2;
        min = 0;
      }
      else if(v[0].y < v[2].y)
      {
        mid = 2;
        max = 1;
        min = 0;
      }
      else
      {
        mid = 0;
        max = 1;
        min = 2;
      }
    }
    else
    {
      if(v[0].y < v[2].y)
      {
        mid = 0;
        max = 2;
        min = 1;
      }
      else if(v[1].y < v[2].y)
      {
        mid = 2;
        max = 0;
        min = 1;
      }
      else
      {
        mid = 1;
        max = 0;
        min = 2;
      }
    }
  }
  int pairnum;
  // create edge
  {
    if((int)v[mid].y == (int)v[min].y)
    {
      if((int)v[mid].y == (int)v[max].y)
      {
        // 全部揃っている
        pairnum = 1;
        assert(!"not implimented");
      }
      else
      {
        // 下線が水平
        pairnum = 1;
        Vec2f tmp0 = v[max] - v[min];
        Vec2f tmp2 = v[max] - v[mid];
        if(v[min].x < v[mid].x)
        {
          // midは右側
          dst[0].left.x = v[min].x; // x
          dst[0].left.y = tmp0.x / tmp0.y; // dx
          dst[0].right.x = v[mid].x; // x
          dst[0].right.y = tmp2.x / tmp2.y; // dx
          dst[0].bottom = v[min].y;
          dst[0].top = v[max].y;
        }
        else
        {
          // midは左側
          dst[0].right.x = v[min].x; // x
          dst[0].right.y = tmp0.x / tmp0.y; // dx
          dst[0].left.x = v[mid].x; // x
          dst[0].left.y = tmp2.x / tmp2.y; // dx
          dst[0].bottom = v[min].y;
          dst[0].top = v[max].y;
        }
        // debprintf("hello2: max=%d, mid=%d, min=%d\n", max, mid, min);
        // debprintf("hello2: max=%f,%f, mid=%f,%f, min=%f,%f\n", v[max].x, v[max].y, v[mid].x, v[mid].y, v[min].x, v[min].y);
        // debprintf("hello2: %f, %f, %f, %f, %f, %f\n", dst[0].left.x, dst[0].right.x, dst[0].top, dst[0].bottom, dst[0].left.y, dst[0].right.y);
      }
    }
    else if((int)v[mid].y == (int)v[max].y)
    {
      // 上線が水平
      pairnum = 1;
      Vec2f tmp0 = v[max] - v[min];
      Vec2f tmp1 = v[mid] - v[min];
      if(v[max].x < v[mid].x)
      {
        // midは右側
        dst[0].left.x = v[min].x; // x
        dst[0].left.y = tmp0.x / tmp0.y; // dx
        dst[0].right.x = v[min].x; // x
        dst[0].right.y = tmp1.x / tmp1.y; // dx
        dst[0].bottom = v[min].y;
        dst[0].top = v[max].y;
      }
      else
      {
        // midは左側
        dst[0].right.x = v[min].x; // x
        dst[0].right.y = tmp0.x / tmp0.y; // dx
        dst[0].left.x = v[min].x; // x
        dst[0].left.y = tmp1.x / tmp1.y; // dx
        dst[0].bottom = v[min].y;
        dst[0].top = v[max].y;
      }
      // debprintf("hello: max=%d, mid=%d, min=%d\n", max, mid, min);
      // debprintf("hello: max=%f,%f, mid=%f,%f, min=%f,%f\n", v[max].x, v[max].y, v[mid].x, v[mid].y, v[min].x, v[min].y);
      // debprintf("hello: %f, %f, %f, %f, %f, %f\n", dst[0].left.x, dst[0].right.x, dst[0].top, dst[0].bottom, dst[0].left.y, dst[0].right.y);
    }
    else
    {
      // いい感じの三角形
      // midから水平線を引き分割。
      pairnum = 2;
      float t = intersect_line_hline(v[min], v[max], v[mid].y);
      Vec2f tmp0 = v[max] - v[min];
      float tmp0_dx = tmp0.x / tmp0.y;
      Vec2f tmp1 = v[mid] - v[min];
      Vec2f tmp2 = v[max] - v[mid];
      Vec2f isec = v[min] + t * tmp0;
      if(isec.x < v[mid].x)
      {
        // midは右側
        dst[0].left.x = v[min].x; // x
        dst[0].left.y = tmp0_dx; // dx
        dst[0].right.x = v[min].x; // x
        dst[0].right.y = tmp1.x / tmp1.y; // dx
        dst[0].bottom = v[min].y;
        dst[0].top = isec.y;

        dst[1].left.x = isec.x; // x
        dst[1].left.y = tmp0_dx; // dx
        dst[1].right.x = v[mid].x; // x
        dst[1].right.y = tmp2.x / tmp2.y; // dx
        dst[1].bottom = v[mid].y;
        dst[1].top = v[max].y;
      }
      else
      {
        // midは左側
        dst[0].right.x = v[min].x; // x
        dst[0].right.y = tmp0_dx; // dx
        dst[0].left.x = v[min].x; // x
        dst[0].left.y = tmp1.x / tmp1.y; // dx
        dst[0].bottom = v[min].y;
        dst[0].top = isec.y;

        dst[1].right.x = isec.x; // x
        dst[1].right.y = tmp0_dx; // dx
        dst[1].left.x = v[mid].x; // x
        dst[1].left.y = tmp2.x / tmp2.y; // dx
        dst[1].bottom = v[mid].y;
        dst[1].top = v[max].y;
      }
    }
  }
  return pairnum;

  // create edge pair
  // 三角形だけにこだわればいい。クリップ済み多角形は、strip処理にまかせればよい。
  // strip処理では、三角形分割し、この関数が処理する。
}

void draw_scanline_interpolate(const TriEdgePair& epair)
{
  float x0 = epair.left.x;
  float dx0 = epair.left.y;
  float x1 = epair.right.x;
  float dx1 = epair.right.y;
  for(int y = (int)epair.bottom; y < (int)epair.top; ++y)
  {
    //      nanoslp(1000);
    moveto((int)x0, y);
    lineto((int)x1, y);
    x0 += dx0;
    x1 += dx1;
  }
}

bool need_clip(const Vec2f* v)
{
  return true;
}

void draw_triangle(const Vec2f* v)
{
  if(need_clip(v))
  {
    float xxxx;
    Vec2f result[3 + 6]; // clipする度に点が１つふえる。clip面は6つあるので、点が最大６増える。
    int vnum = hodge(v, &xxxx, result);
    int triangle_num = vnum - 2;
    // fan stripとして扱う。
    for(int j = 0; j < triangle_num; ++j)
    {
      TriEdgePair epair[2];
      int epnum = triangle_setup(result + j, epair);
      for(int i = 0; i < epnum; ++i)
      {
        // debprintf("%d, %d: %f, %f, %f, %f\n", j, i, epair[i].left.x, epair[i].right.x, epair[i].top, epair[i].bottom);
        draw_scanline_interpolate(epair[i]);
      }
      result[j + 1] = result[0]; // fan中心をコピーして毎回連続配列になるようにする。最後のコピーは無駄だが、配列はあふれないので放っとく。
    }
  }
  else
  {
    TriEdgePair epair[2];
    int epnum = triangle_setup(v, epair);
    for(int i = 0; i < epnum; ++i)
    {
      draw_scanline_interpolate(epair[i]);
    }
  }
}
enum E_PRIMITIVE
  {
    PRIMITIVE_POINTS,
    PRIMITIVE_LINES,
    PRIMITIVE_LINE_STRIP,
    PRIMITIVE_LINE_LOOP,
    PRIMITIVE_TRIANGLES,
    PRIMITIVE_TRIANGLE_STRIP,
    PRIMITIVE_TRIANGLE_FAN,
    PRIMITIVE_QUADS, // fanで実装。
    PRIMITIVE_QUAD_STRIP, // TRIANGLE_STRIPで実装
    PRIMITIVE_POLYGON, // 凹はないのでfanで実装できる。
  };
void draw_triangle_strip(const Vec2f* v, int vnum)
{
  if(vnum < 3)return;
  Vec2f tmp[3];
  // １つおきに三角形配列を逆にしないといけないので、結構面倒。
  for(int i = 0; i < vnum - 2; ++i)
  {
    if(i & 1 == 0)
    {
      draw_triangle(v + i);
    }
    else
    {
      tmp[0] = v[i];
      tmp[1] = v[i + 2];
      tmp[2] = v[i + 1];
      draw_triangle(tmp);
    }
  }
}
void draw_triangles(const Vec2f* v, int vnum)
{
  if(vnum < 3)return;
  for(int i = 0; i < vnum - 2; i += 3)
  {
    draw_triangle(v + i);
  }
}
void draw_triangle_fan(const Vec2f* v, int vnum)
{
  if(vnum < 3)return;
  Vec2f tmp[vnum];
  memcpy(tmp, v, sizeof(Vec2f) * vnum);
  for(int i = 0; i < vnum - 2; i += 3)
  {
    draw_triangle(tmp + i);
    tmp[i + 1] = tmp[0];
  }
}

void draw_poly_raw(const SDL_Color& c, Vec2f* v, int num)
{
  if(num == 0)return;
  moveto(v[0]);
  for(int i = 1; i < num; ++i)
  {
    lineto(c, v[i]);
  }
  lineto(c, v[0]);
}


// TODO:
// 処理順序は、以下を参照
// Jim Blinn's Corner A Trip Down the Graphics Pipeline, p176
// コンピュータグラフィックス 理論と実践, p880
// 頂点の項目は、位置、法線、頂点カラー、テクスチャ座標
void draw_primitive(E_PRIMITIVE ptype, v4f* pos, v4f* normal, v4f* col, v4f* texcood, int vnum)
{
  assert(vnum < 50);
  v4f pos0[50];
  // 座標変換
  {
    mat44d m;
    getmodel2perspective(&m);
    for(int i = 0; i < vnum; ++i)
    {
      mvmul(pos0 + i, &m, pos + i);
    }
  }

  // 法線座標変換、texcood座標変換
  // 裏面カリングの後に座標変換したほうが効率的。ただし、クリップ前には終えないとダメ。


  // clip（hodge処理）

  // w除算

  // 裏面カリング、w除算後じゃないとダメな気がする。

  // トライアングルセットアップ（エッジ出力）

  // スキャンライン（補間 -> w除算 -> シェーディング）
}
// TODO:
void draw_indexed_primitive(E_PRIMITIVE ptype, v4f* pos, v4f* normal, v4f* col, v4f* texcood, int* idx, int vnum)
{


}


void render()
{
  sdlerror();
  if( SDL_LockSurface( gScreenSurface ) == -1 )return;// サーフェースをロック
  SDL_Color blue;
  blue.r = 0;
  blue.g = 0;
  blue.b = 255;
  SDL_Color red;
  red.r = 255;
  red.g = 0;
  red.b = 0;

  // 色を指定したピクセルフォーマット用の色情報に変換
  Uint32 newColor = SDL_MapRGB( gScreenSurface->format, blue.r, blue.g, blue.b );
  for(int y = 30; y < 100; ++y)
    for(int x = 100; x < 200; ++x)
      PutColor( gScreenSurface, x, y, newColor );

  //    line(&c, 200, 300, 300, 400);
  //    line(&c, 200, 300, 300, 440);


  moveto(B_RIGHT, B_BOTTOM);
  lineto(&blue, B_RIGHT, B_TOP);
  lineto(&blue, B_LEFT, B_TOP);
  lineto(&blue, B_LEFT, B_BOTTOM);
  lineto(&blue, B_RIGHT, B_BOTTOM);

  // input Vec2f * 3, clips(left, right, top, bottom)
  // output return dst_n, Vec2f * dst_n
  //int hodge(Vec2f* v, float* clips, Vec2f* dst)
  Vec2f result[30];
  Vec2f input[] = {
    Vec2f(200.f, 200.f),  Vec2f(200.f, 400.f), Vec2f(300.f, 340.f),
    Vec2f(200.f, 0.f),  Vec2f(0.f, 400.f), Vec2f(500.f, 440.f),
    Vec2f(0.f, 0.f),  Vec2f(200.f, 200.f), Vec2f(30.f, 60.f),
  };
  draw_poly_raw(blue, input + 3, 3);
  float xxxx;
  int num = hodge(input + 3, &xxxx, result);
  draw_poly_raw(red, result, num);

  draw_triangle(input + 3);

  sdlerror();
  SDL_UnlockSurface( gScreenSurface );// ロックを解除


  {
    loadidentity();
    //matprint();
    mat44d m, invm;
    getmat(&m);
    inverse(&invm, &m);
    //matprint(invm);
  }
  {
    mat44d invm, m2;
    //randmat(&m);
    mat44d m={
      1., 2., 3., 1.,
      4., 5., 6., 21.,
      3., 2., 7., 1.,
      13., 12., 17., 1.,
    };
    //matprint(m);
    inverse(&invm, &m);
    //matprint(invm);
    mmul(&m2, &invm, &m);
    matprint(m2);
  }
}
