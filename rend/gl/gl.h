
#ifndef GL_H
#define GL_H

#include <engine/video/rend.h>
#include <common/common.h>
#include <common/bmmath.h>
#include <engine/game/dll.h>
#include "glad.h"

#if defined( _WIN32 )
   #define EXPORT __declspec(dllexport)
#elif defined( __linux__ )
   #define EXPORT __attribute__((visibility("default")))
#endif

#define MAX_GLTEXTURES 2048

typedef struct gltexture_s
{
   char     name[32];
   uint     index;
   uint     handle;
   uint     width, height;
   uint     type;
   pic_t*   pic;
}
gltexture_t;

extern gltexture_t      textures[MAX_GLTEXTURES];

extern eng_exports_t*   engFuncs;
extern vidstate_t*      state;

extern uint             quadBuffer, quadIndices;
extern uint             quadArray;
extern uint             picShader;

extern mat4             modelMatrix, viewMatrix, projectionMatrix;
extern vec2             u_top_uv, u_down_uv;
extern ivec2            u_screen_wh;

static void Rend_RGBAToVec( rgba_t color, vec4* vec )
{
   vec4 a = {color[0], color[1], color[2], color[3]};
   vec4 b = {255.f, 255.f, 255.f, 255.f};

   glm_vec4_div( a, b, *vec );
}

void Rend_UpdateUniforms( uint program );
void Rend_InitShaders( void );
void Rend_2DProjection( void );
void Rend_ModelMatrix( vec3 position, vec3 rotation, vec3 scale );

uint Rend_CreateTexture( char* name, pic_t* pic );
void Rend_DestroyTexture( uint index );
void Rend_DrawPic( uint pic, int x, int y, int h, int w );
void Rend_DrawChar(uint pic, char ch, int x, int y, int w, int h);

#endif
