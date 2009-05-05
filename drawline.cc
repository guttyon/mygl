#include <math.h>
#include <SDL/SDL.h>
#include "basetype.h"
#include "vec.h"
#include "drawline.h"
#include "sdluser.h"


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
static int draw0x = 0;
static int draw0y = 0;
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


