
#include "soft.h"

uint32_t*         screenBuffer = NULL;
float*            depthBuffer = NULL;

rgba_t            clearColor = {48, 48, 48, 255};
float             clearDepth = 1.0f;

eng_exports_t*    engFuncs = NULL;
vidstate_t*       state = NULL;

static void Rend_AllocateBuffers( void )
{
   int screenSurface = state->width * state->height;

   if( screenBuffer )
      engFuncs->Mem_Free( screenBuffer );
   if( depthBuffer )
      engFuncs->Mem_Free( depthBuffer );

   screenBuffer = (uint32_t*) engFuncs->Mem_Alloc( screenSurface * sizeof(uint32_t) );
   depthBuffer = (float*) engFuncs->Mem_Alloc( screenSurface * sizeof(float) );
}

static int Rend_Init( void )
{
   memset( textures, 0, sizeof(textures) );
   Rend_AllocateBuffers( );
   return 0;
}

void Rend_Clear( bool depth )
{
   for( int y = 0; y < state->height; y++ )
      for( int x = 0; x < state->width; x++ )
      {
         if( depth )
            Rend_SetDepth( x, y, clearDepth );
         Rend_SetBuffer( x, y, clearColor );
      }
}

void Rend_SetClearValues( rgba_t color, float depth )
{
   memcpy( clearColor, color, sizeof(rgba_t) );
   clearDepth = depth;
}

void Rend_Update( void )
{
   if( !state->rendSurface->pixels )
   {
      engFuncs->Cmd_Errorf( "can't write to software surface! (pixels are NULL)\n" );
   }

   memcpy( state->rendSurface->pixels, screenBuffer,
      (state->rendSurface->w * state->rendSurface->h) * sizeof(rgba_t) );
}

rend_imports_t rendExports = {
   .Rend_Clear = Rend_Clear,
   .Rend_SetClearValues = Rend_SetClearValues,
   .Rend_Update = Rend_Update,

   .Rend_DrawPic = Rend_DrawPic,
   .Rend_DrawChar = Rend_DrawChar,
   .Rend_DrawString = Rend_DrawString,
   .Rend_CreateTexture = Rend_CreateTexture,
   .Rend_DestroyTexture = Rend_DestroyTexture,
};

EXPORT int Rend_RefInit( eng_exports_t* engine, rend_imports_t* imports )
{
   engFuncs = engine;
   engFuncs->state->context = RCTX_SOFT;

   *imports = rendExports;
   state = engFuncs->state;
   return Rend_Init( );
}
