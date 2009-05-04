#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <SDL/SDL.h>

#include "basetype.h"
#include "vec.h"
#include "sdluser.h"
#include "util.h"

#define B_TOP 100
#define B_BOTTOM 400
#define B_LEFT 100
#define B_RIGHT 500

bool Init();      // 初期化処理
void End();       // 終了処理
bool PollEvent(); // イベント処理


SDL_Color GetColor( SDL_Surface* pSurface, int x, int y )
{
    SDL_PixelFormat *fmt = pSurface->format;    // ピクセルフォーマット
    Uint8 BitsPerPixel = fmt->BitsPerPixel;     // 1ピクセルあたりのビット数(bpp)
    Uint8* pixels_8 = (Uint8*)pSurface->pixels; // 先頭アドレス
    Uint16 pitch = pSurface->pitch;             // 1ラインのバイト数
    Uint8 BytesPerPixel = fmt->BytesPerPixel;   // 1ピクセルあたりのバイト数
	
    // 以上の情報から目的のアドレスを算出
    Uint8* target_pixels_8 = &pixels_8[ pitch*y + BytesPerPixel*x ];

    SDL_Color Color;
    switch( BitsPerPixel )
    {// 1ピクセルあたりのビット数に応じて処理を分ける
	case 8:
	{
	    Uint32 pixel = *(Uint8*)target_pixels_8;
	    SDL_GetRGB( pixel, fmt, &Color.r, &Color.g, &Color.b);
	}
	break;
	case 16:
	{
	    Uint32 pixel = *(Uint16*)target_pixels_8;
	    SDL_GetRGB( pixel, fmt, &Color.r, &Color.g, &Color.b);
	}
	break;
	case 24:
	{
	    Uint32 pixel = *(Uint32*)target_pixels_8;
	    SDL_GetRGB( pixel, fmt, &Color.r, &Color.g, &Color.b);
	}
	break;
	case 32:
	{
	    Uint32 pixel = *(Uint32*)target_pixels_8;
	    SDL_GetRGB( pixel, fmt, &Color.r, &Color.g, &Color.b);
	}
	break;
    }

    return Color;

}

void PutColor( SDL_Surface* pSurface, int x, int y, Uint32 Color )
{
    SDL_PixelFormat *fmt = pSurface->format;    // ピクセルフォーマット
    Uint8 BitsPerPixel = fmt->BitsPerPixel;     // 1ピクセルあたりのビット数(bpp)
    Uint8* pixels_8 = (Uint8*)pSurface->pixels; // 先頭アドレス
    Uint16 pitch = pSurface->pitch;             // 1ラインのバイト数
    Uint8 BytesPerPixel = fmt->BytesPerPixel;   // 1ピクセルあたりのバイト数
	
    // 以上の情報から目的のアドレスを算出
    Uint8* target_pixels_8 = &pixels_8[ pitch*y + BytesPerPixel*x ];
    switch( BitsPerPixel )
    {// 1ピクセルあたりのビット数に応じて処理を分ける
	case 8:
	    *target_pixels_8 = Color;
	    break;
	case 16:
	    *(Uint16 *)target_pixels_8 = Color;
	    break;
	case 24:
	    if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
		target_pixels_8[0] = (Color >> 16) & 0xff;
		target_pixels_8[1] = (Color >> 8) & 0xff;
		target_pixels_8[2] = Color & 0xff;
	    } else {
		target_pixels_8[0] = Color & 0xff;
		target_pixels_8[1] = (Color >> 8) & 0xff;
		target_pixels_8[2] = (Color >> 16) & 0xff;
	    }
	    break;
	case 32:
	    *(Uint32 *)target_pixels_8 = Color;
	    break;
    }
}

void WhiteOut()
{
    if( SDL_LockSurface( gScreenSurface ) == -1 )return;// サーフェースをロック
    for( int y = 0 ; y < 480 ; y++ ){
	for( int x = 0 ; x < 639 ; x++ ){
			
	    // ピクセルの色を取得
	    SDL_Color Color = GetColor( gScreenSurface, x, y );
	    float r = (float)Color.r;
	    float g = (float)Color.g;
	    float b = (float)Color.b;
			
	    // 取得した色と白色と混ぜる
	    r = r * 0.75f + 255.0f * 0.25f;
	    g = g * 0.75f + 255.0f * 0.25f;
	    b = b * 0.75f + 255.0f * 0.25f;
			
	    // 最大値のチェック
	    if( r > 255.0f )r = 255.0f;
	    if( g > 255.0f )g = 255.0f;
	    if( b > 255.0f )b = 255.0f;
			
	    // 色を指定したピクセルフォーマット用の色情報に変換
	    Uint32 newColor = SDL_MapRGB( gScreenSurface->format, (Uint8)r, (Uint8)g, (Uint8)b );
	    PutColor( gScreenSurface, x, y, newColor );
	}
    }
    SDL_UnlockSurface( gScreenSurface );// ロックを解除
}

void line(const SDL_Color* c, int x0, int y0, int x1, int y1)
{
    // 色を指定したピクセルフォーマット用の色情報に変換
    Uint32 newColor = SDL_MapRGB( gScreenSurface->format, c->r, c->g, c->b );
    int distx = x1 - x0;
    int disty = y1 - y0;
    int is_x_long = abs(distx) >= abs(disty);
    if(is_x_long)
    {
	if(distx < 0)
	{
	    int tmpx = x0;
	    int tmpy = y0;
	    x0 = x1;
	    y0 = y1;
	    x1 = tmpx;
	    y1 = tmpy;
	    distx = -distx;
	}
	float dy = (y1 - y0) * 1.f / distx;
	float y = y0;
	for(int x = x0; x < x1; ++x, y += dy)
	{
	    PutColor( gScreenSurface, x, (int)y, newColor);
	}
    }
    else
    {
	if(disty < 0)
	{
	    int tmpx = x0;
	    int tmpy = y0;
	    x0 = x1;
	    y0 = y1;
	    x1 = tmpx;
	    y1 = tmpy;
	    disty = -disty;
	}
	float dx = (x1 - x0) * 1.f / disty;
	float x = x0;
	for(int y = y0; y < y1; ++y, x += dx)
	{
	    PutColor( gScreenSurface, (int)x, y, newColor);
	}
    }
}
int draw0x = 0;
int draw0y = 0;
void moveto(int x, int y)
{
    draw0x = x;
    draw0y = y;
}
void lineto(const SDL_Color* c, int x, int y)
{
    line(c, draw0x, draw0y, x, y);
    draw0x = x;
    draw0y = y;
}
void moveto(const Vec2f& v)
{
  moveto((int)v.x, (int)v.y);
}
void lineto(const SDL_Color& c, const Vec2f& v)
{
  line(&c, draw0x, draw0y, (int)v.x, (int)v.y);
  draw0x = (int)v.x;
  draw0y = (int)v.y;
}
void lineto(const Vec2f& v)
{
  SDL_Color c = {0, 255, 0};
  lineto(c, v);
}
void lineto(int x, int y)
{
  SDL_Color c = {0, 255, 0};
  lineto(&c, x, y);
}


bool inside(const Vec2f& v, const Edge2f& boundary)
{
    Vec2f tmp0 = v - boundary[0];
    Vec2f tmp1 = boundary[1] - boundary[0];
    return tmp0.x * tmp1.y - tmp0.y * tmp1.x >= 0;
}
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
}


