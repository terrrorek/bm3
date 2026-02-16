#ifndef VID_REND_H
#define VID_REND_H

#include <SDL3/SDL.h>
#include <common/define.h>
#include <cglm/cglm.h>

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

typedef struct vid_rendsoft_s
{
   SDL_Surface*   surface;
}
vid_rendsoft_t;
typedef struct vid_rendgl_s
{
   SDL_GLContext  gl_context;
   uint           quad_vao;
   uint           quad_vbuf, quad_ibuf;
   uint           base_pipeline;
}
vid_rendgl_t;

typedef struct vid_rendstate_s
{
   SDL_Window* window;
   vid_rend_t  backend;
   union
   {
      vid_rendsoft_t soft;
      vid_rendgl_t   gl;
   }
   data;
}
vid_rendstate_t;

void Rend_Init( vid_rendstate_t* rend, vid_rend_t backend, SDL_Window* window );
void Rend_Quit( vid_rendstate_t* rend );
void Rend_Update( vid_rendstate_t* rend );

/* gl */
void RendGL_Init( vid_rendstate_t* rend, SDL_Window* window );
void RendGL_Quit( vid_rendstate_t* rend );
void RendGL_Update( vid_rendstate_t* rend );

/* soft */
void RendSoft_Init( vid_rendstate_t* rend, SDL_Window* window );
void RendSoft_Quit( vid_rendstate_t* rend );
void RendSoft_Update( vid_rendstate_t* rend );

#endif
