#ifndef VEC_H_INC_SEEN

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
  Vec2f operator/(float rhs){return Vec2f(x/rhs, y/rhs);};
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
  explicit Vec4f(){};
  explicit Vec4f(float x0, float y0, float z0, float w0) : x(x0), y(y0), z(z0), w(w0){};
  Vec4f& operator=(const Vec4f& rhs){x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w; return *this;};
  Vec4f& operator+=(const Vec4f& rhs){x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this;};
  Vec4f& operator-=(const Vec4f& rhs){x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this;};
  Vec4f& operator*=(float rhs){x *= rhs; y *= rhs; z *= rhs; w *= rhs; return *this;};
  Vec4f& operator/=(float rhs){x /= rhs; y /= rhs; z /= rhs; w /= rhs; return *this;};
  Vec4f operator/(float rhs){return Vec4f(x/rhs, y/rhs, z/rhs, w/rhs);};
  Vec4f operator-(){return Vec4f(-x, -y, -z, -w);};
  float norm2()const {return x*x + y*y + z*z + w*w;}
  float norm()const {return sqrt(x*x + y*y + z*z + w*w);}
  void normalize(){float abs = norm(); *this /= abs;}
  float dot(const Vec4f& rhs){return  x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;}
};
typedef Vec2f Edge2f[2];


#endif // VEC_H_INC_SEEN
