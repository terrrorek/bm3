#ifndef NET_H
#define NET_H

#include <common/common.h>

typedef enum socket_flags_e
{
   SOCK_BOUND     = 0x1,
   SOCK_CONNECTED = 0x2,
}
socket_flags_t;

typedef struct socket_s
{
   size_t         packetHandle;
   size_t         cmdHandle;
   socket_flags_t flags;
}
socket_t;

extern socket_t   gamesock, hostsock;

void Net_InitSocket( socket_t* sock );
void Net_DestroySocket( socket_t* sock );

void Net_Init( void );
void Net_Quit( void );

void Net_Bind( socket_t* sock, uint16_t port );
void Net_Connect( socket_t* sock, const char* addr, uint16_t port );
void Net_Disconnect( socket_t* sock );

#endif
