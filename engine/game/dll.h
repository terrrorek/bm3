
#ifndef DLL_H
#define DLL_H

#include <common/common.h>
#include <common/bmstring.h>
#include <video/rend.h>
#include <game/edecl.h>
#include <client/input.h>
#include <common/cvar.h>

typedef struct eng_exports_s
{
   vidstate_t* state;
   inmove_t*   inmove;
   double*     deltaTime;

   void        (*Cmd_Printf)( const char* fmt, ... );
   void        (*Cmd_Errorf)( const char* fmt, ... );
   edecl_t*    (*Ent_DeclEnt)( const char* name );

   void*       (*Mem_Alloc)( uint32_t size );
   void        (*Mem_Free)( void* data );

   void        (*Con_Printf)( const char* fmt, ... );

   void        (*Cvar_Register)( cvar_t* cvar );
   cvar_t*     (*Cvar_Get)( const char* name );
}
eng_exports_t;

#if defined( _WIN32 )
   #define GAME_EXT "dll"
#elif defined( __linux__ )
   #define GAME_EXT "so"
#endif

extern eng_exports_t    engExports;

void  Game_SetupExports( void );
void* Game_GetFuncHandle( const char* sym );

#endif
