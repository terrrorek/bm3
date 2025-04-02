
#include <game/dll.h>
#include <common/cmd.h>
#include <video/rend.h>
#include <game/edecl.h>
#include <common/alloc.h>
#include <common/cvar.h>
#include <game/console.h>

#ifdef _WIN32
   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>
   static HMODULE gameHandle = NULL;
#elif defined( __linux__ )
   #include <dlfcn.h>
   static void* gameHandle = NULL;
#endif

eng_exports_t engExports = {0};

static void Game_SetupExports( void )
{
   engExports = (eng_exports_t){
      .state = &vidState,
      .inmove = &inmove,
      .deltaTime = &deltaTime,

      .Cmd_Printf = Cmd_Printf,
      .Cmd_Errorf = Cmd_Errorf,
      .Ent_DeclEnt = Ent_DeclEnt,

      .Mem_Alloc = Mem_Alloc,
      .Mem_Free = Mem_Free,

      .Con_Printf = Con_Printf,

      .Cvar_Register = CVar_Register,
      .Cvar_Get = CVar_Get,
   };
}

void Game_Init( void )
{
   Game_SetupExports( );

   char nameBuffer[64];

   #if defined( _WIN32 )
      BM_snprintf( nameBuffer, sizeof(nameBuffer), "game." GAME_EXT );
      gameHandle = LoadLibrary( nameBuffer );
   #elif defined( __linux__ )
      BM_snprintf( nameBuffer, sizeof(nameBuffer), "./libgame." GAME_EXT );
      gameHandle = dlopen( nameBuffer, RTLD_NOW | RTLD_GLOBAL );
   #endif

   if( !gameHandle )
   {
      Cmd_Errorf( "failed to load game module!\n" );
   }

   int (*Game_LoadFuncs)(eng_exports_t*) =
      ( int(*)(eng_exports_t*) ) Game_GetFuncHandle( "Game_LoadFuncs" );

   if( !Game_LoadFuncs )
   {
      Cmd_Errorf( "failed to load game module! (Game_LoadFuncs not found)\n" );
   }

   int result = 0;
   if(( result = Game_LoadFuncs( &engExports ) ))
   {
      Cmd_Errorf( "failed to load game module! (Game_LoadFuncs returned %i)\n", result );
   }
}

void Game_Quit( void )
{
   #if defined( _WIN32 )
      FreeLibrary( gameHandle );
   #elif defined( __linux__ )
      dlclose( gameHandle );
   #endif
}

void* Game_GetFuncHandle( const char* name )
{
   void* handle = NULL;
   #if defined( _WIN32 )
      handle = (void*) GetProcAddress( gameHandle, name );
   #elif defined( __linux__ )
      handle = (void*) dlsym( gameHandle, name );
   #endif
   return handle;
}
