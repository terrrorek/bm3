#include <video/rend.h>
#include <common/dll.h>

dll_export_t   dll = {0};

void RendSoft_Init( vid_rendstate_t* rend )
{

}

void RendSoft_Quit( vid_rendstate_t* rend )
{

}

void RendSoft_Update( vid_rendstate_t* rend )
{

}

DLL_EXPORT void Rend_Load( dll_export_t* _dll, vid_rendfuncs_t* func )
{
   dll = *_dll;
   *func = (vid_rendfuncs_t){
      .Init = RendSoft_Init,
      .Quit = RendSoft_Quit,
      .Update = RendSoft_Update,
   };
}
