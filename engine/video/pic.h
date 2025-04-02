
#ifndef PIC_H
#define PIC_H

#include <common/common.h>
#include <common/bmmath.h>

typedef enum pic_flags_e
{
   PIC_FONT    = 0x0002
}
pic_flags_t;

typedef struct pic_font_s
{
   uint8_t width, height;
   uint8_t margin;
}
pic_font_t;

typedef struct pic_s
{
   char        name[64];
   pic_flags_t flags;
   uint16_t    width, height;
   rgba_t*     buffer;
   uint        length;
   pic_font_t  font;
}
pic_t;

pic_t*   R_LoadPicFromDisk( const char* name );
uint     R_LoadTextureFromPic( pic_t* pic );
uint     R_LoadTexture( const char* name, pic_flags_t flags );

#endif
