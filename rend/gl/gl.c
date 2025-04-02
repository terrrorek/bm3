
#include "gl.h"

eng_exports_t*    engFuncs = NULL;
vidstate_t*       state = NULL;

uint              quadArray = 0;
uint              quadBuffer = 0, quadIndices = 0;
uint              picShader = 0;

mat4              modelMatrix, viewMatrix, projectionMatrix;

static float quadVertex[] =
{
   1.0f,  0.0f, 0.0f,   1.0f, 1.0f,   // top right
   1.0f, -1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
   0.0f, -1.0f, 0.0f,   0.0f, 0.0f,   // bottom left
   0.0f,  0.0f, 0.0f,   0.0f, 1.0f    // top left
};

static int quadIndex[] =
{
   3, 1, 0,
   3, 2, 1
};

static void Rend_InitQuad( void )
{
   glGenVertexArrays( 1, &quadArray );
   glGenBuffers( 1, &quadBuffer );
   glGenBuffers( 1, &quadIndices );
   glBindVertexArray( quadArray );
   glBindBuffer( GL_ARRAY_BUFFER, quadBuffer );
   glBufferData( GL_ARRAY_BUFFER, sizeof(quadVertex), quadVertex, GL_STATIC_DRAW );
   glVertexAttribPointer( 0, 3, GL_FLOAT, false, 5 * sizeof(float), 0 );
   glEnableVertexAttribArray( 0 );
   glVertexAttribPointer( 1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)) );
   glEnableVertexAttribArray( 1 );
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, quadIndices );
   glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndex), quadIndex, GL_STATIC_DRAW );
   glBindVertexArray( 0 );
   glBindBuffer( GL_ARRAY_BUFFER, 0 );
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

static int Rend_Init( void )
{
   if(!( gladLoadGL( (GLADloadfunc)SDL_GL_GetProcAddress ) ))
   {
      engFuncs->Cmd_Errorf( "failed to load GLAD!\n" );
   }

   memset( textures, 0, sizeof(textures) );
   glViewport( 0, 0, state->width, state->height );
   u_screen_wh[0] = state->width, u_screen_wh[1] = state->height;

   Rend_InitQuad( );
   Rend_InitShaders( );

   engFuncs->Cmd_Printf( "loaded GL %s\n",
      glGetString( GL_VERSION ) );
   return 0;
}

void Rend_Clear( bool depth )
{
   uint32_t mask = GL_COLOR_BUFFER_BIT;
   if( depth )
      mask |= GL_DEPTH_BUFFER_BIT;

   glClear( mask );
}

void Rend_SetClearValues( rgba_t color, float depth )
{
   vec4 col;
   Rend_RGBAToVec( color, &col );

   glClearColor( col[0], col[1], col[2], col[3] );
   glClearDepth( depth );
}

void Rend_Update( void )
{

}

void Rend_Unload( void )
{

}

void Stub( void ) {}

rend_imports_t rendExports = {
   .Rend_Unload = Rend_Unload,
   .Rend_Update = Rend_Update,
   .Rend_Clear = Rend_Clear,
   .Rend_SetClearValues = Rend_SetClearValues,

   .Rend_CreateTexture = Rend_CreateTexture,
   .Rend_DestroyTexture = Rend_DestroyTexture,

   .Rend_DrawPic = Rend_DrawPic,
   .Rend_DrawChar = Rend_DrawChar,
   .Rend_DrawString = (void*) Stub,
};

EXPORT int Rend_RefInit( eng_exports_t* engine, rend_imports_t* imports )
{
   engFuncs = engine;
   engFuncs->state->context = RCTX_GL;

   *imports = rendExports;
   state = engFuncs->state;
   return Rend_Init( );
}
