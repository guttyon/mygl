/*
 * util.h - 
 */

#ifndef UTIL_INC_SEEN
#define UTIL_INC_SEEN

// util
EXTERN int debprint;
EXTERN void debprintf(const char* format, ...);
EXTERN void nanoslp(unsigned long nsec);
EXTERN void sdlerror(const char* str = 0);

#endif // UTIL_INC_SEEN
