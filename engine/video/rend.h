#ifndef VID_REND_H
#define VID_REND_H

#include <SDL3/SDL.h>
#include <common/define.h>
#include <common/dll.h>
#include <cglm/cglm.h>

struct vidstate_s;
struct vid_rendstate_s;

struct vid_sprite_s;
struct vid_spriteatlas_s;

typedef struct vid_vertex_s
{
   vec3 position;
   vec2 uv;
}
vid_vertex_t;

typedef enum vid_rend_e
{
   VID_REND_SOFT = 0,
   VID_REND_GL = 1,
}
vid_rend_t;

typedef struct vid_rendfuncs_s
{
   void  (*Init)( struct vid_rendstate_s* );
   void  (*Quit)( struct vid_rendstate_s* );
   void  (*Update)( struct vid_rendstate_s* );

   bool  (*LoadSprite)( struct vid_sprite_s*, struct vid_spriteatlas_s*, void* );
   bool  (*FreeSprite)( struct vid_sprite_s* );
}
vid_rendfuncs_t;

typedef struct vid_rendstate_s
{
   vid_rend_t              backend;
   vid_rendfuncs_t         func;
   void*                   dll;

   struct vidstate_s*      vid;
   struct vid_renddata_s*  data;
}
vid_rendstate_t;

typedef void (*vid_rendload_t)( dll_export_t*, vid_rendfuncs_t* );

void Rend_Init( vid_rendstate_t* rend, vid_rend_t backend, struct vidstate_s* vid );
void Rend_Quit( vid_rendstate_t* rend );
void Rend_Update( vid_rendstate_t* rend );

#endif
