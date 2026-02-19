#include <video/rend.h>
#include <video/vid.h>
#include <common/dll.h>
#include <common/cmd.h>

static const char* Rend_DLLNames[] =
{
   "soft",
   "gl",
};

void Rend_Init( vid_rendstate_t* rend, vid_rend_t backend, struct vidstate_s* vid )
{
   char           buffer[512];
   vid_rendload_t load_func;
   dll_export_t   dll;

   WIPE( *rend );
   rend->backend = backend;
   rend->vid = vid;

   snprintf( buffer, sizeof(buffer), DLL_PREFIX "rend_%s" DLL_SUFFIX, Rend_DLLNames[backend] );
   rend->dll = DLL_Open( buffer );
   if( !rend->dll )
      Cmd_Errorf( "failed to load \"%s\"!\n", buffer );

   load_func = (vid_rendload_t) DLL_Symbol( rend->dll, "Rend_Load" );
   if( !load_func )
      Cmd_Errorf( "renderer \"%s\" doesn't export Rend_Load\n", buffer );

   DLL_FillInExports( &dll );
   load_func( &dll, &vid->rend.func );
   rend->func.Init( rend );
}

void Rend_Quit( vid_rendstate_t* rend )
{
   rend->func.Quit( rend );
   DLL_Close( rend->dll );
   WIPE( *rend );
}

void Rend_Update( vid_rendstate_t* rend )
{
   rend->func.Update( rend );
}
