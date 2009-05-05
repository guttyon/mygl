#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <SDL/SDL.h>


#include "basetype.h"
#include "util.h"


int debprint = 1;
void debprintf(const char* format, ...)
{
  va_list ap;
  va_start(ap, format);
  if(debprint)
  {
    vprintf(format, ap);
  }
  va_end(ap);
  return;
}

void nanoslp(unsigned long nsec)
{
  struct timespec treq, trem;

  treq.tv_sec = (time_t)0;
  treq.tv_nsec = nsec;
  
  nanosleep(&treq, &trem);
}

void sdlerror(const char* str)
{
  if (strlen(SDL_GetError()) != 0) {
    if(str)printf(str);
    printf("SDL_Error : %s\n", SDL_GetError());
    exit(-1);
    SDL_ClearError();
  }
}
