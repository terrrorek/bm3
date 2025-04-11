#include <net/net.h>
#include <common/cmd.h>
#include <game/state.h>
#include <game/console.h>
#include <common/bmstring.h>
#include <net/msg.h>
#include <client/input.h>
#include <common/cvar.h>
#include <net/pack.h>

bool     cIsTrying = FALSE;
int      cTries = 0;

eobj_t*  player = NULL;
extern double lastUpdate;

bool Net_Connect( bmsock_t* sock, const char* addr, uint16_t port )
{
   static double        conTime = 0.0;

   int                  result;
   struct sockaddr_in   address;
   int                  len = sizeof(address), block = 0;

   msg_t                msg;

   inet_pton( AF_INET, addr, &address.sin_addr );
   address.sin_port = htons( port + 1 );
   address.sin_family = AF_INET;

   BLOCK( sock->message, block );
   result = connect( sock->message, (struct sockaddr*) &address, sizeof(address) );
   block = 1;
   BLOCK( sock->message, block );

   if( result != 0 )
   {
      WINERR;
      Con_Errorf( "failed to connect to server. (%i)\n", result );
      return FALSE;
   }

   WIPEOUT( msg );
   msg.hash = Net_GetUserHash( );
   msg.type = MSGT_CONNECT;
   BM_snprintf( (char*) msg.data, 120, "%s", playername );
   send( sock->message, (char*) &msg, sizeof(msg), 0 );

   block = 0;
   BLOCK( sock->message, block );
   recv( sock->message, (char*) &msg, sizeof(msg), 0 );
   block = 1;
   BLOCK( sock->message, block );

   if( (msg.type != MSGT_CONNECT && msg.hash != Net_GetUserHash()) || msg.type == 0 )
   {
      WINERR;
      Con_Errorf( "failed to connect to server. no space or server rejected without reason. (%i)\n", result );
      return FALSE;
   }

   Con_Printf( "connected.\n" );

   inet_pton( AF_INET, addr, &address.sin_addr );
   address.sin_port = htons( port );
   address.sin_family = AF_INET;
   connect( sock->packet, (struct sockaddr*) &address, sizeof(address) );

   sock->flags |= SOCK_CONNECTED;
   sock->address = address.sin_addr.s_addr;
   sock->port = port;
   return TRUE;
}

void Net_Disconnect( bmsock_t* sock )
{
   msg_t    msg;
   double   x = 0;

   msg.type = MSGT_DISCONNECT;
   msg.hash = Net_GetUserHash( );
   send( sock->message, (char*) &msg, sizeof(msg), 0 );

   while( x < 0.1 )
      x += gameTime > 0.01 ? gameTime : 0.001;

   Net_ResetSocket( sock );

   sock->flags &=~ SOCK_CONNECTED;
   sock->address = 0;
   sock->port = 0;
   player = NULL;
}

extern cvar_t eng_rate;

static void CNet_ProcessMessage( bmsock_t* sock, msg_t* msg )
{
   if( !msg->type )
      return;
   if( msg->hash != Net_GetUserHash() )
   {
      Con_Errorf( "message hash didn't equal user hash. disconnecting.\n" );
      Net_Disconnect( &gamesock );
      return;
   }

   switch( msg->type )
   {
   case MSGT_DISCONNECT:
      Con_Errorf( "server requested disconnect: %s.\n", msg->data );
      Net_Disconnect( &gamesock );
      break;
   case MSGT_BROADCAST:
      Con_Printf( "%s", msg->data );
      break;
   case MSGT_INFO:
   {
      int rate = *(int*) &msg->data[0];
      eng_rate.number = rate;
      break;
   }
   case MSGT_CONNECT:
   default: break;
   }
}

static void CNet_UnpackEntities( packet_t* pack )
{
   const int   maxpack = MAX_PACKET_SIZE / sizeof(ent_packet_t);
   int         i;

   for( i = 0; i < maxpack; i++ )
   {
      ent_packet_t*  epack = (ent_packet_t*) &pack->data[i * sizeof(ent_packet_t)];
      eobj_t*        obj;

      if( !epack->valid )
         continue;

      if(!( obj = Ent_FindByPlayer(epack->player) ))
      {
         obj = Ent_Create( epack->decl.name );
         obj->player = epack->player;

         if( !obj )
         {
            Cmd_SoftErrorf( "failed to replicate new entity \"%s\"\n", epack->decl.name );
            return;
         }
      }

      obj->interp = 0.0;
      obj->lastdata = obj->data;
      obj->data = epack->data;
      obj->player = epack->player;
      obj->updated = 0;

      if(BM_strcmp( obj->mat.pic, epack->mat.pic ))
         obj->mat = epack->mat;

      if( obj->player == Net_GetUserHash() )
         player = obj;
   }
}

static void CNet_ProcessPackets( bmsock_t* sock )
{
   static bool sent = FALSE;
   static int  tries = 0, resent = 0;
   packet_t    packet;
   int         result;

   if( resent > 20 )
   {
      Con_Errorf( "lost connection to server.\n" );
      Net_Disconnect( sock );
   }

   WIPEOUT( packet );
   if( !sent )
   {
      packet.type = PACK_MOVE;
      packet.hash = Net_GetUserHash( );
      if( player )
      {
         packet.data[0] = 1;
         memcpy( &packet.data[1], &inmove, sizeof(inmove) );
      }

      result = sendto( sock->packet, (char*) &packet, sizeof(packet), 0, NULL, NULL );
      sent = TRUE;
      if( tries++ > 5 )
      {
         sent = FALSE; // retry
         resent++;
      }
      return;
   }

   if(( result = recvfrom( sock->packet, (char*) &packet, sizeof(packet), 0, NULL, NULL )) == EWOULDBLOCK )
   {
      sent = FALSE;
      resent++;
      return;
   }
   if( !packet.type )
      return;

   switch( packet.type )
   {
   case PACK_ENTITY:
   {
      CNet_UnpackEntities( &packet );
      break;
   }
   case PACK_MOVE:
   default: break;
   }

   lastInterp = (double) SDL_GetTicks() / 1000.0;
   sent = FALSE;
   tries = 0;
   resent = 0;
}

void Net_ClientLoop( void )
{
   msg_t msg;

   if(!( gamesock.flags & SOCK_CONNECTED ))
      return;

   // messages
   WIPEOUT( msg );
   if( recv( gamesock.message, (char*) &msg, sizeof(msg), 0 ) != EWOULDBLOCK )
      CNet_ProcessMessage( &gamesock, &msg );
   if(!( gamesock.flags & SOCK_CONNECTED ))
      return;

   CNet_ProcessPackets( &gamesock );
}
