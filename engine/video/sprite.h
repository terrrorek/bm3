#ifndef VIDEO_SPRITE_H
#define VIDEO_SPRITE_H

#include <common/define.h>
#include <cglm/cglm.h>

#define VID_MAX_SPRITEATLAS         16
#define VID_MAX_SPRITEATLASSIZE     512

#define VID_MAX_SPRITES             8192
#define VID_MAX_SPRITESIZE          256

struct vid_sprite_s;
struct vid_spriteatlas_s;

struct vid_spritedata_s;
struct vid_spratlasdata_s;

typedef struct vid_sprite_s
{
   char                       name[64];
   uint16_t                   width, height;
   struct vid_spriteatlas_s*  atlas;
   uint16_t                   atlas_pos[2];
   vec2                       atlas_uvs[2];
   struct vid_spritedata_s*   data;
}
vid_sprite_t;

typedef struct vid_spriteatlas_s
{
   byte                       index;
   uint16_t                   width, height;
   uint16_t                   cursor_pos[2];
   uint16_t                   highest;
   struct vid_spratlasdata_s* data;
}
vid_spriteatlas_t;

extern vid_sprite_t        sprites[VID_MAX_SPRITES];
extern vid_spriteatlas_t   spriteatlases[VID_MAX_SPRITEATLAS];

bool           Vid_LoadSprite(
   const char* name,
   uint16_t width, uint16_t height,
   void* data, vid_sprite_t** sprite
);
bool           Vid_LoadFileSprite( const char* path, vid_sprite_t** sprite );
void           Vid_FreeSprite( vid_sprite_t* sprite );
vid_sprite_t*  Vid_FindSprite( const char* name );

bool           SpriteAtlas_WillSpriteFit( vid_spriteatlas_t* atlas, vid_sprite_t* sprite );

#endif
