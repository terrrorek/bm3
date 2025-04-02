
#include <video/pic.h>
#include <video/rend.h>
#include <common/bmstring.h>
#include <common/cmd.h>
#include <common/alloc.h>
#include <filesys/fsys.h>

pic_t* R_LoadPicFromDisk( const char* name )
{
   pic_t*   pic;

   pic = (pic_t*) Mem_Alloc( sizeof(pic_t) );
   pic->buffer = File_LoadImage( name, &pic->width, &pic->height, pic );
   if( !pic->buffer )
      Cmd_Errorf( "failed to read pic \"%s\" from disk.\n", name );
   BM_strncpy( pic->name, name, sizeof(pic->name) );

   return pic;
}

uint R_LoadTextureFromPic( pic_t* pic )
{
   uint  index;

   index = rendImports.Rend_CreateTexture( pic->name, pic );
   return index;
}

uint R_LoadTexture( const char* name, pic_flags_t flags )
{
   pic_t* pic = R_LoadPicFromDisk( name );
   if( !pic )
      return 0;

   pic->flags = flags;
   return R_LoadTextureFromPic( pic );
}
