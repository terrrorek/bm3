#include <client/client.h>
#include <game/game.h>
#include <server/server.h>
#include <common/cmd.h>
#include <common/event.h>
#include <net/net.h>
#include <net/msg.h>
#include <net/protocol.h>

client_state_t    cl_state;
uint              cl_gametick = 0;

void CL_Connect( netaddr_t addr )
{
   if( cl_state.step == CLIENT_CONNECTED )
      CL_Disconnect( );

   cl_state.destaddr = addr;
   cl_state.step = CLIENT_CONNECTING;
}
void CL_Disconnect( void )
{
   cl_state.destaddr = (netaddr_t){0};
   cl_state.step = CLIENT_MENU;
}


void CL_Init( void )
{
   WIPE( cl_state );
}

void CL_Quit( void )
{
}

void CL_Update( void )
{
   event_t ev;

   if( dedicated.data.integer != 0 || cl_state.step == CLIENT_MENU )
      return;

   do
   {
      Event_PopKind( EVENT_CL_PACKET, &ev );
      if( ev.kind != EVENT_CL_PACKET )
         break;
      switch( ev.data.packet.sock )
      {
      case SOCK_DATA_CLIENT:
         CL_DataPacketUpdate( ev.data.packet.buffer, &ev.data.packet.addr );
         break;
      case SOCK_MSG_CLIENT:
         CL_MsgPacketUpdate( ev.data.packet.buffer, &ev.data.packet.addr );
         break;
      default: break;
      }
   }
   while( 1 );

   CL_PacketFrame( );

   ++cl_gametick;
}
