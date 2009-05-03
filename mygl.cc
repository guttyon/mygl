#include <stdio.h>
#include <SDL/SDL.h>

//#pragma comment(lib, "SDL.lib")
//#pragma comment(lib, "SDLmain.lib")

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP    32
#define GAME_CAPTION  "SDLTutorial"

bool Init();      // $(B=i4|2==hM}(B
void End();       // $(B=*N;=hM}(B
bool PollEvent(); // $(B%$%Y%s%H=hM}(B

SDL_Surface *gScreenSurface;
SDL_Color GetColor( SDL_Surface* pSurface, int x, int y )
{
    SDL_PixelFormat *fmt = pSurface->format;    // $(B%T%/%;%k%U%)!<%^%C%H(B
    Uint8 BitsPerPixel = fmt->BitsPerPixel;     // 1$(B%T%/%;%k$"$?$j$N%S%C%H?t(B(bpp)
    Uint8* pixels_8 = (Uint8*)pSurface->pixels; // $(B@hF,%"%I%l%9(B
    Uint16 pitch = pSurface->pitch;             // 1$(B%i%$%s$N%P%$%H?t(B
    Uint8 BytesPerPixel = fmt->BytesPerPixel;   // 1$(B%T%/%;%k$"$?$j$N%P%$%H?t(B
	
    // $(B0J>e$N>pJs$+$iL\E*$N%"%I%l%9$r;;=P(B
    Uint8* target_pixels_8 = &pixels_8[ pitch*y + BytesPerPixel*x ];

    SDL_Color Color;
    switch( BitsPerPixel )
    {// 1$(B%T%/%;%k$"$?$j$N%S%C%H?t$K1~$8$F=hM}$rJ,$1$k(B
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
    SDL_PixelFormat *fmt = pSurface->format;    // $(B%T%/%;%k%U%)!<%^%C%H(B
    Uint8 BitsPerPixel = fmt->BitsPerPixel;     // 1$(B%T%/%;%k$"$?$j$N%S%C%H?t(B(bpp)
    Uint8* pixels_8 = (Uint8*)pSurface->pixels; // $(B@hF,%"%I%l%9(B
    Uint16 pitch = pSurface->pitch;             // 1$(B%i%$%s$N%P%$%H?t(B
    Uint8 BytesPerPixel = fmt->BytesPerPixel;   // 1$(B%T%/%;%k$"$?$j$N%P%$%H?t(B
	
    // $(B0J>e$N>pJs$+$iL\E*$N%"%I%l%9$r;;=P(B
    Uint8* target_pixels_8 = &pixels_8[ pitch*y + BytesPerPixel*x ];
    switch( BitsPerPixel )
    {// 1$(B%T%/%;%k$"$?$j$N%S%C%H?t$K1~$8$F=hM}$rJ,$1$k(B
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
    if( SDL_LockSurface( gScreenSurface ) == -1 )return;// $(B%5!<%U%'!<%9$r%m%C%/(B
    for( int y = 0 ; y < 480 ; y++ ){
	for( int x = 0 ; x < 639 ; x++ ){
			
	    // $(B%T%/%;%k$N?'$r<hF@(B
	    SDL_Color Color = GetColor( gScreenSurface, x, y );
	    float r = (float)Color.r;
	    float g = (float)Color.g;
	    float b = (float)Color.b;
			
	    // $(B<hF@$7$??'$HGr?'$H:.$<$k(B
	    r = r * 0.75f + 255.0f * 0.25f;
	    g = g * 0.75f + 255.0f * 0.25f;
	    b = b * 0.75f + 255.0f * 0.25f;
			
	    // $(B:GBgCM$N%A%'%C%/(B
	    if( r > 255.0f )r = 255.0f;
	    if( g > 255.0f )g = 255.0f;
	    if( b > 255.0f )b = 255.0f;
			
	    // $(B?'$r;XDj$7$?%T%/%;%k%U%)!<%^%C%HMQ$N?'>pJs$KJQ49(B
	    Uint32 newColor = SDL_MapRGB( gScreenSurface->format, (Uint8)r, (Uint8)g, (Uint8)b );
	    PutColor( gScreenSurface, x, y, newColor );
	}
    }
    SDL_UnlockSurface( gScreenSurface );// $(B%m%C%/$r2r=|(B
}

void line(const SDL_Color* c, int x0, int y0, int x1, int y1)
{
    // $(B?'$r;XDj$7$?%T%/%;%k%U%)!<%^%C%HMQ$N?'>pJs$KJQ49(B
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

void render()
{
    if( SDL_LockSurface( gScreenSurface ) == -1 )return;// $(B%5!<%U%'!<%9$r%m%C%/(B
    SDL_Color c;
    c.r = 0;
    c.g = 0;
    c.b = 255;
    
    // $(B?'$r;XDj$7$?%T%/%;%k%U%)!<%^%C%HMQ$N?'>pJs$KJQ49(B
    Uint32 newColor = SDL_MapRGB( gScreenSurface->format, c.r, c.g, c.b );
    for(int y = 30; y < 100; ++y)
	for(int x = 100; x < 200; ++x)
	    PutColor( gScreenSurface, x, y, newColor );

//    line(&c, 200, 300, 300, 400);
//    line(&c, 200, 300, 300, 440);


#define B_TOP 100
#define B_BOTTOM 400
#define B_LEFT 100
#define B_RIGHT 500
    moveto(B_RIGHT, B_BOTTOM);
    lineto(&c, B_RIGHT, B_TOP);
    lineto(&c, B_LEFT, B_TOP);
    lineto(&c, B_LEFT, B_BOTTOM);
    lineto(&c, B_RIGHT, B_BOTTOM);

    SDL_UnlockSurface( gScreenSurface );// $(B%m%C%/$r2r=|(B
}

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
    Vec2f& operator=(const Vec2f& rhs){x = rhs.x; y = rhs.y; return *this;};
    Vec2f& operator+=(const Vec2f& rhs){x += rhs.x; y += rhs.y; return *this;};
    Vec2f& operator-=(const Vec2f& rhs){x -= rhs.x; y -= rhs.y; return *this;};
};
Vec2f operator+(Vec2f lhs, const Vec2f& rhs){return lhs += rhs;}
Vec2f operator-(Vec2f lhs, const Vec2f& rhs){return lhs -= rhs;}

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


bool inside(const Vec2f& v, const Edge2f& boundary)
{
    Vec2f tmp0 = v - boundary[0];
    Vec2f tmp1 = boundary[1] - boundary[0];
    return tmp0.x * tmp1.y - tmp0.y * tmp1.x >= 0;
}
Vec2f intersect(const Vec2f& v0, const Vec2f& v1, const Edge2f& boundary)
{

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
    Edge2f top = {{B_RIGHT, B_TOP}, {B_LEFT, B_TOP}};
    Edge2f left = {{B_LEFT, B_TOP}, {B_LEFT, B_BOTTOM}};
    Edge2f bottom = {{B_LEFT, B_BOTTOM}, {B_RIGHT, B_BOTTOM}};
    Edge2f right = {{B_RIGHT, B_BOTTOM}, {B_RIGHT, B_TOP}};
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



// $(BEI$j$D$V$9(B
void FillScreen( SDL_Color Color )
{
    // $(B?'$r;XDj$7$?%T%/%;%k%U%)!<%^%C%HMQ$N?'>pJs$KJQ49(B
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
    // $(B=i4|2=(B
    if( !Init() ){
	printf( "error.:%s\n", SDL_GetError());
	return 0;
    }

    // $(B%a%$%s%k!<%W(B
    while( 1 ){
	// WhiteOut();
	render();
	// $(B2hLL$r99?7$7$^$9(B
	SDL_Flip( gScreenSurface );

	// $(B%$%Y%s%H=hM}(B
	if( !PollEvent() )break;
    }

    // $(B=*N;=hM}(B
    End();

    return 0;
}

// $(B=i4|2==hM}(B
bool Init()
{
    // SDL$(B$N=i4|2=(B
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
	printf(SDL_GetError());
	return false;
    }
    // $(B%-%c%W%7%g%s$N@_Dj(B
    SDL_WM_SetCaption( GAME_CAPTION, NULL );

    // $(B%&%#%s%I%&$N=i4|2=(B
    gScreenSurface = SDL_SetVideoMode(
	SCREEN_WIDTH,
	SCREEN_HEIGHT,
	SCREEN_BPP,
	SDL_SWSURFACE//|SDL_FULLSCREEN
	);

    // $(B%^%&%9%+!<%=%k$r>C$9>l9g$O(B
    // SDL_ShowCursor(SDL_DISABLE );
    return gScreenSurface != NULL;
}

// $(B=*N;=hM}(B
void End()
{
    // SDL$(B$N=*N;(B
    SDL_Quit();
}

// $(B%$%Y%s%H=hM}(B
bool PollEvent()
{
    SDL_Event ev;
    SDLKey *key;
    while(SDL_PollEvent(&ev) )
    {
	switch(ev.type){
	    case SDL_QUIT:// $(B%&%#%s%I%&$N(B$(BNW%\%?%s$,2!$5$l$?;~$J$I(B
		return false;
		break;
	    case SDL_KEYDOWN:// $(B%-!<%\!<%I$+$i$NF~NO$,$"$C$?;~(B
	    {
		key=&(ev.key.keysym.sym); // $(B$I$N%-!<$,2!$5$l$?$+$r<hF@(B
		if(*key==27){// ESC$(B%-!<(B
		    return false;
		}
	    }
	    break;
	}
    }
    return true;
}
