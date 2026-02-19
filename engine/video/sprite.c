#include <video/sprite.h>
#include <common/cmd.h>

vid_sprite_t      sprites[VID_MAX_SPRITES] = {0};
vid_spriteatlas_t spriteatlases[VID_MAX_SPRITEATLAS] = {0};

static vid_sprite_t* Vid_AllocSprite( void )
{
   uint  i;

   for( i = 0; i < VID_MAX_SPRITES; i++ )
   {
      if( !sprites[i].data )
         return &sprites[i];
   }
   return NULL;
}

bool Vid_LoadSprite(
   const char* name,
   uint16_t width, uint16_t height,
   void* data, vid_sprite_t** _sprite
)
{
   vid_sprite_t*  sprite = Vid_AllocSprite( );
   bool           fit = false;
   uint           i;

   if( !sprite )
      Cmd_Errorf( "failed to allocate sprite (no space left)\n" );

   strncpy( sprite->name, name, sizeof(sprite->name) );
   sprite->width = width;
   sprite->height = height;

   for( i = 0; i < VID_MAX_SPRITEATLAS; i++ )
   {
      if( SpriteAtlas_WillSpriteFit( &spriteatlases[i], sprite ) )
      {
         fit = true;
         break;
      }
   }

   if( !fit )
   {
      WIPE( *sprite );
      return false;
   }

   return true;
}

bool Vid_LoadFileSprite( const char* path, vid_sprite_t** _sprite )
{
   *_sprite = NULL;
   return false;
}

void Vid_FreeSprite( vid_sprite_t* sprite )
{

}

vid_sprite_t* Vid_FindSprite( const char* name )
{
   uint  i;

   for( i = 0; i < VID_MAX_SPRITES; i++ )
   {
      if( strncmp( sprites[i].name, name, sizeof(sprites[i].name) ) == 0 )
         return &sprites[i];
   }
   return NULL;
}
