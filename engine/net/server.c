#include <net/net.h>
#include <common/cmd.h>
#include <common/bmstring.h>
#include <game/console.h>
#include <net/msg.h>
#include <game/state.h>
#include <net/pack.h>
#include <common/alloc.h>
#include <game/eobj.h>
#include <client/input.h>
#include <game/edecl.h>
#include <common/cvar.h>

static sockclient_t* SNet_FindClientWithHash( bmsock_t* sock, uint32_t hash )
{
   int   i;

   for( i = 0; i < MAX_CLIENTS; i++ )
      if( sock->clients[i].hash == hash )
         return &sock->clients[i];
   return NULL;
}

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

   Con_Printf( "%s", buf );
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

   Con_Printf( "%s", buf );
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

static void SNet_Disconnect( bmsock_t* sock, sockclient_t* cl, const char* message )
{
   if( message )
   {
      msg_t msg;
      strcpy( (char*) msg.data, message );
      msg.type = MSGT_DISCONNECT;
      msg.hash = cl->hash;
      send( cl->message, (char*) &msg, sizeof(msg), 0 );
   }

   if( cl->ent )
   {
      Ent_Destroy( cl->ent );
      cl->ent = NULL;
   }

   Net_BroadcastAll( sock, "%s disconnected.\n", cl->name );
   cl->closing = TRUE;
}

extern cvar_t eng_rate;

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

   WIPEOUT( msg );
   msg.type = MSGT_INFO;
   msg.hash = cls->hash;
   *((int*)&msg.data[0]) = eng_rate.number;

   Net_BroadcastAll( sock, "%s connected.\n", cls->name );

   // spawn player
   cls->ent = Ent_Create( "player" );
   cls->ent->player = cls->hash;
   BM_strncpy( cls->ent->data.name, cls->name, sizeof(cls->name) );

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
      SNet_Disconnect( sock, cl, "message hash not equal to client hash!\n" );
      return;
   }

   switch( msg->type )
   {
   case MSGT_DISCONNECT:
      SNet_Disconnect( sock, cl, NULL );
      break;
   // not handled here / invalid messages
   default:
   case MSGT_CONNECT:
      break;
   }
}

static void SNet_ConstructEntityPackets( packet_t** _packs, uint* _amount )
{
   const int   maxpack = MAX_PACKET_SIZE / sizeof(ent_packet_t);
   int         amount = ceil(
      (float)(Ent_Count( ) * sizeof(ent_packet_t)) / MAX_PACKET_SIZE);
   packet_t*   packs;
   int         i, j;

   packs = Mem_Alloc( sizeof(packet_t) * amount );

   for( i = 0; i < amount; i++ )
   {
      packs[i].type = PACK_ENTITY;
      packs[i].hash = ~0;
      WIPEOUT( packs[i].data );

      for( j = 0; j < maxpack && (j * sizeof(ent_packet_t)) < MAX_PACKET_SIZE; j++ )
      {
         ent_packet_t*  pack =
            (ent_packet_t*) &packs[i].data[j * sizeof(ent_packet_t)];
         eobj_t*        ent = &eobjs[j + (maxpack * i)];

         if( !ent->edecl )
            continue;

         pack->valid = TRUE;

         pack->player = ent->player;
         pack->data = ent->data;
         pack->decl = *ent->edecl;
         pack->mat = ent->mat;

         pack->decl.spawn = 0;
         pack->decl.update = 0;
         pack->decl.kill = 0;
         pack->decl.create = 0;
      }
   }

   *_packs = packs;
   *_amount = amount;
}

static void SNet_ProcessPackets( bmsock_t* sock )
{
   packet_t             pack;
   packet_t*            ents;
   uint                 entamount, i;

   struct sockaddr_in   addr;
   int                  len = sizeof(addr);
   sockclient_t*        cl;

   WIPEOUT( pack );
   SNet_ConstructEntityPackets( &ents, &entamount );

   if(recvfrom( sock->packet, (char*) &pack, sizeof(pack),
         0, (struct sockaddr*) &addr, &len ) == EWOULDBLOCK)
      return;
   if( !pack.type || !pack.hash )
      return;

   cl = SNet_FindClientWithHash( sock, pack.hash );
   if( !cl )
   {
      SNet_Disconnect( sock, cl, "invalid packet hash!\n" );
      return;
   }

   switch( pack.type )
   {
   case PACK_MOVE:
   {
      /*
         WARNING!!!!
         VERY HACKY!!!!
         FIND A BETTER WAY PLEASE
      */
      if( !pack.data[0] )
         break;
      memcpy( &inmove, &pack.data[1], sizeof(inmove) );
      if( cl->ent )
         cl->ent->edecl->update( cl->ent->clazz, true );
      WIPEOUT( inmove );
      break;
   }
   case PACK_ENTITY:
   default: break;
   }

   for( i = 0; i < entamount; i++ )
   {
      sendto( sock->packet, (char*) &ents[i], sizeof(ents[i]), 0,
         (struct sockaddr*) &addr, len );
   }

   Mem_Free( ents );
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
