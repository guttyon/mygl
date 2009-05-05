/*
 * intersect.h - 
 */

#ifndef INTERSECT_INC_SEEN
#define INTERSECT_INC_SEEN

// intersect
EXTERN Vec2f intersect_line_line(const Vec2f& v0, const Vec2f& v1, const Edge2f& boundary);


// 水平線との交点までの、線分v0v1に対するパラメータt[0.0 ... 1.0]を返す。
// 交点があることを前提とする。
// 
EXTERN float intersect_line_hline(const Vec2f& v0, const Vec2f& v1, const float y);
EXTERN Vec2f intersect_line_plane(const Vec2f& v0, const Vec2f& v1, const Edge2f& boundary);

#endif // INTERSECT_INC_SEEN
