#include <video/rend.h>

void Rend_Init( vid_rendstate_t* rend, vid_rend_t backend, SDL_Window* window )
{
   WIPE( *rend );
   rend->window = window;
   rend->backend = backend;

   switch( rend->backend )
   {
   case VID_REND_SOFT:
      RendSoft_Init( rend, window );
      break;
   case VID_REND_GL:
      RendGL_Init( rend, window );
      break;
   }
}

void Rend_Quit( vid_rendstate_t* rend )
{
   switch( rend->backend )
   {
   case VID_REND_SOFT:
      RendSoft_Quit( rend );
      break;
   case VID_REND_GL:
      RendGL_Quit( rend );
      break;
   }
   WIPE( *rend );
}

void Rend_Update( vid_rendstate_t* rend )
{
   switch( rend->backend )
   {
   case VID_REND_SOFT:
      RendSoft_Update( rend );
      break;
   case VID_REND_GL:
      RendGL_Update( rend );
      break;
   }
}
