
#ifndef CHAN_H
#define CHAN_H

#include <common/common.h>
#include <net/def.h>

typedef struct socket_s
{
   size_t   handle;
} socket_t;
typedef struct addr_s
{
   uint32_t base;
   uint16_t port;
}
addr_t;

#define MAX_CHANNEL_BUF_SIZE BIT(8)

typedef enum channel_flags_e
{
   CHAN_NONE = 0x0,
}
channel_flags_t;
typedef struct channel_s
{
   channel_flags_t      flags;
   socket_t             socket;
   addr_t               dest;

   struct sockaddr_in   inaddr;
   byte                 outcoming[MAX_CHANNEL_BUF_SIZE];
   byte                 incoming[MAX_CHANNEL_BUF_SIZE];
   bool                 inavail;
}
channel_t;

void Chan_OpenChannel( channel_t* chan, channel_flags_t flags );
void Chan_OpenBoundChannel( channel_t* chan, channel_flags_t flags, addr_t addr );
void Chan_CloseChannel( channel_t* chan );

void Chan_Receive( channel_t* chan );
void Chan_Send( channel_t* chan );

#endif
