#include <video/sprite.h>
#include <video/rend.h>
#include <video/vid.h>
#include <common/cmd.h>

vid_sprite_t      sprites[VID_MAX_SPRITES] = {0};

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

   if( !sprite )
      Cmd_Errorf( "failed to allocate sprite (no space left)\n" );

   strncpy( sprite->name, name, sizeof(sprite->name) );
   sprite->width = width;
   sprite->height = height;

   if( !vid.rend.func.LoadSprite( &vid.rend, sprite, data ) )
   {
      WIPE( *sprite );
      if( _sprite )
         *_sprite = NULL;
      return false;
   }

   if( _sprite )
      *_sprite = sprite;
   return true;
}

bool Vid_LoadFileSprite( const char* path, vid_sprite_t** _sprite )
{
   *_sprite = NULL;
   return false;
}

void Vid_FreeSprite( vid_sprite_t* sprite )
{
   vid.rend.func.FreeSprite( &vid.rend, sprite );
   WIPE( *sprite );
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
