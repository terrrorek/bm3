#include <net/net.h>
#include <common/cmd.h>
#include <common/cvar.h>
#include <game/console.h>
#include <common/alloc.h>
#include <common/bmstring.h>

#include <3rdparty/blake2/blake2.h>

const static char    netkey[] = "czokomastek";

socket_t    gamesock = {0}, hostsock = {0};

char        playername[32] = "player";

static int Net_HashName( void )
{
   int   x;
   void* out;

   out = Mem_Alloc( sizeof(int) );
   blake2s( out, sizeof(int), playername, sizeof(playername), netkey, sizeof(netkey) );
   x = *(int*)out;
   Mem_Free( out );

   return x;
}

static void Net_SocketInfo( socket_t* sock )
{
   Con_Printf( "SOCKET:\n" );
   Con_Printf( "   packet: %i\n", sock->packetHandle );
   Con_Printf( "   flags: %x\n", sock->flags );
}

void Net_SockState_f( void )
{
   Net_SocketInfo( &gamesock );
   Net_SocketInfo( &hostsock );
}

cvar_t net_sockstate_f = {
   .name = "net_sockstate",
   .function = Net_SockState_f,
   .flags = CVAR_FUNCTION,
};

void Net_InitSocket( socket_t* sock )
{
   int   type = SOCK_DGRAM;

   #ifndef _WIN32
   type |= SOCK_NONBLOCK;
   #endif

   WIPEPTR( sock );
   sock->packetHandle = socket( AF_INET, type, IPPROTO_UDP );

   #ifdef _WIN32
   unsigned long blocking = TRUE;
   if( ioctlsocket( sock->packetHandle, FIONBIO, &blocking ) != 0 )
      Cmd_Errorf( "failed to open packet socket.\n" );
   #endif
}

void Net_DestroySocket( socket_t* sock )
{
   if( sock->packetHandle )
      closesocket( sock->packetHandle );

   sock->packetHandle = 0;
   sock->addr = 0;
   sock->port = 0;
}

void Net_Init( void )
{
   #ifdef _WIN32
   int   err;
   WORD  req = MAKEWORD(2, 2);

   err = WSAStartup( req, &wsData );
   if( err != 0 )
      Cmd_Errorf( "failed to init winsock2\n" );

   #endif

   Net_InitSocket( &gamesock );
   Net_InitSocket( &hostsock );
   Net_Bind( &hostsock, 21371 );

   CVar_Register( &net_sockstate_f );
}

void Net_Quit( void )
{
   Net_DestroySocket( &gamesock );
   Net_DestroySocket( &hostsock );

   #ifdef _WIN32
   WSACleanup( );
   #endif
}

void Net_Bind( socket_t* sock, uint16_t port )
{
   struct sockaddr_in sockaddr;

   if( sock->flags & SOCK_BOUND || sock->flags & SOCK_CONNECTED )
   {
      Con_Errorf( "failed to bind socket. already is bound/connected.\n" );
   }

   sockaddr.sin_addr.s_addr = inet_addr( "0.0.0.0" );
   sockaddr.sin_port = htons( port );
   sockaddr.sin_family = AF_INET;

   if( bind( sock->packetHandle,
      (const struct sockaddr*) &sockaddr, sizeof(sockaddr) ) != 0 )
   {
      Con_Errorf( "failed to bind socket to \"0.0.0.0\".\n" );
      return;
   }

   sock->addr = sockaddr.sin_addr.s_addr;
   sock->port = port;
   sock->flags |= SOCK_BOUND;
}

bool Net_Connect( socket_t* sock, const char* addr, uint16_t port )
{
   uint32_t    address;
   msg_t       msg;

   Con_Printf( "trying connect to %s:%i...\n", addr, port );

   if( !sock->packetHandle )
   {
      Con_Errorf( "failed to connect to \"%s\". socket is not initialized.\n", addr );
      Net_InitSocket( &gamesock );
      return FALSE;
   }

   address = inet_addr( addr );
   sock->addr = address;
   sock->port = port;

   msg.type = MSG_TYPE_CONNECT;
   BM_snprintf( (char*)msg.data, 256, "%i\0", Net_HashName( ) );
   Net_SendMessage( sock, &msg );

   Con_Printf( "connected to %s:%i.\n", addr, port );
   sock->flags |= SOCK_CONNECTED;
}

void Net_Disconnect( socket_t* sock )
{
   Net_DestroySocket( sock );
   Net_InitSocket( sock );
}

void Net_SendPacket( socket_t* sock, packet_t* pack )
{
   void* msg;
   uint  packsize;

   if( !sock->packetHandle )
      return;

   packsize = sizeof(packet_t*) - sizeof(void*) - sizeof(uint);
   msg = Mem_Alloc( packsize + pack->datasize + 1 );
   ((byte*)msg)[0] = '\x2';

   memcpy( (void*)((uintptr_t)msg + 1), pack, packsize );
   memcpy( (void*)((uintptr_t)msg + 1 + packsize), pack->data, pack->datasize );

   if( sock->flags & SOCK_BOUND )
   {
      send( sock->packetHandle, msg, packsize + pack->datasize + 1, 0 );
   }
   else
   {
      struct sockaddr_in sockaddr;
      int   len = sizeof(sockaddr);

      sockaddr.sin_addr.s_addr = sock->addr;
      sockaddr.sin_port = htons( sock->port );
      sockaddr.sin_family = AF_INET;

      sendto( sock->packetHandle, msg, packsize + pack->datasize + 1, 0,
         (struct sockaddr*) &sockaddr, len );
   }
}

void Net_SendMessage( socket_t* sock, msg_t* msg )
{
   if( !sock->packetHandle )
      return;

   void* fmsg = Mem_Alloc( sizeof(msg_t) + 1 );
   ((byte*)fmsg)[0] = '\x1';
   memcpy( (void*)((uintptr_t)fmsg + 1), msg, sizeof(msg_t) );

   if( sock->flags & SOCK_BOUND )
   {
      send( sock->packetHandle, (char*) fmsg, sizeof(*msg) + 1, 0 );
   }
   else
   {
      struct sockaddr_in sockaddr;
      int   len = sizeof(sockaddr), code;

      sockaddr.sin_addr.s_addr = sock->addr;
      sockaddr.sin_port = htons( sock->port );
      sockaddr.sin_family = AF_INET;

      if( (code = sendto( sock->packetHandle, (char*) fmsg, sizeof(*msg) + 1, 0,
         (struct sockaddr*) &sockaddr, len )) != 0 )
      {
         WINERR
         if( code && code != EWOULDBLOCK )
            Con_Errorf( "failed to send msg to server. (%i)\n", code );
      }
   }
}

void Net_RecvPacket( socket_t* sock, packet_t* packet )
{
   int   code;
   int   packsize = sizeof(packet_t) - sizeof(void*) - sizeof(int);
   void* buf = Mem_Alloc( packsize + 1 );

   WIPEPTR(packet);

   if( sock->flags & SOCK_BOUND )
   {
      if(( code = recv(sock->packetHandle, buf, packsize + 1, 0) ) != 0 )
      {
         WINERR
         if( code && code != EWOULDBLOCK )
         {
            Con_Errorf( "failed to receive packet. (%i)\n", code );
            return;
         }
         return;
      }

      memcpy( packet, (void*)((uintptr_t)buf + 1), packsize );
      Mem_Free( buf );

      if( packet->datasize <= 0 )
      {
         Con_Errorf( "invalid packet size.\n" );
         return;
      }

      packet->data = Mem_Alloc( packet->datasize );

      if(( code = recv(sock->packetHandle, packet->data, packet->datasize, 0) ) != 0 )
      {
         WINERR
         if( code && code != EWOULDBLOCK )
         {
            Con_Errorf( "failed to receive packet data. (%i)\n", code );
            return;
         }
         return;
      }
   }
   else
   {
      struct sockaddr_in   sockaddr;
      int                  len = sizeof(sockaddr);

      sockaddr.sin_addr.s_addr = sock->addr;
      sockaddr.sin_port = htons( sock->port );
      sockaddr.sin_family = AF_INET;

      if(( code = recvfrom(sock->packetHandle, buf, packsize + 1,
         0, (struct sockaddr*) &sockaddr, &len) ) != 0 )
      {
         WINERR
         if( code && code != EWOULDBLOCK )
         {
            Con_Errorf( "failed to receive packet. (%i)\n", code );
            return;
         }
         return;
      }

      memcpy( packet, (void*)((uintptr_t)buf + 1), packsize );
      Mem_Free( buf );
      len = sizeof(sockaddr);

      if( packet->datasize <= 0 )
      {
         Con_Errorf( "invalid packet size.\n" );
         return;
      }

      packet->data = Mem_Alloc( packet->datasize );

      if(( code = recvfrom(sock->packetHandle, packet->data, packet->datasize,
         0, (struct sockaddr*)&sockaddr, &len ) ) != 0 )
      {
         WINERR
         if( code && code != EWOULDBLOCK )
         {
            Con_Errorf( "failed to receive packet data. (%i)\n", code );
            return;
         }
         return;
      }
   }
}

void Net_RecvMessage( socket_t* sock, msg_t* msg )
{
   int   code;
   void* buf = Mem_Alloc( sizeof(msg_t) + 1 );

   WIPEPTR(msg);

   if( sock->flags & SOCK_BOUND )
   {
      if(( code = recv(sock->addr, buf, sizeof(msg_t) + 1, 0) ) != 0)
      {
         WINERR
         if( code && code != EWOULDBLOCK )
         {
            Con_Errorf( "failed to receive message. (%i)\n", code );
            return;
         }
         return;
      }

      memcpy( msg, (void*)((uintptr_t)buf + 1), sizeof(msg_t) );
      Mem_Free( buf );
   }
   else
   {
      struct sockaddr_in   sockaddr;
      int                  len = sizeof(sockaddr);

      if(( code = recvfrom(sock->addr, buf, sizeof(msg_t) + 1,
         0, &sockaddr, &len ) ) != 0)
      {
         WINERR
         if( code && code != EWOULDBLOCK )
         {
            Con_Errorf( "failed to receive message. (%i)\n", code );
            return;
         }
         return;
      }

      memcpy( msg, (void*)((uintptr_t)buf + 1), sizeof(msg_t) );
      Mem_Free( buf );
   }
}

byte Net_GetPendingType( socket_t* sock )
{
   short type;
   int   code;

   if( sock->flags & SOCK_BOUND )
   {
      if(( code = recv(sock->packetHandle, (char*)&type, 2, MSG_PEEK) ) != 0 )
      {
         WINERR
         if( code && code != EWOULDBLOCK )
         {
            Con_Errorf( "failed to get pending type. (%i)\n", code );
            return 0;
         }
         return 0;
      }
   }
   else
   {
      struct sockaddr_in   addr;
      int                  len = sizeof(addr);

      addr.sin_addr.s_addr = sock->addr;
      addr.sin_port = htons( sock->port );
      addr.sin_family = AF_INET;

      if(( code = recvfrom(sock->packetHandle, (char*)&type, 1,
         MSG_PEEK, (struct sockadd*) &addr, &len) ) != 0 )
      {
         WINERR
         if( code && code != EWOULDBLOCK )
         {
            Con_Errorf( "failed to get pending type. (%i)\n", code );
            return 0;
         }
         return 0;
      }
   }

   return type;
}
