#include <rend/gl/gl.h>
#include <common/cmd.h>
#include <cglm/cglm.h>

static vid_vertex_t RendGL_QuadVertices[] = {
   {{-1.f, +1.f, +0.f}, {+0.f, +0.f}},
   {{+1.f, +1.f, +0.f}, {+1.f, +0.f}},
   {{+1.f, -1.f, +0.f}, {+1.f, +1.f}},
   {{-1.f, -1.f, +0.f}, {+0.f, +1.f}},
};
static uint32_t RendGL_QuadIndices[] = {
   0, 1, 2,
   2, 3, 0
};

static const char* RendGL_BaseVertexShader =
"#version 330 core\n"
"layout( location = 0 ) in vec3 v_Position;\n"
"layout( location = 1 ) in vec2 v_UV;\n"
"out vec2 o_UV;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4( v_Position, 1.0 );\n"
"   o_UV = v_UV;\n"
"}\0"
;
static const char* RendGL_BaseFragmentShader =
"#version 330 core\n"
"in vec2 o_UV;\n"
"layout( location = 0 ) out vec4 v_Color;\n"
"void main()\n"
"{\n"
"   v_Color = vec4( o_UV, 0.0, 1.0 );\n"
"}\0"
;

static void RendGL_CreateBasePipeline( vid_rendstate_t* rend )
{
   uint  vert = glCreateShader( GL_VERTEX_SHADER ),
         frag = glCreateShader( GL_FRAGMENT_SHADER ),
         program;

   int   success;
   char  log[1024];

   glShaderSource( vert, 1, &RendGL_BaseVertexShader, NULL );
   glShaderSource( frag, 1, &RendGL_BaseFragmentShader, NULL );

   glCompileShader( vert );
   glCompileShader( frag );

   glGetShaderiv( vert, GL_COMPILE_STATUS, &success );
   if( !success )
   {
      glGetShaderInfoLog( vert, 1024, NULL, log );
      Cmd_Errorf( "failed to compile base vertex shader\n%s", log );
   }

   glGetShaderiv( frag, GL_COMPILE_STATUS, &success );
   if( !success )
   {
      glGetShaderInfoLog( frag, 1024, NULL, log );
      Cmd_Errorf( "failed to compile base fragment shader\n%s", log );
   }

   program = glCreateProgram( );
   glAttachShader( program, vert );
   glAttachShader( program, frag );

   glLinkProgram( program );
   glGetProgramiv( program, GL_LINK_STATUS, &success );
   if( !success )
   {
      glGetProgramInfoLog( program, 1024, NULL, log );
      Cmd_Errorf( "failed to link base program\n%s", log );
   }

   glDeleteShader( vert );
   glDeleteShader( frag );

   rend->data.gl.base_pipeline = program;
}

static void RendGL_CreateQuadBuffers( vid_rendstate_t* rend )
{
   uint buffers[2];

   glGenVertexArrays( 1, &rend->data.gl.quad_vao );
   glGenBuffers( 2, buffers );

   glBindVertexArray( rend->data.gl.quad_vao );
   glBindBuffer( GL_ARRAY_BUFFER, buffers[0] );
   glBufferData( GL_ARRAY_BUFFER, sizeof(RendGL_QuadVertices), RendGL_QuadVertices, GL_STATIC_DRAW );

   glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(vid_vertex_t), (void*) offsetof(vid_vertex_t, position) );
   glEnableVertexAttribArray( 0 );
   glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof(vid_vertex_t), (void*) offsetof(vid_vertex_t, uv) );
   glEnableVertexAttribArray( 1 );

   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffers[1] );
   glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(RendGL_QuadIndices), RendGL_QuadIndices, GL_STATIC_DRAW );

   glBindVertexArray( 0 );
   glBindBuffer( GL_ARRAY_BUFFER, 0 );
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void RendGL_Init( vid_rendstate_t* rend, SDL_Window* window )
{
   if(!( rend->data.gl.gl_context = SDL_GL_CreateContext(window) ))
      Cmd_Errorf( "failed to create GL 3.3 context\n" );
   if(!( gladLoadGL( (GLADloadfunc) SDL_GL_GetProcAddress ) ))
      Cmd_Errorf( "failed to load GLAD\n" );

   Cmd_Printf( "loaded GL %s\n", glGetString(GL_VERSION) );
   SDL_GL_SetSwapInterval( 0 );

   glClearColor( .1f, .1f, .1f, 1.f );
   glClearDepth( 1.f );

   RendGL_CreateQuadBuffers( rend );
   RendGL_CreateBasePipeline( rend );
}

void RendGL_Quit( vid_rendstate_t* rend )
{
   SDL_GL_DestroyContext( rend->data.gl.gl_context );
   rend->data.gl.gl_context = NULL;
}

void RendGL_Update( vid_rendstate_t* rend )
{
   glClear( GL_COLOR_BUFFER_BIT );
   glBindVertexArray( rend->data.gl.quad_vao );
   glUseProgram( rend->data.gl.base_pipeline );
   glDrawElementsInstanced( GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL, 1 );
   SDL_GL_SwapWindow( rend->window );
}
