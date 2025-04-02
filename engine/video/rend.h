
#ifndef REND_H
#define REND_H

#include <common/common.h>
#include <common/bmmath.h>
#include <video/vid.h>
#include <video/pic.h>

typedef struct rend_imports_s
{
   void (*Rend_Unload)( void );
   void (*Rend_Update)( void );
   void (*Rend_Clear)( bool depth );
   void (*Rend_SetClearValues)( rgba_t color, float depth );

   uint (*Rend_CreateTexture)( char* name, pic_t* pic );
   void (*Rend_DestroyTexture)( uint index );

   void (*Rend_DrawPic)( uint pic, int x, int y, int w, int h );
   void (*Rend_DrawChar)( uint pic, char ch, int x, int y, int w, int h );
   void (*Rend_DrawString)( uint pic, int x, int y, int w, int h, const char* fmt, ... );
}
rend_imports_t;

extern rend_imports_t rendImports;

void  Rend_LoadRenderer( const char* name );
void  Rend_UnloadRenderer( void );
void* Rend_GetFuncHandle( const char* name );

#endif
