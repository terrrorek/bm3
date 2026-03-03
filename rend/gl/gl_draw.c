#include "gl_draw.h"

gl_drawcall_t  gl_drawcalls[GL_MAX_DRAWCALLS];
uint           gl_drawcall_ptr = 0;

void GL_PushDrawCall( gl_drawcall_t* call )
{
   gl_drawcalls[gl_drawcall_ptr++] = *call;
}

void GL_ClearDrawCalls( void )
{
   gl_drawcall_ptr = 0;
}

void GL_ProcessDrawCalls( gl_renddata_t* data )
{
   gl_pipeline_t* last_pp;
   gl_sprite_t*   last_spr;
   uint           i;

   last_pp = NULL, last_spr = NULL;

   glBindVertexArray( data->quad_vao );
   for( i = 0; i < gl_drawcall_ptr; i++ )
   {
      gl_drawcall_t* call = &gl_drawcalls[i];

      if( call->pp != last_pp )
      {
         last_pp = call->pp;
         glUseProgram( call->pp->handle );
      }
      if( call->sprite != last_spr )
      {
         last_spr = call->sprite;
         glActiveTexture( GL_TEXTURE0 );
         glBindTexture( GL_TEXTURE_2D, call->sprite->texture );
      }

      glm_mat4_identity( model_matrix );
      glm_translate( model_matrix, call->transform.position );
      glm_rotate_x( model_matrix, call->transform.rotation[0], model_matrix );
      glm_rotate_y( model_matrix, call->transform.rotation[1], model_matrix );
      glm_rotate_z( model_matrix, call->transform.rotation[2], model_matrix );
      glm_scale( model_matrix, call->transform.scale );

      glUniformMatrix4fv( call->pp->model_loc,
         1, false, (float*) model_matrix );
      glDrawElementsInstanced( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 1 );
   }
}
