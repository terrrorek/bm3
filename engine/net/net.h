
#ifndef NET_H
#define NET_H

#include <common/common.h>
#include <net/def.h>
#include <net/chan.h>

#define NET_PORT 21735

typedef struct netchan_s
{
   channel_t   chan;
   uint32_t    lastack, seq;
}
netchan_t;

void Net_Init( void );
void Net_Quit( void );

void Net_ClientLoop( void );
void Net_HostLoop( void );

#endif
