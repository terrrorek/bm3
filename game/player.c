#include <game/eobj.h>
#include <game/state.h>
#include <game/state.h>
#include <common/alloc.h>
#include <game/helpers.h>
#include <client/input.h>
#include <common/cmd.h>
#include <common/bmstring.h>

CLASS( player,
   vec3  velocity;
   float speed;
)

static void InputMove( player_t* this )
{
   this->velocity[1] += (-inmove.forward) * this->speed;
   this->velocity[0] += inmove.right * this->speed;
}

CREATE( player,
   glm_vec3_zero( this->velocity );
   this->speed = 100.f;
   glm_vec3_fill( this->obj->data.transform.scale, 64.f );
   BM_strncpy( this->obj->mat.pic, "test/pic.tga", sizeof(this->obj->mat.pic) );
)

SPAWN( player )
{
   (void) this;
}
UPDATE( player )
{
   if( client )
   {
      InputMove( this );
      return;
   }

   glm_vec3_scale( this->velocity, gameTime, this->velocity );
   glm_vec3_add( this->obj->data.transform.position,
      this->velocity, this->obj->data.transform.position );
   glm_vec3_scale( this->velocity, 0.f, this->velocity );
}
KILL( player )
{
   (void) this;
}
