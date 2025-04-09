#include <net/net.h>
#include <common/cmd.h>
#include <common/bmstring.h>
#include <game/console.h>
#include <net/msg.h>
#include <game/state.h>
#include <common/alloc.h>

static void SNet_BroadcastPlayers( bmsock_t* sock )
{
   int   i;

   char        buffer[MAX_MSG_SIZE];
   char*       ptr = buffer;
   const char* start = "\ncurrently active players:\n";

   strcpy( ptr, start );
   ptr += strlen( start );

   for( i = 0; i < MAX_CLIENTS && ptr < buffer + MAX_MSG_SIZE; i++ )
   {
      int            len;
      sockclient_t*  cl = &sock->clients[i];
      if( !cl->hash )
         continue;

      len = snprintf( ptr, MAX_MSG_SIZE, "\t%s\n", cl->name );
      ptr += len;
   }

   Net_BroadcastAll( sock, "%s", buffer );
}

void Net_Broadcast( sockclient_t* cl, const char* fmt, ... )
{
   va_list  args;
   int      len;
   char*    buf;
   msg_t    msg;

   va_start( args, fmt );
   len = vsnprintf( NULL, 0, fmt, args );
   if( len <= 0 )
      return;
   buf = Mem_Alloc( len + 1 );
   vsnprintf( buf, len + 1, fmt, args );
   va_end( args );

   Con_Printf( "%s\n", buf );
   if( !cl->hash )
      return;

   msg.hash = cl->hash;
   msg.type = MSGT_BROADCAST;
   memcpy( msg.data, buf, 120 );
   send( cl->message, (char*) &msg, sizeof(msg), 0 );

   Mem_Free( buf );
}

void Net_BroadcastAll( bmsock_t* sock, const char* fmt, ... )
{
   va_list  args;
   int      len;
   char*    buf;
   msg_t    msg;

   int      i;

   va_start( args, fmt );
   len = vsnprintf( NULL, 0, fmt, args );
   if( len <= 0 )
      return;
   buf = Mem_Alloc( len + 1 );
   vsnprintf( buf, len + 1, fmt, args );
   va_end( args );

   Con_Printf( "%s\n", buf );
   for( i = 0; i < MAX_CLIENTS; i++ )
   {
      sockclient_t*  cl = &sock->clients[i];
      if( !cl->hash )
         return;

      msg.hash = cl->hash;
      msg.type = MSGT_BROADCAST;
      memcpy( msg.data, buf, 120 );
      send( cl->message, (char*) &msg, sizeof(msg), 0 );
   }

   Mem_Free( buf );
}

static sockclient_t* SNet_FindEmptyClient( bmsock_t* sock )
{
   int   i;

   for( i = 0; i < MAX_CLIENTS; i++ )
   {
      if( !sock->clients[i].hash )
         return &sock->clients[i];
   }
   return NULL;
}

static void SNet_Disconnect( sockclient_t* cl, const char* message )
{
   if( message )
   {
      msg_t msg;
      strcpy( (char*) msg.data, message );
      msg.type = MSGT_DISCONNECT;
      msg.hash = cl->hash;
      send( cl->message, &msg, sizeof(msg), 0 );
   }

   // todo: broadcast
   Con_Printf( "%s disconnected.\n", cl->name );
   cl->closing = TRUE;
}

static void SNet_Listen( bmsock_t* sock )
{
   int                  result, block = 0;
   sockhandle_t         cl;

   struct sockaddr_in   addr;
   int                  len = sizeof(addr);
   sockclient_t*        cls;
   msg_t                msg;

   result = listen( sock->message, 1 );
   if( result != 0 )
   {
      WINERR;
      if( result == EWOULDBLOCK )
         return;

      Cmd_SoftErrorf( "failed to listen for incoming clients. (%i)\n", result );
      return;
   }

   cl = accept( sock->message, (struct sockaddr*) &addr, &len );
   #ifdef _WIN32
   if( cl == INVALID_SOCKET )
   #else
   if( cl <= 0 )
   #endif
   {
      WINERR;
      if( result == EWOULDBLOCK )
         return;
      Cmd_SoftErrorf( "failed to accept client. (%i)\n", result );
      return;
   }

   BLOCK( sock->message, block );
   WIPEOUT( msg );
   recv( cl, (char*) &msg, sizeof(msg), 0 );
   Cmd_Printf( "%x\n", msg.hash );

   if( msg.data[31] != 0 )
   {
      Cmd_SoftErrorf( "invalid message length. not accepting.\n" );
      goto dis;
   }
   block = 1;
   BLOCK( sock->message, block );

   if( msg.type != MSGT_CONNECT )
      goto dis;
   if(!( cls = SNet_FindEmptyClient(sock) ))
      goto dis;

   cls->message = cl;
   cls->hash = msg.hash;
   cls->address = addr;
   memcpy( cls->name, msg.data, 32 );

   send( cl, (char*) &msg, sizeof(msg), 0 );
   Con_Printf( "%s connected.\n", cls->name );

   return;
dis:
   block = 1;
   BLOCK( sock->message, block );
   closesocket( cl );
}

static void SNet_ProcessMessage( bmsock_t* sock, sockclient_t* cl, msg_t* msg )
{
   if( !msg->type )
      return;
   if( msg->hash != cl->hash )
   {
      SNet_Disconnect( cl, "message hash not equal to client hash!\n" );
      return;
   }

   switch( msg->type )
   {
   case MSGT_DISCONNECT:
      SNet_Disconnect( cl, NULL );
      break;
   // not handled here / invalid messages
   default:
   case MSGT_CONNECT:
      break;
   }
}

static void SNet_ProcessPackets( bmsock_t* sock )
{

}

void Net_ServerLoop( void )
{
   int            i = 0;
   sockclient_t*  cl;
   msg_t          msg;
   int            result;

   if(!( hostsock.flags & SOCK_BOUND ))
      return;

   SNet_Listen( &hostsock );

   // messages
   for( cl = &hostsock.clients[i];
      cl->hash && i < MAX_CLIENTS; cl = &hostsock.clients[++i] )
   {
      WIPEOUT( msg );
      if( !cl->hash )
         continue;

      if( cl->closing )
      {
         closesocket( cl->message );
         WIPEPTR( cl );
         continue;
      }

      if(( result = recv( cl->message, (char*) &msg, sizeof(msg), 0 )) != EWOULDBLOCK )
          SNet_ProcessMessage( &hostsock, cl, &msg );
   }

   SNet_ProcessPackets( &hostsock );
}
