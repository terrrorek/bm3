#ifndef NET_H
#define NET_H

#include <common/define.h>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <WS2tcpip.h>
#endif

#define NET_DEFAULT_PORT      NET_DATA_DEFAULT_PORT
#define NET_DATA_DEFAULT_PORT 37215
#define NET_MSG_DEFAULT_PORT  NET_DATA_DEFAULT_PORT+1

#define NET_PACKET_SIZE       1400

typedef size_t nethandle_t;

typedef struct netaddr_s
{
   uint32_t addr;
   uint16_t port;
}
netaddr_t;

extern nethandle_t   sockets[4];
typedef enum netsock_e
{
   SOCK_DATA_CLIENT = 0, SOCK_DATA_SERVER = 1,
   SOCK_MSG_CLIENT = 2, SOCK_MSG_SERVER = 3,
}
netsock_t;

void Net_Init( void );
void Net_Quit( void );
void Net_Update( void );

void Net_Send( netsock_t sock, byte* buf, uint32_t size, netaddr_t addr );
bool Net_Recv( netsock_t sock, byte* buf, uint32_t size, netaddr_t* addr );

#endif
