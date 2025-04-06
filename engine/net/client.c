#include <net/net.h>
#include <net/msg.h>
#include <net/pack.h>
#include <common/cmd.h>
#include <common/bmstring.h>
#include <game/console.h>
#include <game/state.h>
#include <SDL3/SDL.h>

static void Net_SendAcceptPacket( socket_t* sock )
{
   packet_t pack;
   msg_t    msg;

   double   now = (double) SDL_GetTicks() / 1000.0;

   if( now - connectTime >= 5.0 )
   {
      connectTime = 0;
      Game_Exit( );
      Con_Errorf( "failed to get accepted into server. disconnecting.\n" );
      return;
   }

   WIPEOUT( msg );
   msg.type = MSG_TYPE_CONNECT;
   BM_snprintf( (char*)msg.data, 256, "%i\0", Net_HashName( ) );

   Net_ConstructPacket( &pack, PACK_TYPE_CTOS, PACK_FMT_MESSAGE, &msg, sizeof(msg) );
   Net_SendPacket( sock, &pack );
   Net_DissolvePacket( &pack );
}

static void Net_CheckForAcceptance( socket_t* sock )
{
   packet_t pack;
   msg_t*   msg;
   uint32_t hash;

   Net_GetPacketHeader( sock, &pack );
   if( pack.type != PACK_TYPE_STOC || pack.format != PACK_FMT_MESSAGE )
      return;

   msg = (msg_t*) pack.data;
   hash = *(uint32_t*)(msg->data);

   if( hash != Net_HashName() )
      return;

   if( msg->type == MSG_TYPE_DISCONNECT )
   {
      Con_Errorf( "server is full.\n" );
      Game_Exit( );
      return;
   }
   else if( msg->type == MSG_TYPE_CONNECT )
   {
      Cmd_Printf( "accepted.\n" );
      sock->flags |= SOCK_ACCEPTED;
   }
}

void Net_ClientLoop( socket_t* sock )
{
   if(!( sock->flags & SOCK_CONNECTED ))
      return;

   if(!( gamesock.flags & SOCK_ACCEPTED ))
   {
      Net_CheckForAcceptance( sock );
      Net_SendAcceptPacket( sock );
   }
}
