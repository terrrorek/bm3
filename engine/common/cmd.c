#include <SDL3/SDL.h>
#include <common/cmd.h>
#include <server/server.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

cvar_t     cmd_devmode = {"cmd_devmode", CVARK_INTEGER, 0, false, {.integer = 0}, 0, 0};

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

cmdarg_t*  cmdargs = NULL;
uint       cmdargs_count = 0;


static void Cmd_PushArg( cmdarg_t* arg )
{
   cmdargs = Mem_Realloc( cmdargs, sizeof(cmdarg_t) * (++cmdargs_count) );
   cmdargs[cmdargs_count-1] = *arg;
}

static void Cmd_IndirectArg( int* i, int argc, char** argv )
{
#define _case( __str ) else if( strcmp(&argv[*i][1], __str) == 0 )
   (void) argc;

   if(0);
   _case("devmode")
   {
      cmd_devmode.data.integer = 1;
      Cmd_PushArg(&(cmdarg_t){
         .name = "devmode", .number = cmd_devmode.data.integer
      });
   }

#undef  _case
}

static void Cmd_ConsoleArg( int* i, int argc, char** argv )
{
   (void) i, (void) argc, (void) argv;
}

void Cmd_Args( int argc, char** argv )
{
   CVar_Register( &cmd_devmode );

   for( int i = 1; i < argc; i++ )
   {
      switch( argv[i][0] )
      {
      case '+':
         Cmd_ConsoleArg( &i, argc, argv );
         break;
      case '-':
         Cmd_IndirectArg( &i, argc, argv );
         break;
      default: break;
      }
   }
}
