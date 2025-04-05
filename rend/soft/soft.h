
#ifndef GL_H
#define GL_H

#include <engine/video/rend.h>
#include <common/common.h>
#include <engine/game/dll.h>
#include <engine/video/pic.h>

#if defined( _WIN32 )
   #define EXPORT __declspec(dllexport)
#elif defined( __linux__ )
   #define EXPORT __attribute__((visibility("default")))
#endif

#define RAWPOS(x, y, w) y * w + x
#define POS(x, y) RAWPOS(x, y, state->width)

#define MAX_SOFTTEXTURES 2048

typedef struct softtex_s
{
   char     name[32];
   uint     index;
   pic_t*   pic;
   uint     width, height;
}
softtex_t;

extern softtex_t           textures[MAX_SOFTTEXTURES];

extern uint32_t*           screenBuffer;
extern float*              depthBuffer;

extern rgba_t              clearColor;
extern float               clearDepth;

extern eng_exports_t*      engFuncs;
extern vidstate_t*         state;

inline static uint32_t Rend_PackColor( rgba_t col )
{
   return (uint32_t)
      ( col[0] << 8 | col[1] << 16 | col[2] << 24 | col[3] );
}

inline static void Rend_SetBuffer( int x, int y, rgba_t col )
{
   uint32_t packed = Rend_PackColor( col );
   if( (x >= state->width || x < 0) || (y >= state->height || y < 0) )
      return;
   screenBuffer[POS(x,y)] = packed;
}

inline static void Rend_SetDepth( int x, int y, float depth )
{
   if( (x >= state->width || x < 0) || (y >= state->height || y < 0) )
      return;
   depthBuffer[POS(x,y)] = depth;
}

uint Rend_CreateTexture( char* name, pic_t* pic );
void Rend_DestroyTexture( uint index );

void Rend_DrawPic( uint pic, int x, int y, int w, int h );
void Rend_DrawChar( uint pic, char ch, int x, int y, int w, int h );
void Rend_DrawString( uint pic, int x, int y, int w, int h, const char* fmt, ... );

#endif
