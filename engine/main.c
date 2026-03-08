#include <video/vid.h>
#include <video/sprite.h>
#include <game/ent.h>
#include <common/event.h>
#include <common/cmd.h>
#include <net/net.h>
#include <game/game.h>
#include <server/server.h>
#include <client/client.h>

#include <stdio.h>

bool running = true;

int main( int argc, char** argv )
{
   double   now_time = 0.0;

   printf( "Hello, World!\n" );

   Cmd_Args( argc, argv );

   Net_Init( );
   Game_Init( false );
   Vid_Init( );

   SV_Init( );
   if( !dedicated.data.integer )
      CL_Init( );

   sv_state.running = true;

   CL_Connect( (netaddr_t){0x0100007f, NET_DEFAULT_PORT} );

   while( running )
   {
      event_t  ev;
      now_time = SDL_GetTicks() / 1000.0;

      Net_Update( );

      if( now_time > (last_update + (1. / game_rate.data.integer)) )
      {
         SV_Update( );
         CL_Update( );

         update_time = now_time - last_update;
         last_update = now_time;
      }
      if( now_time > (last_render + (1. / vid_rate.data.integer)) )
      {
         Vid_Update( );

         render_time = now_time - last_render;
         last_render = now_time;
      }

      Event_PopKind( EVENT_QUIT, &ev );
      if( ev.kind == EVENT_QUIT )
      {
         Cmd_Printf( "quitting... reason: %s\n", ev.data.quit.reason );
         running = false;
      }
   }

   if( !dedicated.data.integer )
      CL_Quit( );
   SV_Quit( );

   Vid_Quit( );
   Game_Quit( );
   Net_Quit( );

   return 0;
}
