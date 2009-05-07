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


//           |Y
//           |
//           |
//           |
//           /--------X
//          /
//         /Z
// TODO:
// 処理順序は、以下を参照
// Jim Blinn's Corner A Trip Down the Graphics Pipeline, p176
// コンピュータグラフィックス 理論と実践, p880
// OpenGLの神髄, p76 .. p81
// 頂点の項目は、位置、法線、頂点カラー、テクスチャ座標
void draw_primitive(E_PRIMITIVE ptype, v4f* pos, v4f* normal, v4f* col, v4f* texcood, int vnum)
{
  assert(vnum < 50);
  v4f pos0[50];
  // 位置座標変換
  // 変換後の同次座標表現(x,y,z,w)の値の範囲は、
  // x,y,zは、[-w .. w]の範囲になるので、
  // w除算後のユークリッド座標ではx,y,zは、[-1 .. 1]の範囲になる。
  // OpenGLプログラミングガイド, p724
  // （または、そうなるように透視変換行列がスケールされているようにも見える。
  // 同次座標でスケールさせても空間位置は変わらないので）
  {
    mat44d m;
    getmodel2perspective(&m);
    for(int i = 0; i < vnum; ++i)
    {
      mvmul(pos0 + i, &m, pos + i);
      vecprint(pos0[i]);
    }
  }

  // OpenGLプログラミングガイド, p639
  // texcood自動生成モードが有効なら、
  // texcood座標変換の前に、texcoodを自動生成しないといけない。
  // その後、さらにtexcood座標変換が行われる。
  // 下の説明にあるEYE_PLANEは視点座標を使用するので、
  // 上の位置座標変換から透視変換を削除し、
  // texcood自動生成後に、改めて透視変換をする必要がある。
  // 
  // OpenGLプログラミングガイド, p411, 415
  // 自動生成モードは、３種類。OBJ_PLANE, EYE_PLANE, SPHERE_MAP
  // OBJ_PLANE: オブジェクト座標を(x,y,z,w)とし、生成パラメータを(a,b,c,d)とする。
  // 生成される座標 = a * x + b * y + c * z + d * w
  // つまり、w = 1とし、(a, b, c, d)を平面のパラメータと考えると、
  // 平面からの距離をテクスチャ座標に生成できる。（ターゲットはs,t,r,qの選択されたもの）
  // EYE_PLANE: 視点座標を(x,y,z,w)とし、生成パラメータを(a,b,c,d)とする。
  // 生成される座標 = a' * x + b' * y + c' * z + d' * w
  // ここで、(a', b', c', d') = (a, b, c, d)M^-1
  // Mはモデルビュー行列である。
  // EYE_PLANEの例は、OpenGLの神髄のシャドウマップ例を参照。
  

  
  // 法線座標変換(位置座標変換の逆転置行列で)
  // texcood座標変換(テクスチャ行列で)


  // 早めに裏面カリングの後に座標変換したほうが効率的そうだが、
  // 法線を求めるための外積計算と視線ベクトルとの内積計算が重そう。


  // clip（hodge処理）, jim本の高速クリップの13章と、 16章参照。
  // 同次座標そのままでクリップする。
  // クリップ必要時に補間するときは、同次座標でもよい。
  // 補間時は、全ての要素を補間する。
  // このときに重要な役割を果たす概念が、x,y,zで別に存在する。
  // z方向について：
  // x,yはユークリッド座標において、0.0<Z<1.0に存在し、
  // 同次座標においては、0<z<wに存在する。  
  // 点と平面の距離を同次座標での内積計算で表現できる。
  // w倍の距離=(x,y,z,w)・(a,b,c,d), 3Dグラフィックス数学,p84
  // 通常、w=1で計算するので、上式は正しく距離をだす。
  // ただし、ここは透視変換後なのでw=1ではないので、wで割らないと距離はだせない。
  // しかし、ここで知りたいのは面のどちら側に点があるか？ということ。
  // w倍の距離でも符号さえわかればそれでよい。
  // zのクリップ面は、(a,b,c,d)=(0,0,1,0) and (0,0,-1,1)である。
  // 内積を実行し、zとw-zの符号がわかればよくなる。
  // x,y方向について：
  // x,yはユークリッド座標において、-1.0<X,Y<1.0に存在し、
  // 同次座標においては、-w<x,y<wに存在する。
  // z座標のときは、負には有効な点は存在しなかったが、x,yでは存在する。
  // ここでも、z方向のように、クリップ面との距離ではなく、
  // クリップ面のどちらにあるかということを知りたいのだが、
  // 負の側に点が存在しない方が計算効率がよい。
  // そこで、Jim本の16章で示されているように、
  // 一旦、x,yを0<x,y<1になるように変換し、
  // クリップ後に、x,yに逆変換を与え、元に戻すというやりかただ。
  // x,yだけ、変換するのであって、z,wはそのままでよい。
  // 変換処理が必要なので、負荷が増えているようだが、
  // この変換は透視変換等の変換に混ぜておけばよく、
  // 逆変換についても、ピクセル座標への変換と組み合わせれば、負荷は増えない。


  // 頂点に新たな要素として、１を追加。

  
  // w除算（全ての要素をw除算）
  // クリップで行うw除算は、位置要素のみ除算した。
  // ここでは全ての要素を除算する点でクリップ時のw除算とは異なる。
  // 位置については2重で除算しちゃダメなことに注意


  
  // 裏面カリング、w除算後じゃないとダメな気がする。
  // 2次元座標系で法線計算できるので、3次元で計算するより楽そう。

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

  {
    matmode(MATMODE_WORLD);
    loadidentity();
    matmode(MATMODE_VIEW);
    loadidentity();
    lookat(0., 0., 0.,          // eye
           0., 0., -1.,         // center
           0., 1., 0.);         // up
    matmode(MATMODE_PROJ);
    loadidentity();
    perspective(60., 640. / 480., 1., 1024.); // fovy, aspect, near, far
    // matprint(MATMODE_PROJ);exit(-1);

    // pespectiveにあわせたzにする。
    v4d pos[] = {
      {0.0,   0.0, -2.0, 1.0},
      {-0.2, -0.2, -2.0, 1.0},
      {0.2,  -0.2, -2.0, 1.0},
    };
    draw_primitive(PRIMITIVE_TRIANGLE_STRIP, pos, pos, pos, pos, 3);
  }

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
