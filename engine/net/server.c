#include <net/net.h>
#include <net/msg.h>
#include <net/pack.h>
#include <common/cmd.h>

static uint32_t playerIds[MAX_CLIENTS] = {0};

static int Net_FindEmptyPlayerID( void )
{
   int   i;

   for( i = 0; i < MAX_CLIENTS; i++ )
      if( !playerIds[i] ) return i;

   return -1;
}

static void Net_AcceptClients( socket_t* sock )
{
   packet_t packet;
   msg_t*   msg;
   int      id, i;

   Net_GetPacketHeader( sock, &packet );
   if( packet.type != PACK_TYPE_CTOS || packet.format != PACK_FMT_MESSAGE )
      return;

   Net_RecvPacket( sock, &packet );
   msg = (msg_t*) packet.data;

   if( msg->type != MSG_TYPE_CONNECT )
   {
      Net_DissolvePacket( &packet );
      return;
   }

   packet.type = PACK_TYPE_STOC;

   for( i = 0; i < MAX_CLIENTS; i++ )
   {
      if( playerIds[i] == *(uint32_t*)(msg->data) )
      {
         msg->type = MSG_TYPE_DISCONNECT;
         Net_SendPacket( sock, &packet );
         Net_DissolvePacket( &packet );
         Cmd_Printf( "rejected %i\n", playerIds[id] );
      }
      if( playerIds[i] == 0 )
      {
         playerIds[id] = *(uint32_t*)(msg->data);
         Net_SendPacket( sock, &packet );
         Cmd_Printf( "accepted %i\n", playerIds[id] );
      }
   }
}

void Net_ServerLoop( socket_t* sock )
{
   Net_AcceptClients( sock );
}
