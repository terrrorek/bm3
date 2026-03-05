#ifndef SERVER_H
#define SERVER_H
#include <common/define.h>
#include <common/cvar.h>
#include <common/net.h>

extern cvar_t     dedicated;
extern netchan_t  sv_netchan;

void SV_Init( void );
void SV_Quit( void );
void SV_Update( void );

#endif
