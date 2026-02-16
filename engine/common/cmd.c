#include <SDL3/SDL.h>
#include <common/cmd.h>

#include <stdio.h>
#include <stdarg.h>

void Cmd_Printf( const char* fmt, ... )
{
   va_list  args;
   va_start( args, fmt );
   vfprintf( stdout, fmt, args );
   va_end( args );
}

void Cmd_Errorf( const char* fmt, ... )
{
   char*    buffer;
   size_t   buflen;
   va_list  args;

   va_start( args, fmt );
   buflen = vsnprintf( NULL, 0, fmt, args );
   va_end( args );

   buffer = Mem_Alloc( buflen + 2 );

   va_start( args, fmt );
   vsnprintf( buffer, buflen + 2, fmt, args );
   va_end( args );

   SDL_ShowMessageBox( &(SDL_MessageBoxData){
      .title = "Cmd_Errorf",
      .message = buffer,
      .buttons = (SDL_MessageBoxButtonData[]){
         [0] = {
            .text = "OK"
         }
      },
      .numbuttons = 1
   }, NULL );

   fprintf( stderr, "ERROR: %s\n", buffer );
   exit( 1 );
}

void Cmd_SoftErrorf( const char* fmt, ... )
{
   va_list  args;
   va_start( args, fmt );
   fprintf( stderr, "ERROR: " );
   vfprintf( stderr, fmt, args );
   va_end( args );
}
