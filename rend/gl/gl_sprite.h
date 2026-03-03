#ifndef GL_SPRITE_H
#define GL_SPRITE_H
#include <common/define.h>
#include <video/sprite.h>
#include <glad/gl.h>

typedef struct gl_sprite_s
{
   vid_sprite_t*  sprite;
   GLuint         texture;
}
gl_sprite_t;

extern gl_sprite_t gl_sprites[VID_MAX_SPRITES];

gl_sprite_t* GL_AllocSprite( void );
gl_sprite_t* GL_FindSprite( const char* name );
void GL_LoadSprite( gl_sprite_t* glspr, vid_sprite_t* sprite, void* data );
void GL_FreeSprite( gl_sprite_t* glspr );

#endif
