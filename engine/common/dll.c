#include <common/dll.h>
#include <common/cmd.h>
#include <common/alloc.h>
#include <video/vid.h>

void DLL_FillInExports( dll_export_t* dll )
{
   *dll = (dll_export_t){
      .Cmd_Printf = Cmd_Printf,
      .Cmd_Errorf = Cmd_Errorf,
      .Cmd_SoftErrorf = Cmd_SoftErrorf,

      .Mem_Alloc = Mem_Alloc,
      .Mem_Free = Mem_Free,
   };
}

#if defined( _WIN32 )
#  define WIN32_LEAN_AND_MEAN
#  include <Windows.h>
#elif defined( __unix__ )
#  include <dlfcn.h>
#endif

void* DLL_Open( const char* name )
{
#if defined( _WIN32 )
   return (void*) LoadLibraryA( name );
#elif defined( __unix__ )
   return (void*) dlopen( name, RTLD_GLOBAL | RTLD_NOW )
#endif
}

void DLL_Close( void* dll )
{
#if defined( _WIN32 )
   FreeLibrary( (HMODULE) dll );
#elif defined( __unix__ )
   dlclose( dll );
#endif
}

void* DLL_Symbol( void* dll, const char* name )
{
#if defined( _WIN32 )
   return (void*) GetProcAddress( (HMODULE) dll, name );
#elif defined( __unix__ )
   return dlsym( dll, name );
#endif
}
