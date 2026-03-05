#ifndef COMMON_DLL_H
#define COMMON_DLL_H

#include <common/define.h>

#if defined( _WIN32 )
#  define DLL_PREFIX
#  define DLL_SUFFIX ".dll"
#  define DLL_EXPORT __declspec(dllexport)
#elif defined( __unix__ )
#  define DLL_PREFIX "lib"
#  define DLL_SUFFIX ".so"
#  define DLL_EXPORT __attribute__((__visibility__("default")))
#else
#  error implement!
#endif

struct vidstate_s;
struct gamestate_s;

typedef struct dll_export_s
{
   struct vidstate_s*   vidstate;
   struct gamestate_s*  gamestate;

   // cmd
   void     (*Cmd_Printf)( const char* fmt, ... );
   void     (*Cmd_Errorf)( const char* fmt, ... );
   void     (*Cmd_SoftErrorf)( const char* fmt, ... );

   // memory handling
   void*    (*Mem_Alloc)( size_t size );
   void     (*Mem_Free)( void* ptr );
}
dll_export_t;

void  DLL_FillInExports( dll_export_t* dll );
void* DLL_Open( const char* name );
void  DLL_Close( void* dll );
void* DLL_Symbol( void* dll, const char* name );

#endif
