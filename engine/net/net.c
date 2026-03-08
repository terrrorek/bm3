#include <net/net.h>
#include <common/cmd.h>
#include <server/server.h>
#include <client/client.h>
#include <common/event.h>

#include <string.h>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <WinSock2.h>
#  define WINERR  WSAGetLastError()
#  undef EWOULDBLOCK
#  define EWOULDBLOCK WSAEWOULDBLOCK
#else
#  include <errno.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <fcntl.h>
#  define WINERR  err != 0 ? errno : 0
#  define closesocket close
#endif

#ifdef _WIN32
static WSADATA wsadata;
#else
#endif

#undef bool

nethandle_t sockets[4] = {0};

static void Net_AddrToSockaddr( netaddr_t* addr, struct sockaddr_in* sockaddr )
{
   sockaddr->sin_addr.S_un.S_addr = addr->addr;
   sockaddr->sin_port = htons( addr->port );
   sockaddr->sin_family = AF_INET;
}

static void Net_OpenSocket( nethandle_t* han, netaddr_t addr, int type, int proto )
{
   int                  err;
   size_t               ctl;
   struct sockaddr_in   sockaddr;

   *han = socket( AF_INET, type, proto );
   err = WINERR;
   if( err != 0 )
      Cmd_Errorf( "failed to open socket (%i)\n", err );

#ifdef _WIN32
   ctl = 1;
   ioctlsocket( *han, FIONBIO, (u_long*) &ctl );
#else
   ctl = fcntl( *han, F_GETFL, 0 );
   fcntl( *han, F_SETFL, ctl | O_NONBLOCK );
#endif

   Net_AddrToSockaddr( &addr, &sockaddr );

   err = bind( *han, (struct sockaddr*) &sockaddr, sizeof(sockaddr) );
   err = WINERR;
   if( err != 0 )
      Cmd_Errorf( "failed to bind socket (%i)\n", err );
}

static void Net_CloseSocket( nethandle_t han )
{
   closesocket( han );
}

void Net_Init( void )
{
   int err;

#ifdef _WIN32
   err = WSAStartup( MAKEWORD(2, 2), &wsadata );
   if( err != NO_ERROR )
      Cmd_Errorf( "winsock initialization error (%i)\n", err );
#else
#endif

   Net_OpenSocket( &sockets[SOCK_DATA_CLIENT], (netaddr_t){0, 0}, SOCK_DGRAM, IPPROTO_UDP );
   Net_OpenSocket( &sockets[SOCK_DATA_SERVER], (netaddr_t){0, NET_DATA_DEFAULT_PORT}, SOCK_DGRAM, IPPROTO_UDP );
   Net_OpenSocket( &sockets[SOCK_MSG_CLIENT], (netaddr_t){0, 0}, SOCK_DGRAM, IPPROTO_UDP );
   Net_OpenSocket( &sockets[SOCK_MSG_SERVER], (netaddr_t){0, NET_MSG_DEFAULT_PORT}, SOCK_DGRAM, IPPROTO_UDP );
}

void Net_Quit( void )
{
   Net_CloseSocket( sockets[SOCK_DATA_SERVER] );
   Net_CloseSocket( sockets[SOCK_DATA_CLIENT] );
   Net_CloseSocket( sockets[SOCK_MSG_SERVER] );
   Net_CloseSocket( sockets[SOCK_MSG_CLIENT] );

#ifdef _WIN32
   WSACleanup( );
#else
#endif
}

void Net_Update( void )
{
   netaddr_t   addr = {0, 0};
   event_t     ev = {.data = {0}};

   ev.kind = EVENT_CL_PACKET;
   ev.data.packet.sock = SOCK_DATA_CLIENT;
   while( Net_Recv( SOCK_DATA_CLIENT, ev.data.packet.buffer, NET_PACKET_SIZE, &addr ) )
   {
      ev.data.packet.addr = addr;
      Event_Push( &ev );
   }
   ev.data.packet.sock = SOCK_MSG_CLIENT;
   while( Net_Recv( SOCK_MSG_CLIENT, ev.data.packet.buffer, NET_PACKET_SIZE, &addr ) )
   {
      ev.data.packet.addr = addr;
      Event_Push( &ev );
   }

   ev.kind = EVENT_SV_PACKET;
   ev.data.packet.sock = SOCK_DATA_SERVER;
   while( Net_Recv( SOCK_DATA_SERVER, ev.data.packet.buffer, NET_PACKET_SIZE, &addr ) )
   {
      ev.data.packet.addr = addr;
      Event_Push( &ev );
   }
   ev.data.packet.sock = SOCK_MSG_SERVER;
   while( Net_Recv( SOCK_MSG_SERVER, ev.data.packet.buffer, NET_PACKET_SIZE, &addr ) )
   {
      ev.data.packet.addr = addr;
      Event_Push( &ev );
   }
}


void Net_Send( netsock_t sock, byte* buf, uint32_t size, netaddr_t addr )
{
   struct sockaddr_in sockaddr;
   int err;

   Net_AddrToSockaddr( &addr, &sockaddr );

   err = sendto( sockets[sock], (char*) buf, size, 0, (struct sockaddr*) &sockaddr, sizeof(sockaddr) );
   err = WINERR;

   if( err != 0 && err != EWOULDBLOCK )
      Cmd_Errorf( "failed to send socket (%i)\n", err );
}
bool Net_Recv( netsock_t sock, byte* buf, uint32_t size, netaddr_t* addr )
{
   struct sockaddr_in sockaddr;
   int32_t socksize = sizeof(sockaddr);
   int err;

   err = recvfrom( sockets[sock], (char*) buf, size, 0, (struct sockaddr*) &sockaddr, &socksize );
   err = WINERR;

   if( err != 0 && err != EWOULDBLOCK )
   {
      Cmd_Errorf( "failed to recv socket (%i)\n", err );
      return false;
   }
   else if( err == EWOULDBLOCK )
      return false;

   addr->addr = sockaddr.sin_addr.S_un.S_addr;
   addr->port = ntohs( sockaddr.sin_port );

   return true;
}
