#ifndef TGA_H
#define TGA_H

#include <common/common.h>
#include <common/bmmath.h>
#include <engine/video/pic.h>

rgba_t* File_LoadTargaFile( const char* path, uint16_t* width, uint16_t* height, pic_t* pic );

#endif
