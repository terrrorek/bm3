
#include <video/vid.h>
#include <common/cmd.h>
#include <common/args.h>
#include <video/rend.h>
#include <common/cvar.h>
#include <client/input.h>

vidstate_t  vidState = {0};
char*       renderer = NULL;

static rend_context_t Vid_GetContextFromString( const char* str )
{
   if( !BM_strcmp( str, "soft") )
      return RCTX_SOFT;
   else if( !BM_strcmp( str, "gl" ) )
      return RCTX_GL;
   else return RCTX_INVALID;
}

static void Vid_LoadGL( void )
{
   SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
   SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
   SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );

   if(!( vidState.glContext = SDL_GL_CreateContext( vidState.window ) ))
   {
      Cmd_Errorf( "%s: context creation failed!\n", __func__ );
   }
}

static void Vid_CreateRenderer( void )
{
   if(!( vidState.rendSurface = SDL_CreateSurface(
      vidState.width, vidState.height, SDL_PIXELFORMAT_RGBA8888 ) ))
   {
      Cmd_Errorf( "failed to create software surface!\n" );
   }
}

static void Vid_LoadRenderer( void )
{
   switch( vidState.context )
   {
   case RCTX_GL:
      Vid_LoadGL( );
      SDL_GL_SetSwapInterval( 0 );
      break;
   case RCTX_SOFT:
      Vid_CreateRenderer( );
      break;
   default:
      break;
   }
}

static void Vid_CreateWindow( void )
{
   if( !SDL_InitSubSystem( SDL_INIT_VIDEO ) )
   {
      Cmd_Errorf( "%s: video subsystem failed to init!\n", __func__ );
   }

   SDL_WindowFlags flags = 0;
   switch( vidState.context )
   {
   case RCTX_GL:
      flags |= SDL_WINDOW_OPENGL;
      break;
   case RCTX_SOFT:
   default:
      break;
   }

   if(!( vidState.window = SDL_CreateWindow(
      BMSTR(GAMENAME), vidState.width, vidState.height, flags )))
   {
      Cmd_Errorf( "%s: window creation failed!\n", __func__ );
   }

   Vid_LoadRenderer( );
}

static void Vid_QuitWindow( void )
{
   if( vidState.glContext )
      SDL_GL_DestroyContext( vidState.glContext );
   if( vidState.rendSurface )
      SDL_DestroySurface( vidState.rendSurface );
   if( vidState.window )
      SDL_DestroyWindow( vidState.window );
}

void Vid_Init( void )
{
   WIPEOUT( vidState );

   int width = Cmd_GetIntArg( "-width" ),
       height = Cmd_GetIntArg( "-height" );

   if( !width || !height )
   {
      width = 640;
      height = 480;
   }

   vidState.width = width,
   vidState.height = height;

   if( !renderer )
      renderer = Cmd_GetStringArg( "-rend" );

   vidState.context = Vid_GetContextFromString( renderer );

   Vid_CreateWindow( );
   Rend_LoadRenderer( renderer );

   rendImports.Rend_SetClearValues(
      (rgba_t){48, 48, 48, 255}, 1.0 );
}

void Vid_Quit( void )
{
   Rend_UnloadRenderer( );
   Vid_QuitWindow( );
}

void Vid_Update( void )
{
   vidState.deltaTime = deltaTime;
   do
   {
      SDL_Event* event = &vidState.event;

      switch( event->type )
      {
      case SDL_EVENT_QUIT:
         shouldExit = true;
         break;
      case SDL_EVENT_KEY_DOWN:
         if( event->key.key != SDLK_RETURN &&
             event->key.key != SDLK_BACKSPACE &&
             event->key.key != SDLK_ESCAPE &&
             event->key.key != SDLK_SPACE &&
             event->key.key != SDLK_UNDERSCORE )
            break;

         IN_Event( event->key.key );
         break;
      case SDL_EVENT_TEXT_INPUT:
         IN_Event( *event->text.text );
         break;
      default:
         break;
      }
   }
   while( SDL_PollEvent( &vidState.event ) );
}

void Vid_Draw( void )
{
   rendImports.Rend_Update( );

   switch( vidState.context )
   {
   case RCTX_GL:
      SDL_GL_SwapWindow( vidState.window );
      break;
   case RCTX_SOFT:
   {
      SDL_Surface* winSurface = SDL_GetWindowSurface( vidState.window );
      if( !winSurface )
      {
         Cmd_Errorf( "failed to get window surface!\n" );
      }

      SDL_Surface* new = SDL_ConvertSurface( vidState.rendSurface, winSurface->format );
      memcpy( winSurface->pixels, new->pixels, winSurface->w * winSurface->h * 4 );
      SDL_UpdateWindowSurface( vidState.window );
      SDL_DestroySurface( new );
      break;
   }
   default:
      break;
   }
}
