/*
 * matstack.h - 
 */

#ifndef MATSTACK_INC_SEEN
#define MATSTACK_INC_SEEN

// matstack
#if 0
struct mat22d
{
  double m[4];
};
struct mat33d
{
  double m[9];
};
struct mat44d
{
  double m[16];
};
#else
typedef double mat22d[ 4];
typedef double mat33d[ 9];
typedef double mat44d[16];
#endif // 0
enum E_MATMODE
  {
    MATMODE_WORLD,
    MATMODE_VIEW,
    MATMODE_PROJ
  };

EXTERN void matmode(E_MATMODE mode);
EXTERN void loadidentity();
EXTERN void pushmat();
EXTERN void popmat();
EXTERN void setmat(const mat44d* mat);
EXTERN void getmat(mat44d* mat);


EXTERN void add_m33_m33(mat33d* pdst, const mat33d* pa, const mat33d* pb);
EXTERN void add_m44_m44(mat44d* pdst, const mat44d* pa, const mat44d* pb);
EXTERN void sub_m33_m33(mat33d* pdst, const mat33d* pa, const mat33d* pb);
EXTERN void sub_m44_m44(mat44d* pdst, const mat44d* pa, const mat44d* pb);


EXTERN void mul_m33_m33(mat33d* pdst, const mat33d* pa, const mat33d* pb);

EXTERN void mul_m44_m44(mat44d* pdst, const mat44d* pa, const mat44d* pb);

// 右にかけていく。
EXTERN void mulmat(const mat44d* m);

EXTERN void transpose(mat33d* pdst, const mat33d* pa);
EXTERN void transpose(mat44d* pdst, const mat44d* pa);
// 余因子行列
EXTERN void cofactor(mat33d* pdst, const mat44d* pa, int i, int j);


// ゲームプログラミングのための3Dグラフィックス数学
// p49
EXTERN double determinant(const mat22d* pa);
EXTERN double determinant(const mat33d* pa);
EXTERN double determinant(const mat44d* pa);

// p50
EXTERN void inverse(mat22d* pdst, const mat22d* pa);
EXTERN void inverse(mat33d* pdst, const mat33d* pa);
EXTERN void inverse(mat44d* pdst, const mat44d* pa);

// p63, p89
// 一般の4x4ではなく、モデル変換（移動、拡大、回転）だけの4x4の変換。
// 平面の変換に使う。
// p67
// 法線の変換は、w=0にして、(M^-1)^Tで変換。
EXTERN void world_inverse();


// p77
// 位置ベクトルはw=1
// p64
// 方向ベクトルはw=0
// p83, 85
// 平面の表現は、ax + by + cz + d = 0とし、(a, b, c, d)とする。


EXTERN void scale(float x, float y, float z);
EXTERN void translate(float x, float y, float z);
EXTERN void rotate_x(float theta);
EXTERN void rotate_y(float theta);
EXTERN void rotate_z(float theta);
EXTERN void rotate(float x, float y, float z, float theta);




EXTERN void matprint(const mat33d& pa);
EXTERN void matprint(const mat44d& pa);
EXTERN void matprint(E_MATMODE mode);
EXTERN void matprint();




#endif // MATSTACK_INC_SEEN
