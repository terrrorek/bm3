
#include <common/cmd.h>
#include <common/alloc.h>
#include <SDL3/SDL.h>

void Cmd_Printf( const char* fmt, ... )
{
   va_list args;
   va_start( args, fmt );
   vfprintf( stdout, fmt, args );
   va_end( args );
}

void Cmd_Debugf( const char* fmt, ... )
{
   #ifdef __DEBUG
   va_list args;
   va_start( args, fmt );
   fprintf( stderr, "DEBUG: " );
   vfprintf( stderr, fmt, args );
   va_end( args );
   #endif
}

void Cmd_Errorf( const char* fmt, ... )
{
   char* buffer;
   size_t buflen;
   va_list args;

   va_start( args, fmt );
   buflen = vsnprintf( NULL, 0, fmt, args );
   va_end( args );

   buffer = Mem_Alloc( buflen + 2 );

   va_start( args, fmt );
   vsnprintf( buffer, buflen + 2, fmt, args );
   va_end( args );

   fprintf( stderr, "ERROR: %s\n", buffer );
   SDL_ShowMessageBox( &(SDL_MessageBoxData){
      .message = buffer,
      .buttons = (SDL_MessageBoxButtonData[]){
         [0] = {
            .buttonID = 0,
            .text = "OK"
         }
      },
      .numbuttons = 1,
      .title = "Cmd_Errorf"
   }, NULL );

   exit( 1 );
}

void Cmd_SoftErrorf( const char* fmt, ... )
{
   va_list args;
   va_start( args, fmt );
   fprintf( stderr, "ERROR: " );
   vfprintf( stderr, fmt, args );
   va_end( args );
}
