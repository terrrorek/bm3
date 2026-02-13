
#include <video/rend.h>
#include <common/cmd.h>
#include <common/bmstring.h>
#include <game/dll.h>

#if defined( _WIN32 )
   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>
   #include <libloaderapi.h>
   static HMODULE rendHandle = NULL;
#elif defined( __linux__ )
   #include <dlfcn.h>
   static void* rendHandle = NULL;
#endif

rend_imports_t rendImports;

void Rend_LoadRenderer( const char* name )
{
   char nameBuffer[64];

   #if defined( _WIN32 )
      BM_snprintf( nameBuffer, sizeof(nameBuffer), "rend_%s." GAME_EXT, name );
      rendHandle = LoadLibrary( nameBuffer );
   #elif defined( __linux__ )
      BM_snprintf( nameBuffer, sizeof(nameBuffer), "./librend_%s." GAME_EXT, name );
      rendHandle = dlopen( nameBuffer, RTLD_NOW | RTLD_GLOBAL );
   #endif

   if( !rendHandle )
   {
      const char* msg = NULL;
      #if defined(_WIN32)
      msg = "?";
      #elif defined(__linux__)
      msg = dlerror( );
      #else
      #endif

      Cmd_Errorf( "failed to load renderer \"%s\"! (%s)\n", nameBuffer, msg );
   }

   int (*Rend_RefInit)( eng_exports_t* engine, rend_imports_t* imports ) =
      ( int(*)(eng_exports_t*, rend_imports_t*) )
      Rend_GetFuncHandle( "Rend_RefInit" );

   if( !Rend_RefInit )
   {
      Cmd_Errorf( "failed to load renderer \"%s\"! (doesn't export Rend_RefInit)\n", nameBuffer );
   }

   int result;
   if(( result = Rend_RefInit( &engExports, &rendImports ) ))
   {
      Cmd_Errorf( "failed to load renderer \"%s\"! (Rend_RefInit returns %i)\n",
         nameBuffer, result );
   }
}

void Rend_UnloadRenderer( void )
{
   if( rendImports.Rend_Unload )
      rendImports.Rend_Unload( );

   #if defined( _WIN32 )
   FreeLibrary( rendHandle );
   #elif defined( __linux__ )
   dlclose( rendHandle );
   #endif

   memset( &rendImports, 0, sizeof(rendImports) );
}

void* Rend_GetFuncHandle( const char* name )
{
   void* handle = NULL;
   #if defined( _WIN32 )
      handle = (void*) GetProcAddress( rendHandle, name );
   #elif defined( __linux__ )
      handle = (void*) dlsym( rendHandle, name );
   #endif
   return handle;
}
