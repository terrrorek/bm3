#ifndef NET_H
#define NET_H

#include <common/common.h>
#include <common/cmd.h>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <WinSock2.h>
#  include <ws2tcpip.h>
#else
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#endif

typedef struct sockaddr_in
   addr_t;

typedef struct sockcl_s
{
   char     name[32];
   uint32_t hash;
   addr_t   address;
   float    last_packet;
}
sockcl_t;

typedef struct sock_s
{
   size_t   handle;
   bool     bound;
}
sock_t;

#ifdef _WIN32
#define WINERR\
   result = WSAGetLastError( )
#undef EWOULDBLOCK
#undef EMSGSIZE
#define EWOULDBLOCK WSAEWOULDBLOCK
#define EMSGSIZE WSAEMSGSIZE
#endif

#define Net_Reset( sock )\
   Net_Close( sock ); Net_Open( sock )

inline addr_t Net_MakeAddress( const char* addr, uint16_t port )
{
   addr_t address;
   address.sin_family = AF_INET;
   address.sin_port = htons( port );
   inet_pton( AF_INET, addr, &address.sin_addr.s_addr );
   return address;
}

void     Net_Init( void );
void     Net_Quit( void );

void     Net_Open( sock_t* sock );
int      Net_Send( sock_t* sock, void* data, size_t size, addr_t* addr );
int      Net_Receive( sock_t* sock, void* data, size_t size, addr_t* addr );
void     Net_Bind( sock_t* sock, addr_t* addr );
void     Net_Close( sock_t* sock );

uint32_t Net_GetUserHash( void );

// server
void     SNet_Host( void );
void     SNet_Update( void );
void     SNet_EndHost( void );

// client
int      CNet_Connect( addr_t* addr );
void     CNet_Update( void );
void     CNet_Disconnect( void );

#define MAX_PLAYERS 4
#define MAX_PLAYERNAME 32

extern   sock_t   net_client, net_server;
extern   char     playername[MAX_PLAYERNAME];

extern   sockcl_t net_serverclients[MAX_PLAYERS];
extern   addr_t   net_serveraddress;

#endif
