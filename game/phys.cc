#include "phys.hh"

CVAR( phys_friction, 4.f );

static void CheckCVars( void )
{
   REG_CHECK( phys_friction );
}

void Phys::NonCollideMove( CPhysEntity* ent )
{
   CheckCVars( );
   glm_vec3_add(
      ent->eobj->data.transform.position,
      ent->velocity,
      ent->eobj->data.transform.position );
   Phys::ApplyFriction( ent );
}

void Phys::ApplyFriction( CPhysEntity* ent )
{
   vec3 div;

   CheckCVars( );
   glm_vec3_divs( ent->velocity, phys_friction.cv.normal, div );

   glm_vec3_scale( div, 1.f - .3f * (*engFuncs->deltaTime), div );
   glm_vec3_sub( ent->velocity, div, ent->velocity );
}
