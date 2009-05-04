#include <SDL/SDL.h>

#include "basetype.h"
#include "setting.h"
#include "util.h"
#include "sdluser.h"
#include "mygl.h"

#define GAME_CAPTION  "SDLTutorial"


// 初期化処理
bool Init()
{
    sdlerror("-1");
    // SDLの初期化
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
      sdlerror();
	return false;
    }
    // 何故かエラーがでるので、ここでクリアしておく。
    SDL_ClearError();

    sdlerror("0");
    // キャプションの設定
    SDL_WM_SetCaption( GAME_CAPTION, NULL );
    sdlerror("1");

    // ウィンドウの初期化
    gScreenSurface = SDL_SetVideoMode(
	SCREEN_WIDTH,
	SCREEN_HEIGHT,
	SCREEN_BPP,
	SDL_SWSURFACE//|SDL_FULLSCREEN
	);
    sdlerror("2");

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
	    case SDL_QUIT:// ウィンドウの閉じる(×)ボタンが押された時など
		return false;
		break;
	    case SDL_KEYDOWN:// キーボードからの入力があった時
	    {
		key=&(ev.key.keysym.sym); // どのキーが押されたかを取得
		if(*key==27 || *key=='q'){// ESCキー
		    return false;
		}
		else if(*key=='s'){// ESCキー
		  debprint = debprint ? 0 : 1;
		}
	    }
	    break;
	}
    }
    return true;
}

int main(int argc, char* argv[])
{
  sdlerror();
    // 初期化
    if( !Init() ){
      sdlerror();
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


