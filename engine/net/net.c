#include <net/net.h>
#include <common/cmd.h>
#include <common/cvar.h>
#include <game/console.h>

#ifdef _WIN32
#include <WinSock2.h>
WSADATA  wsData;
#endif

socket_t gamesock = {0}, hostsock = {0};

static void Net_SocketInfo( socket_t* sock )
{
   Con_Printf( "SOCKET:\n" );
   Con_Printf( "   packet: %i\n", sock->packetHandle );
   Con_Printf( "   cmd: %i\n", sock->cmdHandle );
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
   int   cmdtype = SOCK_STREAM;
   int   pcktype = SOCK_DGRAM;

   #ifndef _WIN32
   cmdtype |= SOCK_NONBLOCK;
   pcktype |= SOCK_NONBLOCK;
   #endif

   WIPEPTR( sock );
   sock->packetHandle = socket( AF_INET, pcktype, IPPROTO_UDP );
   sock->cmdHandle = socket( AF_INET, cmdtype, IPPROTO_TCP );

   if( !sock->packetHandle || !sock->cmdHandle )
      Cmd_Errorf( "failed to open packet and/or command sockets.\n" );

   ioctlsocket( sock->packetHandle, FIONBIO, NULL );
   ioctlsocket( sock->cmdHandle, FIONBIO, NULL );
}

void Net_DestroySocket( socket_t* sock )
{
   if( sock->cmdHandle )
      closesocket( sock->cmdHandle );
   if( sock->packetHandle )
      closesocket( sock->packetHandle );
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

   sock->flags |= SOCK_BOUND;
}

void Net_Connect( socket_t* sock, const char* addr, uint16_t port )
{
   uint32_t    address;

   Con_Printf( "trying connect to %s:%i...\n", addr, port );

   if( !sock->cmdHandle || !sock->packetHandle )
   {
      Con_Errorf( "failed to connect to \"%s\". socket is not initialized.\n", addr );
      Net_InitSocket( &gamesock );
      return;
   }

   address = inet_addr( addr );

   struct sockaddr_in sockaddr;
   sockaddr.sin_addr.s_addr = address;
   sockaddr.sin_port = htons( port );
   sockaddr.sin_family = AF_INET;

   if( connect( sock->packetHandle,
      (const struct sockaddr*) &sockaddr, sizeof(sockaddr) ) != 0 )
   {
      Con_Errorf( "failed to connect to \"%s\".", addr );
      return;
   }

   Con_Printf( "connected to %s:%i.\n", addr, port );
   sock->flags |= SOCK_CONNECTED;
}

void Net_Disconnect( socket_t* sock )
{
   Net_DestroySocket( sock );
   Net_InitSocket( sock );
}
