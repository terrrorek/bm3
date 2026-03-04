#include <stdio.h>
#include <video/vid.h>
#include <video/sprite.h>
#include <game/game.h>
#include <game/ent.h>

bool running = true;

int main( void )
{
   printf( "Hello, World!\n" );

   Vid_Init( );
   Game_Init( false );

   Ent_Allocate( "player" );

   while( running )
   {
      Game_Update( );
      Vid_Update( );
   }

   Game_Quit( );
   Vid_Quit( );
   return 0;
}
