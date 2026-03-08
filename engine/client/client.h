#ifndef CLIENT_H
#define CLIENT_H

#include <common/define.h>
#include <net/net.h>
#include <game/player.h>

typedef enum client_step_e
{
   CLIENT_MENU,
   CLIENT_CONNECTING,
   CLIENT_CONNECTED
}
client_step_t;

typedef struct client_state_s
{
   char           name[64];
   client_step_t  step;
   uint32_t       pid;
   player_t       players[GAME_MAX_PLAYERS];
   netaddr_t      destaddr;
}
client_state_t;

extern client_state_t   cl_state;

void CL_Connect( netaddr_t addr );
void CL_Disconnect( void );

void CL_Init( void );
void CL_Quit( void );
void CL_Update( void );

void CL_DataPacketUpdate( byte* buf, netaddr_t* from );
void CL_MsgPacketUpdate( byte* buf, netaddr_t* from );
void CL_PacketFrame( void );

#endif
