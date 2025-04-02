
#ifndef FSYS_H
#define FSYS_H

#include <common/common.h>
#include <common/bmmath.h>
#include <engine/video/pic.h>

void  File_Init( void );
void  File_AddSearchPath( const char* path );

byte* File_Read( const char* path, uint* len );
void  File_Write( const char* path, byte* data, uint size );

rgba_t* File_LoadImage( const char* path, uint16_t* width, uint16_t* height, pic_t* pic );

#endif
