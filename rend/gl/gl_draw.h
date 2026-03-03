#ifndef GL_DRAW_H
#define GL_DRAW_H
#include <common/define.h>
#include <rend/gl/gl.h>
#include <rend/gl/gl_sprite.h>

#define GL_MAX_DRAWCALLS VID_MAX_SPRITES*2

typedef struct gl_drawcall_s
{
   vid_transform_t   transform;
   gl_pipeline_t*    pp;
   gl_sprite_t*      sprite;
}
gl_drawcall_t;

void GL_PushDrawCall( gl_drawcall_t* call );
void GL_ClearDrawCalls( void );
void GL_ProcessDrawCalls( gl_renddata_t* data );

#endif
