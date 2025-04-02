
#include <filesys/fsys.h>
#include <filesys/tga.h>
#include <filesys/raw.h>
#include <common/cmd.h>
#include <common/bmstring.h>

#ifdef _WIN32
   #include <io.h>
   #define access _access
   #define F_OK 0
#else
   #include <unistd.h>
#endif

char  searchPaths[32][256];
byte  spathCount = 0;

void  File_Init( void )
{
   File_AddSearchPath( "." );
   File_AddSearchPath( "./data" );
}

void  File_AddSearchPath( const char* path )
{
   BM_strncpy( searchPaths[spathCount++], path, 256 );
}

bool File_Exists( const char* path )
{
   return ( access(path, F_OK) == 0 );
}

byte* File_Read( const char* path, uint* len )
{
   int   i;
   char  finalPath[256];

   for( i = 0; i < spathCount; i++ )
   {
      BM_snprintf( finalPath, 256, "%s/%s", searchPaths[i], path );

      // check raw files
      if( File_Exists( finalPath ) )
         return File_ReadRawBinary( finalPath, len );
   }

   Cmd_SoftErrorf( "failed to find file \"%s\".\n", path );
   return NULL;
}

void  File_Write( const char* path, byte* data, uint32_t size )
{
   (void) path, (void) data, (void) size;
}

rgba_t* File_LoadImage( const char* path, uint16_t* width, uint16_t* height, pic_t* pic )
{
   rgba_t*  data;
   char*    ext;

   data = NULL;
   ext = strrchr( path, '.' ) + 1;

   if( !BM_strcmp(ext, "tga") )
      data = File_LoadTargaFile( path, width, height, pic );


   return data;
}
