#include <stdio.h>
#include <SDL/SDL.h>

#include "basetype.h"
#include "setting.h"
#include "sdluser.h"

SDL_Surface *gScreenSurface;


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


