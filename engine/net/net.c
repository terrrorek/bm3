#include <net/net.h>
#include <common/cmd.h>
#include <common/cvar.h>
#include <game/console.h>
#include <common/alloc.h>
#include <common/bmstring.h>

#include <3rdparty/blake2/blake2.h>

byte        netkey[16] = "czokomastek";

socket_t    gamesock = {0}, hostsock = {0};

char        playername[32] = "player";

uint32_t Net_HashName( void )
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
   .function = (confunc_t) Net_SockState_f,
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

   WIPEOUT( gamesock );
   WIPEOUT( hostsock );

   Net_InitSocket( &gamesock );

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

void Net_Host( void )
{
   Net_DestroySocket( &hostsock );
   Net_InitSocket( &hostsock );
   Net_Bind( &hostsock, 21371 );
}

void Net_EndHost( void )
{
   Net_DestroySocket( &hostsock );
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
   packet_t    pack;

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

   sock->flags |= SOCK_CONNECTED;
   return TRUE;
}

void Net_Disconnect( socket_t* sock )
{
   Net_DestroySocket( sock );
   Net_InitSocket( sock );
}

#define EXTENDED_BUS 4
void Net_SendPacket( socket_t* sock, packet_t* pack )
{
   if( !sock->packetHandle )
      return;

   if( sock->flags & SOCK_BOUND )
   {
      send( sock->packetHandle, (char*)pack, sizeof(*pack), 0 );
   }
   else
   {
      struct sockaddr_in sockaddr;
      int   len = sizeof(sockaddr);

      sockaddr.sin_addr.s_addr = sock->addr;
      sockaddr.sin_port = htons( sock->port );
      sockaddr.sin_family = AF_INET;

      sendto( sock->packetHandle, (char*)pack, sizeof(*pack), 0,
         (struct sockaddr*) &sockaddr, len );
   }
}

void Net_RecvPacket( socket_t* sock, packet_t* packet )
{
   int   code;

   WIPEPTR(packet);

   if( sock->flags & SOCK_BOUND )
   {
      if(( code = recv(sock->packetHandle, (char*)packet, sizeof(*packet), 0) ) != 0 )
      {
         WINERR
         if( code && code != EWOULDBLOCK && code != EMSGSIZE )
         {
            Con_Errorf( "failed to receive packet. (%i)\n", code );
            return;
         }
         if( code != EMSGSIZE )
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

      if(( code = recvfrom(sock->packetHandle, (char*)packet, sizeof(*packet),
         0, (struct sockaddr*) &sockaddr, &len) ) != 0 )
      {
         WINERR
         if( code && code != EWOULDBLOCK )
         {
            Con_Errorf( "failed to receive packet. (%i)\n", code );
            return;
         }
         if( code != EMSGSIZE )
            return;
      }
   }
}

void Net_GetPacketHeader( socket_t* sock, packet_t* packet )
{
   int         code;
   const int   packsize = sizeof(packet_t) - MAX_PACK_SIZE;

   if( !sock->packetHandle )
      return;

   if( sock->flags & SOCK_BOUND )
   {
      if(( code = recv( sock->packetHandle, (char*) packet,
         packsize, MSG_PEEK )) != 0 )
      {
         WINERR
         if( code && code != EWOULDBLOCK && code != EMSGSIZE )
         {
            Con_Errorf( "failed to get packet header (%i).\n", code );
            return;
         }
         if( code != EMSGSIZE)
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

      if(( code = recvfrom( sock->packetHandle, (char*) packet,
         packsize, MSG_PEEK, (struct sockaddr*) &sockaddr, &len )) != 0 )
      {
         WINERR
         if( code && code != EWOULDBLOCK && code != EMSGSIZE )
         {
            Con_Errorf( "failed to get packet header (%i).\n", code );
            return;
         }
         if( code != EMSGSIZE)
            return;
      }
   }
}
