#include <game/game.h>
#include <common/dll.h>
#include <common/cmd.h>
#include <game/ent.h>

gamestate_t    game = {0};
cvar_t         game_rate = {"game_rate", CVARK_INTEGER, 0, false, {.integer = 40}, 0, 0};
double         last_update = 0.0, update_time = 0.0;

typedef void (*gamevminit_t)( dll_export_t* );
typedef void (*gamevmquit_t)( void );

void Game_Init( bool vm )
{
   WIPE( game );
   game.vm = vm;

   if( !game.vm )
   {
      gamevminit_t   init;
      dll_export_t   dll;

      game.handle.dll = DLL_Open( "game" DLL_ARCH );
      if( !game.handle.dll )
         Cmd_Errorf( "failed to open game dll (arch string: %s)\n", DLL_ARCH );

      init = (gamevminit_t) DLL_Symbol( game.handle.dll, "GameVM_Init" );
      if( !init )
         Cmd_Errorf( "failed to open game dll (no GameVM_Init)\n" );

      DLL_FillInExports( &dll );
      init( &dll );
   }
   else
      ; /* TODO: vm */

   CVar_Register( &game_rate );
}

void Game_Quit( void )
{
   if( !game.vm )
   {
      gamevmquit_t   quit;

      quit = (gamevmquit_t) DLL_Symbol( game.handle.dll, "GameVM_Quit" );
      if( !quit )
         Cmd_Errorf( "game vm error (no GameVM_Quit)\n" );

      quit( );
      DLL_Close( game.handle.dll );
   }
   else
      ; /* TODO: vm */
   WIPE( game );
}

void* Game_GetSymbol( const char* name )
{
   if( !game.vm )
      return DLL_Symbol( game.handle.dll, name );
   else
      return NULL; /* TODO: vm */
}

void Game_UpdateSim( void )
{
   uint32_t i;
   econ_t*  e;

   for( i = 0; i < farthest_entity+1; i++ )
   {
      e = &game_entities[0];
      if( !e->classname[0] || e->frozen )
         continue;

      Ent_Update( e );
   }
}
