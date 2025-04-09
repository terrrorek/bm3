#include <game/cfg.h>
#include <common/cmd.h>
#include <common/alloc.h>
#include <common/bmstring.h>
#include <game/console.h>

void Cfg_Exec( const char* path )
{
   char*    data, * ptr, * last;
   char*    line;
   int      i, linecount, len;

   data = (char*) File_Read( path, NULL );
   if( !data )
   {
      Cmd_SoftErrorf( "failed to execute cfg \"%s\" (file failure).\n",
         path );
      return;
   }

   // split by lines
   last = ptr = data;

   for( linecount = 0; *ptr; ptr++ )
      if( *ptr == '\n' ) ++linecount;

   if( !linecount )
   {
      Con_Execute( data );
      return;
   }

   ptr = data;
   for( i = 0; i < linecount; i++ )
   {
      while( *ptr != '\n' && *ptr ) ptr++;
      if( !*ptr ) break;

      len = ptr - 1 - last;
      line = Mem_Alloc( len + 1 );
      memset( line, 0, len + 1 );
      BM_strncpy( line, last, len );
      Con_Execute( line );
      Mem_Free( line );

      *ptr++ = 0;
      last = ptr;
   }
}
