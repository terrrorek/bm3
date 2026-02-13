
#ifndef CHAN_H
#define CHAN_H

#include <common/common.h>

typedef enum chan_flags_e
{
   CHAN_PACKET = 0x10,
   CHAN_INTERNAL = 0x20,

   CHAN_INVALID,
} chan_flags_t;

#define CHANNEL_DATA_SIZE BIT(6)
#define CHANNEL_BUS_AMOUNT BIT(4)

// Remotes are a channel's way of sending data to a specified destination.
// It could be a local channel, or a one across the network.
typedef struct remote_s
{
   // the parent channel
   struct channel_s* parent;
   uint              index;

   // local destination
   struct channel_s* channel; 

   // ipv4 destination
   uint32_t          address;
   uint16_t          port;
}
remote_t;

// Channels are the engine's way of host-client communication.
typedef struct channel_s
{
   char name[16];

   // type and flags of the channel
   chan_flags_t   type;
   
   byte           data[CHANNEL_BUS_AMOUNT][CHANNEL_DATA_SIZE];
   uint           len;
   uint           index;
   // if this is set, there is data waiting in a buffer at bus X.
   bool           pending[CHANNEL_BUS_AMOUNT];

   // the channel's destinations
   uint           remoteCount;
   remote_t**     remotes;
} channel_t;

#define MAX_CHANNELS BIT(7)
#define MAX_REMOTES  BIT(5)

extern channel_t  channels[MAX_CHANNELS];
extern remote_t   remotes[MAX_REMOTES];

remote_t*   Chan_OpenRemote( channel_t* parent );
channel_t*  Chan_OpenChannel( chan_flags_t type, bool allocRemote );

void        Chan_CloseRemote( remote_t* remote );
void        Chan_CloseChannel( channel_t* channel );

bool        Chan_SendToRemote( channel_t* channel, uint remote, byte bus );
bool        Chan_SendToRemotes( channel_t* channel, byte bus );

uint        Chan_AddRemote( channel_t* channel, remote_t* remote );
void        Chan_SendRemote( channel_t* channel, remote_t* remote, byte bus );
uint        Chan_GetRemoteIndex( channel_t* channel, remote_t* remote );
bool        Chan_IsDataPending( channel_t* channel, byte bus );

#endif
