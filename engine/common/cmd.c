
#include <common/cmd.h>

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
   va_list args;
   va_start( args, fmt );
   fprintf( stderr, "ERROR: " );
   vfprintf( stderr, fmt, args );
   va_end( args );
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

