#include <game/eobj.h>
#include <game/state.h>
#include <game/state.h>
#include <common/alloc.h>
#include <game/helpers.h>
#include <client/input.h>
#include <common/cmd.h>

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
)

SPAWN( player )
{
   (void) this;
}
UPDATE( player )
{
   vec3  vels;

   if( client )
   {
      InputMove( this );
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
