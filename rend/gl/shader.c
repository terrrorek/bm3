
#include "gl.h"

static const char* picVertex =
#include "shaders/pic.vert"
;

static const char* picFragment =
#include "shaders/pic.frag"
;

ivec4 u_xywh, u_tex_xywh;
ivec2 u_raw_wh, u_screen_wh;

void Rend_InitShaders( void )
{
   uint  frag, vert;

   vert = glCreateShader( GL_VERTEX_SHADER );
   frag = glCreateShader( GL_FRAGMENT_SHADER );

   glShaderSource( vert, 1, &picVertex, NULL );
   glShaderSource( frag, 1, &picFragment, NULL );

   glCompileShader( vert );
   glCompileShader( frag );

   picShader = glCreateProgram( );
   glAttachShader( picShader, vert );
   glAttachShader( picShader, frag );
   glLinkProgram( picShader );

   glDeleteShader( vert );
   glDeleteShader( frag );
}

void Rend_UpdateUniforms( uint program )
{
   uint  pos;

   pos = glGetUniformLocation( program, "model" );
   glUniformMatrix4fv( pos, 1, false, (float*)modelMatrix );

   pos = glGetUniformLocation( program, "view" );
   glUniformMatrix4fv( pos, 1, false, (float*)viewMatrix );

   pos = glGetUniformLocation( program, "projection" );
   glUniformMatrix4fv( pos, 1, false, (float*)projectionMatrix );

   pos = glGetUniformLocation( program, "xywh" );
   glUniform4iv( pos, 1, (int*)u_xywh );

   pos = glGetUniformLocation( program, "tex_xywh" );
   glUniform4iv( pos, 1, (int*)u_tex_xywh );

   pos = glGetUniformLocation( program, "raw_wh" );
   glUniform2iv( pos, 1, (int*)u_raw_wh );

   pos = glGetUniformLocation( program, "screen_wh" );
   glUniform2iv( pos, 1, (int*)u_screen_wh );
}

void Rend_2DProjection( void )
{
   glm_mat4_identity( projectionMatrix );
   // glm_ortho(
   //    0.f, (float) state->width,
   //    -(float) state->height, 0.f,
   //    -1.f, 1.f, projectionMatrix );
   glm_ortho( 0.f, 1.f, 1.f, 0.f, -1.f, 1.f, projectionMatrix );
}

void Rend_ModelMatrix( vec3 position, vec3 rotation, vec3 scale )
{
   glm_mat4_identity( modelMatrix );
   glm_scale( modelMatrix, scale );
   glm_rotate_x( modelMatrix, rotation[0], modelMatrix );
   glm_rotate_y( modelMatrix, rotation[1], modelMatrix );
   glm_rotate_z( modelMatrix, rotation[2], modelMatrix );
   glm_translate( modelMatrix, position );
}
