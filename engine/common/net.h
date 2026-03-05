#ifndef NET_H
#define NET_H

#include <common/define.h>

#define NETCHAN_LOOPBACK_SIZE 512
#define NET_DEFAULT_PORT      37215

typedef enum netchkind_e
{
   NETCHAN_LOOPBACK = 0,
   NETCHAN_NET = 1,
}
netchkind_t;

typedef struct netchan_s
{
   netchkind_t kind;
   uint        handle;
   byte        addr[4];
   uint16_t    port;

   byte*       loopback;
   uint        loopback_bytes;
}
netchan_t;

void Net_Init( void );
void Net_Quit( void );

void Netchan_Init( netchan_t* ch, netchkind_t kind, byte* addr, uint16_t port );
void Netchan_Quit( netchan_t* ch );

#endif
