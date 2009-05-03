#include <stdio.h>
#include <math.h>
#include <SDL/SDL.h>

//#pragma comment(lib, "SDL.lib")
//#pragma comment(lib, "SDLmain.lib")

#define B_TOP 100
#define B_BOTTOM 400
#define B_LEFT 100
#define B_RIGHT 500


struct Vec2i
{
    int x;
    int y;
    Vec2i& operator=(const Vec2i& rhs){x = rhs.x; y = rhs.y; return *this;};
    Vec2i& operator+=(const Vec2i& rhs){x += rhs.x; y += rhs.y; return *this;};
    Vec2i& operator-=(const Vec2i& rhs){x -= rhs.x; y -= rhs.y; return *this;};
};
struct Vec2f
{
  float x;
  float y;
  explicit Vec2f(){};
  explicit Vec2f(float x0, float y0) : x(x0), y(y0){};
  Vec2f& operator=(const Vec2f& rhs){x = rhs.x; y = rhs.y; return *this;};
  Vec2f& operator+=(const Vec2f& rhs){x += rhs.x; y += rhs.y; return *this;};
  Vec2f& operator-=(const Vec2f& rhs){x -= rhs.x; y -= rhs.y; return *this;};
  Vec2f& operator*=(float rhs){x *= rhs; y *= rhs; return *this;};
  Vec2f& operator/=(float rhs){x /= rhs; y /= rhs; return *this;};
  Vec2f operator-(){return Vec2f(-x, -y);};
  float norm2()const {return x*x + y*y;}
  float norm()const {return sqrt(x*x + y*y);}
  void normalize(){float abs = norm(); x /= abs; y /= abs;}
  float dot(const Vec2f& rhs){return x * rhs.x + y * rhs.y;}
};
Vec2f operator+(Vec2f lhs, const Vec2f& rhs){return lhs += rhs;}
Vec2f operator-(Vec2f lhs, const Vec2f& rhs){return lhs -= rhs;}
Vec2f operator*(float lhs, Vec2f rhs){return rhs *= lhs;}


struct Vec3f
{
    float x;
    float y;
    float z;
    Vec3f& operator=(const Vec3f& rhs){x = rhs.x; y = rhs.y; z = rhs.z; return *this;};
    Vec3f& operator+=(const Vec3f& rhs){x += rhs.x; y += rhs.y; z += rhs.z; return *this;};
    Vec3f& operator-=(const Vec3f& rhs){x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this;};
};
Vec3f operator+(Vec3f lhs, const Vec3f& rhs){return lhs += rhs;}
Vec3f operator-(Vec3f lhs, const Vec3f& rhs){return lhs -= rhs;}

struct Vec4f
{
    float x;
    float y;
    float z;
    float w;
};
typedef Vec2f Edge2f[2];



#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP    32
#define GAME_CAPTION  "SDLTutorial"

bool Init();      // 初期化処理
void End();       // 終了処理
bool PollEvent(); // イベント処理

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
Vec2f intersect(const Vec2f& v0, const Vec2f& v1, const Edge2f& boundary)
{
  Vec2f tmp0 = v1 - v0;
  Vec2f tmp1 = boundary[1] - boundary[0];
  tmp0.normalize();
  tmp1.normalize();
  float dot0 = tmp0.dot(tmp1);
  float m[4] = {1, dot0, dot0, 1};
  Vec2f tmp3((boundary[0] - v0).dot(tmp0), (v0 - boundary[0]).dot(tmp1));
  Vec2f param = mul_m22_v2(m, tmp3);
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
		dst[outnum++] = intersect(s, p, boundary);
		dst[outnum++] = p;
	    }
	}
	else
	{
	    if(inside(s, boundary))
	    {
		dst[outnum++] = intersect(s, p, boundary);
	    }
	}
	s = p;
    }
    return outnum;
}

// input Vec2f * 3, clips(left, right, top, bottom)
// output return dst_n, Vec2f * dst_n
int hodge(Vec2f* v, float* clips, Vec2f* dst)
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
    return vnum;
}


// input: vertex, 
// output: 
// egde_pair(left_egde(x, dx), right_egde(x, dx))
// and y0, y1.
void triangle_setup()
{

}

void render()
{
    if( SDL_LockSurface( gScreenSurface ) == -1 )return;// サーフェースをロック
    SDL_Color c;
    c.r = 0;
    c.g = 0;
    c.b = 255;
    
    // 色を指定したピクセルフォーマット用の色情報に変換
    Uint32 newColor = SDL_MapRGB( gScreenSurface->format, c.r, c.g, c.b );
    for(int y = 30; y < 100; ++y)
	for(int x = 100; x < 200; ++x)
	    PutColor( gScreenSurface, x, y, newColor );

//    line(&c, 200, 300, 300, 400);
//    line(&c, 200, 300, 300, 440);


    moveto(B_RIGHT, B_BOTTOM);
    lineto(&c, B_RIGHT, B_TOP);
    lineto(&c, B_LEFT, B_TOP);
    lineto(&c, B_LEFT, B_BOTTOM);
    lineto(&c, B_RIGHT, B_BOTTOM);



    // input Vec2f * 3, clips(left, right, top, bottom)
    // output return dst_n, Vec2f * dst_n
    //int hodge(Vec2f* v, float* clips, Vec2f* dst)
    Vec2f result[30];
    Vec2f input[3] = {
      Vec2f(0.f, 0.f),
      Vec2f(100.f, 100.f),
      Vec2f(30.f, 60.f),
    };
    float xxxx;
    int num = hodge(input, &xxxx, result);
    moveto(result[0]);
    for(int i = 1; i < num; ++i)
      {
	lineto(c, result[i]);
	printf("%d: %f, %f\n", i, result[i].x, result[i].y);
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


int main(int argc, char* argv[])
{
    // 初期化
    if( !Init() ){
	printf( "error.:%s\n", SDL_GetError());
	return 0;
    }

    // メインループ
    while( 1 ){
	// WhiteOut();
	render();
	// 画面を更新します
	SDL_Flip( gScreenSurface );

	// イベント処理
	if( !PollEvent() )break;
    }

    // 終了処理
    End();

    return 0;
}

// 初期化処理
bool Init()
{
    // SDLの初期化
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
	printf(SDL_GetError());
	return false;
    }
    // キャプションの設定
    SDL_WM_SetCaption( GAME_CAPTION, NULL );

    // ウィンドウの初期化
    gScreenSurface = SDL_SetVideoMode(
	SCREEN_WIDTH,
	SCREEN_HEIGHT,
	SCREEN_BPP,
	SDL_SWSURFACE//|SDL_FULLSCREEN
	);

    // マウスカーソルを消す場合は
    // SDL_ShowCursor(SDL_DISABLE );
    return gScreenSurface != NULL;
}

// 終了処理
void End()
{
    // SDLの終了
    SDL_Quit();
}

// イベント処理
bool PollEvent()
{
    SDL_Event ev;
    SDLKey *key;
    while(SDL_PollEvent(&ev) )
    {
	switch(ev.type){
	    case SDL_QUIT:// ウィンドウの臨ボタンが押された時など
		return false;
		break;
	    case SDL_KEYDOWN:// キーボードからの入力があった時
	    {
		key=&(ev.key.keysym.sym); // どのキーが押されたかを取得
		if(*key==27){// ESCキー
		    return false;
		}
	    }
	    break;
	}
    }
    return true;
}
