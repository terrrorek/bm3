#ifndef PHYS_HH
#define PHYS_HH

#include "game.hh"
#include "cvar.hh"

extern CVar phys_friction;

class CPhysEntity : public CEntity
{
public:
   vec3 velocity, move;
};

namespace Phys
{
   void NonCollideMove( CPhysEntity* ent );
   void ApplyFriction( CPhysEntity* ent );
};

#endif
