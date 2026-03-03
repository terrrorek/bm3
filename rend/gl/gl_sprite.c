#include <rend/gl/gl_sprite.h>

gl_sprite_t gl_sprites[VID_MAX_SPRITES] = {0};

gl_sprite_t* GL_AllocSprite( void )
{
   uint  i;

   for( i = 0; i < VID_MAX_SPRITES; i++ )
   {
      if( !gl_sprites[i].sprite )
         return &gl_sprites[i];
   }

   return NULL;
}

gl_sprite_t* GL_FindSprite( const char* name )
{
   uint  i;

   for( i = 0; i < VID_MAX_SPRITES; i++ )
   {
      if( !gl_sprites[i].sprite )
         continue;
      if( strncmp( gl_sprites[i].sprite->name, name,
         sizeof(gl_sprites[i].sprite->name) ) == 0 )
      {
         return &gl_sprites[i];
      }
   }

   return NULL;
}

void GL_LoadSprite( gl_sprite_t* glspr, vid_sprite_t* sprite, void* data )
{
   if( glspr->sprite )
      GL_FreeSprite( glspr );

   glspr->sprite = sprite;
   glGenTextures( 1, &glspr->texture );

   glBindTexture( GL_TEXTURE_2D, glspr->texture );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGBA8,
      sprite->width, sprite->height, 0,
      GL_RGBA, GL_UNSIGNED_BYTE, data
   );
}

void GL_FreeSprite( gl_sprite_t* glspr )
{
   glDeleteTextures( 1, &glspr->texture );
   WIPE( *glspr );
}
