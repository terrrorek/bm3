#include <video/vid.h>
#include <common/cmd.h>

vidstate_t vid = {0};

void Vid_Init( void )
{
   WIPE( vid );
   vid.width = 800, vid.height = 600;

   if( !SDL_InitSubSystem( SDL_INIT_VIDEO ) )
      Cmd_Errorf( "SDL video init failed (%s)\n", SDL_GetError() );

   // gl attributes
   SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
   SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
   SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

   if(!( vid.window = SDL_CreateWindow( "bm", vid.width, vid.height, SDL_WINDOW_OPENGL ) ))
      Cmd_Errorf( "failed to create SDL window (%s)", SDL_GetError() );

   Rend_Init( &vid.rend, VID_REND_GL, &vid );
}

void Vid_Quit( void )
{
   Rend_Quit( &vid.rend );
   SDL_QuitSubSystem( SDL_INIT_VIDEO );
   WIPE( vid );
}

void Vid_Update( void )
{
   SDL_Event event;

   while( SDL_PollEvent(&event) )
   {
      switch( event.type )
      {
      case SDL_EVENT_QUIT:
         running = false;
         break;
      default: break;
      }
   }

   Rend_Update( &vid.rend );
}

void Vid_DrawSprite( struct vid_sprite_s* sprite, vid_transform_t* transform )
{
   if( !sprite )
      return;
   vid.rend.func.DrawSprite( &vid.rend, sprite, transform );
}
