
#include <filesys/raw.h>
#include <engine/common/cmd.h>
#include <engine/common/alloc.h>

byte* File_ReadRawBinary( const char* path, uint* len )
{
   FILE* handle;

   if(!( handle = fopen( path, "rb" ) ))
   {
      Cmd_Errorf( "read raw binary failed for file \"%s\"\n", path );
   }

   uint length;
   byte* buffer;

   fseek( handle, 0, SEEK_END );
   length = ftell( handle );
   rewind( handle );

   buffer = (byte*) Mem_Alloc( length + 1 );
   memset( buffer, 0, length + 1 );

   fread( buffer, 1, length, handle );
   fclose( handle );

   if( len )
      *len = length;
   return buffer;
}
