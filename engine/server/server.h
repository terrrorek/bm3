#ifndef SERVER_H
#define SERVER_H
#include <common/define.h>
#include <common/cvar.h>
#include <net/net.h>
#include <game/player.h>
#include <game/ent.h>
#undef bool

#define SERVER_MAX_SNAPSHOTS 32

extern cvar_t     dedicated;

typedef struct server_client_s
{
   bool        taken;
   netaddr_t   addr;
   uint        pid;
}
server_client_t;

typedef struct server_snapshot_s
{
   econ_t   entities[ENT_MAX_ENTITIES];
}
server_snapshot_t;

typedef struct server_state_s
{
   server_client_t      clients[GAME_MAX_PLAYERS];
   player_t             players[GAME_MAX_PLAYERS];
   server_snapshot_t**  snapshots;
   bool                 running;
}
server_state_t;

extern server_state_t   sv_state;

void SV_Init( void );
void SV_Quit( void );
void SV_Update( void );

void SV_DataPacketUpdate( byte* buf, netaddr_t* from );
void SV_MsgPacketUpdate( byte* buf, netaddr_t* from );
void SV_PacketFrame( void );

#endif
