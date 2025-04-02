#include <filesys/tga.h>
#include <filesys/fsys.h>
#include <common/cmd.h>
#include <common/alloc.h>

#define TGA_ERROR(x)\
   { error = x; goto error; }

typedef struct tga_header_s
{
   byte  idLength;
   byte  mapType;
   byte  imageType;
   byte  colorSpec[5];
   byte  imageSpec[10];
}
tga_header_t;

static void TGA_FlipImage( rgba_t* data, int width, int height )
{
   int         i, j;
   rgba_t*     final;

   final = (rgba_t*) Mem_Alloc( width * height * sizeof(rgba_t) );

   for( i = height - 1, j = 0; i >= 0 && j < height; i--, j++ )
   {
      memcpy( final[i * width], data[j * width], width * sizeof(rgba_t) );
   }

   memcpy( data, final, width * height * sizeof(rgba_t) );
   Mem_Free( final );
}

rgba_t* File_LoadTargaFile( const char* path, uint16_t* width, uint16_t* height, pic_t* pic )
{
   rgba_t*  final;
   byte*    raw, * data;
   uint     rawlen;
   char*    error;
   int      depth, size, i, j;

   final = NULL;
   raw = File_Read( path, &rawlen );
   if( !raw )
      TGA_ERROR( "file read failure." )

   tga_header_t* header = (tga_header_t*) raw;

   if( header->mapType )
      TGA_ERROR( "image contains colormap. engine only supports uncompressed true-color." )

   // TODO: more image types
   if( header->imageType != 2 )
      TGA_ERROR( "engine only supports uncompressed true-color." )

   if( (header->colorSpec[1] != 0) && pic )
   {
      // tga is font!
      pic->flags |= PIC_FONT;
      pic->font.margin = header->colorSpec[0];
      pic->font.width = header->colorSpec[1];
      pic->font.height = header->colorSpec[2];
   }

   *width = *(uint16_t*)(&header->imageSpec[4]);
   *height = *(uint16_t*)(&header->imageSpec[6]);
   depth = *(byte*)(&header->imageSpec[8]);

   data = (byte*)( (uintptr_t)raw + sizeof(tga_header_t) );

   if( depth == 32 )
   {
      size = (*width) * (*height) * 4;
      final = Mem_Alloc( (*width) * (*height) * sizeof(rgba_t) );
      for (i = 0, j = 0; i < size; i += 4, j++)
      {
         rgba_t col;
         col[0] = data[i];
         col[1] = data[i+1];
         col[2] = data[i+2];
         col[3] = data[i+3];
         memcpy( final[j], col, sizeof(col) );
      }
   }
   else if( depth == 24)
   {
      size = (*width) * (*height) * 3;
      final = Mem_Alloc( (*width) * (*height) * sizeof(rgba_t) );
      for (i = 0, j = 0; i < size; i += 3, j++)
      {
         rgba_t col;
         col[0] = data[i];
         col[1] = data[i+1];
         col[2] = data[i+2];
         col[3] = 255;
         memcpy( final[j], col, sizeof(col) );
      }
   }
   else
      TGA_ERROR( "unsupported pixel depth." )

   TGA_FlipImage( final, *width, *height );

   return final;

error:
   if( final )
      Mem_Free( final );
   if( raw )
      Mem_Free( raw );
   Cmd_SoftErrorf( "failed to load targa file. %s\n", error );
   return NULL;
}
