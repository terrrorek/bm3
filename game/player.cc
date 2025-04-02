#include "game.hh"
#include "phys.hh"

class CPlayer : public CPhysEntity
{
public:
   void Spawn( ) override;
   void Update( bool isClient ) override;
};

CVAR( game_movespeed, 100.f );

void CPlayer::Spawn( )
{
   engFuncs->Cmd_Printf( "Hello, Entity!\n" );
   if(!( game_movespeed.cv.flags & CVAR_REGISTERED ))
      game_movespeed.Register( );
}

void CPlayer::Update( bool isClient )
{
   if( isClient )
   {
      if( engFuncs->inmove->forward > 0 )
         velocity[1] -= 10.f * (*engFuncs->deltaTime);
      else if( engFuncs->inmove->forward < 0 )
         velocity[1] += 10.f * (*engFuncs->deltaTime);

      Phys::NonCollideMove( this );
   }
   else
   {

   }
}

EXPORT_CLASS(player, CPlayer)
