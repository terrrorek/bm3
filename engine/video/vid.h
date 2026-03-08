#ifndef VID_VID_H
#define VID_VID_H

#include <SDL3/SDL.h>
#include <common/define.h>
#include <common/cvar.h>
#include <video/rend.h>

typedef struct vidstate_s
{
   SDL_Window*          window;
   uint16_t             width, height;
   vid_rendstate_t      rend;
   struct vid_sprite_s* checker;
}
vidstate_t;

extern vidstate_t vid;
extern cvar_t     vid_rate;
extern double     last_render, render_time;

void Vid_Init( void );
void Vid_Quit( void );
void Vid_Update( void );

void Vid_DrawSprite( struct vid_sprite_s* sprite, vid_transform_t* transform );

#endif
