/*
 * drawline.h - 
 */

#ifndef DRAWLINE_INC_SEEN
#define DRAWLINE_INC_SEEN

// drawline
EXTERN void line(const SDL_Color* c, int x0, int y0, int x1, int y1);
EXTERN void moveto(int x, int y);
EXTERN void lineto(const SDL_Color* c, int x, int y);
EXTERN void moveto(const Vec2f& v);
EXTERN void lineto(const SDL_Color& c, const Vec2f& v);
EXTERN void lineto(const Vec2f& v);
EXTERN void lineto(int x, int y);






#endif // DRAWLINE_INC_SEEN
