#ifndef GL_GL_H
#define GL_GL_H

#include <common/define.h>
#include <common/dll.h>
#include <video/vid.h>
#include <video/rend.h>
#include <glad/gl.h>

typedef struct gl_pipeline_s
{
   GLuint         handle;
   GLuint         model_loc, view_loc, proj_loc;
}
gl_pipeline_t;
typedef struct vid_renddata_s
{
   SDL_GLContext  gl_context;
   GLuint         quad_vbuf, quad_ibuf;
   GLuint         quad_vao;
   gl_pipeline_t  base_pipeline;
}
gl_renddata_t;

extern dll_export_t dll;
extern mat4
   model_matrix,
   view_matrix,
   proj_matrix;

#endif
