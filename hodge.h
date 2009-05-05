/*
 * hodge.h - 
 */

#ifndef HODGE_INC_SEEN
#define HODGE_INC_SEEN

// hodge

// input Vec2f * 3, clips(left, right, top, bottom)
// output return dst_n, Vec2f * dst_n
EXTERN int hodge(const Vec2f* v, float* clips, Vec2f* dst);

#endif // HODGE_INC_SEEN
