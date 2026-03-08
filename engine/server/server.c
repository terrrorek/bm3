#include <server/server.h>
#include <common/cmd.h>
#include <common/event.h>
#include <game/game.h>

cvar_t            dedicated = {"dedicated", CVARK_INTEGER, 0, false, {.integer = 0}, 0, NULL};

server_state_t    sv_state;
uint              sv_gametick = 0;

void SV_Init( void )
{
   WIPE( sv_state );
}

void SV_Quit( void )
{
}

void SV_Update( void )
{
   event_t ev;

   if( !sv_state.running )
      return;

   do
   {
      Event_PopKind( EVENT_SV_PACKET, &ev );
      if( ev.kind != EVENT_SV_PACKET )
         break;
      switch( ev.data.packet.sock )
      {
      case SOCK_DATA_CLIENT:
         SV_DataPacketUpdate( ev.data.packet.buffer, &ev.data.packet.addr );
         break;
      case SOCK_MSG_CLIENT:
         SV_MsgPacketUpdate( ev.data.packet.buffer, &ev.data.packet.addr );
         break;
      default: break;
      }
   }
   while( 1 );

   Game_UpdateSim( );
   SV_PacketFrame( );

   ++sv_gametick;
}
