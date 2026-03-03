#include <game/game.h>
#include <common/dll.h>
#include <common/cmd.h>

game_state_t game = {0};

void Game_Init( bool vm )
{
   WIPE( game );
   game.vm = vm;

   if( !game.vm )
   {
      game.handle.dll = DLL_Open( "game" DLL_ARCH );
      if( !game.handle.dll )
         Cmd_Errorf( "failed to open game dll (arch string: %s)\n", DLL_ARCH );
   }
   else
      ; /* TODO: vm */
}

void Game_Quit( void )
{
   if( !game.vm )
      DLL_Close( game.handle.dll );
   else
      ; /* TODO: vm */
   WIPE( game );
}

void Game_Update( void )
{

}
