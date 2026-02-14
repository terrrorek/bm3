
#ifndef CLIENT_H
#define CLIENT_H

#include <common/common.h>
#include <net/net.h>
#include <net/chan.h>

extern bool       clientConnected;
extern netchan_t  clientChan;

void Client_Init( void );
void Client_Update( void );
void Client_Quit( void );

#endif
