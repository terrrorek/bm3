#include <video/vid.h>
#include <video/sprite.h>
#include <common/cmd.h>
#include <common/event.h>
#include <game/ent.h>

vidstate_t vid = {0};
cvar_t     vid_rate = {"vid_rate", CVARK_INTEGER, 0, false, {.integer = 60}, 0, 0};
double     last_render = 0.0, render_time = 0.0;


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
   vid.checker = Vid_FindSprite( "checker" );

   CVar_Register( &vid_rate );
}

void Vid_Quit( void )
{
   Rend_Quit( &vid.rend );
   SDL_QuitSubSystem( SDL_INIT_VIDEO );
   WIPE( vid );
}

static void Vid_DrawEntities( void )
{
   for( int i = 0; i < ENT_MAX_ENTITIES; i++ )
   {
      econ_t*           ent = &cl_entities[i];
      vid_transform_t   transform;

      transform.position[0] = ent->frames[0].transform.position[0];
      transform.position[1] = ent->frames[0].transform.position[1];
      Vid_DrawSprite( vid.checker, &transform );
   }
}

void Vid_Update( void )
{
   SDL_Event   event;
   event_t     ev;

   ev.kind = EVENT_QUIT;
   ev.data.quit.reason = "SDL_QUIT";

   while( SDL_PollEvent(&event) )
   {
      switch( event.type )
      {
      case SDL_EVENT_QUIT:
         Event_Push( &ev );
         break;
      default: break;
      }
   }

   Vid_DrawEntities( );
   Rend_Update( &vid.rend );
}

void Vid_DrawSprite( struct vid_sprite_s* sprite, vid_transform_t* transform )
{
   if( !sprite )
      return;
   vid.rend.func.DrawSprite( &vid.rend, sprite, transform );
}
