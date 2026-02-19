#include <video/sprite.h>
#include <video/vid.h>
#include <video/rend.h>
#include <common/cmd.h>

bool SpriteAtlas_WillSpriteFit( vid_spriteatlas_t* atlas, vid_sprite_t* sprite )
{
   if( atlas->cursor_pos[0] + sprite->width > atlas->width )
   {
      atlas->cursor_pos[0] = 0;
      atlas->cursor_pos[1] += atlas->highest;
      atlas->highest = 0;
   }
   if( atlas->cursor_pos[1] + sprite->height > atlas->height )
      return false;

   if( sprite->height > atlas->highest )
      atlas->highest = sprite->height;

   sprite->atlas_pos[0] = atlas->cursor_pos[0];
   sprite->atlas_pos[1] = atlas->cursor_pos[1];
   return true;
}
