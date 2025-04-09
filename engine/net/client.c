#include <net/net.h>
#include <common/cmd.h>
#include <game/state.h>
#include <game/console.h>
#include <common/bmstring.h>
#include <net/msg.h>
#include <net/pack.h>

bool     cIsTrying = FALSE;
int      cTries = 0;

bool Net_Connect( bmsock_t* sock, const char* addr, uint16_t port )
{
   static double        conTime = 0.0;

   int                  result;
   struct sockaddr_in   address;
   int                  len = sizeof(address), block = 0;

   msg_t                msg;

   address.sin_addr.s_addr = inet_addr( addr );
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
   address.sin_addr.s_addr = inet_addr( addr );
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
}

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

   Cmd_Printf( "msg: %i\n", msg->type );

   switch( msg->type )
   {
   case MSGT_DISCONNECT:
      Con_Errorf( "server requested disconnect: %s.\n", msg->data );
      Net_Disconnect( &gamesock );
      break;
   case MSGT_BROADCAST:
      Con_Printf( "broadcast: %s\n", msg->data );
      break;
   case MSGT_CONNECT:
   default: break;
   }
}

static void CNet_ProcessPackets( bmsock_t* sock )
{
   packet_t packet;

   if( recv( sock->packet, (char*) &packet, sizeof(packet), 0 ) == EWOULDBLOCK )
      return;
   if( !packet.hash )
      return;

   Cmd_Printf( "%i\n", packet.type );
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
