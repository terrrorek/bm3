
#ifndef VID_H
#define VID_H

#include <common/common.h>
#include <SDL.h>

#undef bool

typedef enum rend_context_e
{
   RCTX_GL,
   RCTX_SOFT,

   RCTX_INVALID,
} rend_context_t;

typedef struct vidstate_s
{
   SDL_Window*    window;
   SDL_Event      event;
   int            width, height;
   double         deltaTime;
   rend_context_t context;

   // gl
   SDL_GLContext  glContext;

   // software
   SDL_Surface*   rendSurface;
} vidstate_t;

extern vidstate_t vidState;
extern bool shouldExit;

void Vid_Init( void );
void Vid_Quit( void );
void Vid_Update( void );
void Vid_Draw( void );

#endif
