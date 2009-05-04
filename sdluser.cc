#include <stdio.h>
#include <SDL/SDL.h>

#include "basetype.h"
#include "setting.h"
#include "sdluser.h"

SDL_Surface *gScreenSurface;

// 塗りつぶす
void FillScreen( SDL_Color Color )
{
    // 色を指定したピクセルフォーマット用の色情報に変換
    Uint32 color = SDL_MapRGB( gScreenSurface->format, Color.r, Color.g, Color.b );
	
    SDL_Rect dest;
    dest.x = 0;dest.y = 0;
    dest.w = SCREEN_WIDTH;
    dest.h = SCREEN_HEIGHT;
    SDL_FillRect( gScreenSurface, &dest, color );
}
void FillScreen( int r, int g, int b )
{
    SDL_Color Color;
    Color.r = r;Color.g = g;Color.b = b;
    FillScreen( Color );
}


