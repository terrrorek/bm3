
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

void Game_SetupExports( void )
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

#ifdef _WIN32
// can't be static
HMODULE Game_GetEngineHandle( void )
{
   HMODULE  hm;
   GetModuleHandleEx( GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
      (LPCTSTR)Game_GetEngineHandle, &hm );
   return hm;
}
#else
void* Game_GetEngineHandle( void )
{
   void* gm;

   gm = dlopen( "bm", RTLD_NOW | RTLD_GLOBAL );
   if( !gm )
      Cmd_Error( "failed to load engine as dl\n" );

   return gm;
}
#endif

void* Game_GetFuncHandle( const char* sym )
{
   if( !gameHandle )
      gameHandle = Game_GetEngineHandle( );

   #ifdef _WIN32
   return (void*) GetProcAddress( gameHandle, sym );
   #else
   return dlsym( gameHandle, sym );
   #endif
}
