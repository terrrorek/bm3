#ifndef VID_VID_H
#define VID_VID_H

#include <SDL3/SDL.h>
#include <common/define.h>
#include <video/rend.h>

typedef struct vidstate_s
{
   SDL_Window*       window;
   uint16_t          width, height;
   vid_rendstate_t   rend;
}
vidstate_t;

extern vidstate_t vid;

void Vid_Init( void );
void Vid_Quit( void );
void Vid_Update( void );

#endif
