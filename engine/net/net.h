#ifndef NET_H
#define NET_H

#include <common/common.h>
#include <net/pack.h>
#include <net/msg.h>

#ifdef _WIN32
#include <WinSock2.h>
WSADATA  wsData;
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#endif


#ifdef _WIN32
#undef EWOULDBLOCK
#undef EMSGSIZE
#define EMSGSIZE WSAEMSGSIZE
#define EWOULDBLOCK WSAEWOULDBLOCK
#define WINERR\
   code = WSAGetLastError( );
#else
#define WINERR
#endif

typedef enum socket_flags_e
{
   SOCK_BOUND     = 0x1,
   SOCK_CONNECTED = 0x2,
   SOCK_ACCEPTED  = 0x4,
}
socket_flags_t;

#define MAX_CLIENTS 16

typedef struct socket_s
{
   size_t         packetHandle;

   socket_flags_t flags;

   uint32_t       addr;
   uint16_t       port;
}
socket_t;

extern socket_t   gamesock, hostsock;
extern char       playername[32];
extern byte       netkey[16];

extern bool       isDedicated;

void Net_InitSocket( socket_t* sock );
void Net_DestroySocket( socket_t* sock );

void Net_Init( void );
void Net_Quit( void );

void Net_Host( void );
void Net_EndHost( void );

void Net_ClientLoop( socket_t* sock );
void Net_ServerLoop( socket_t* sock );

void Net_Bind( socket_t* sock, uint16_t port );

bool Net_Connect( socket_t* sock, const char* addr, uint16_t port );
void Net_Disconnect( socket_t* sock );

void Net_SendPacket( socket_t* sock, packet_t* pack );
void Net_RecvPacket( socket_t* sock, packet_t* packet );
void Net_GetPacketHeader( socket_t* sock, packet_t* packet );

uint32_t Net_HashName( void );

#endif
