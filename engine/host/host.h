
#ifndef HOST_H
#define HOST_H

#include <common/common.h>
#include <net/net.h>
#include <net/chan.h>

extern netchan_t hostChan;

void Host_Init( void );
void Host_Update( void );
void Host_Quit( void );

#endif
