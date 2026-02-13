
#ifndef MSG_H
#define MSG_H

#include <common/common.h>
#include <common/chan.h>
#include <game/edecl.h>
#include <game/eobj.h>

/*
   Messages are the engine's way of sending data packets
   over it's channels.
*/

typedef enum msg_type_e
{
   MSG_EMPTY = 0,
   MSG_CONNECT,
   MSG_DISCONNECT,

   MSG_ENTCHUNK,

   MSG_INVALID,
} msg_type_t;

#define MSG_TYPE(name) typedef struct msg_##name##_s

// message types
MSG_TYPE(connect)
{
   uint16_t clid;
   bool isLocal;
   byte bus;
   uint32_t addr;
   uint16_t port;
   uint64_t digest;
} msg_connect_t;
MSG_TYPE(disconnect)
{
   uint16_t clid;
} msg_disconnect_t;

// ent chunks are packets containing eobj's that
// will be send to the client. They contain MAX_ENTCHUNK_ENTS amount of eobjs.

#define MAX_ENTCHUNK_ENTS 32

MSG_TYPE(entchunk)
{
   byte     bus;
   byte     declCount, objCount;

   edecl_t  decls[MAX_ENTCHUNK_ENTS];
   eobj_t   objs[MAX_ENTCHUNK_ENTS];
} msg_entchunk_t;

#define MAX_TRIES 4

static inline bool Msg_Send( 
   msg_type_t type, byte* data, bool response,
   uint dataLen, channel_t* chan, byte bus )
{
   if( chan->pending[bus] )
      return FALSE;

   chan->data[bus][0] = (byte) type;
   chan->data[bus][1] = response;
   memcpy( &chan->data[bus][4], data, dataLen );
   chan->len = dataLen + 4;

   return Chan_SendToRemotes( chan, bus );
}

static inline bool Msg_SendToRemote(
   msg_type_t type, byte* data, bool response,
   uint dataLen, channel_t* chan, byte bus, uint rem )
{
   if( chan->pending[bus] )
      return FALSE;

   chan->data[bus][0] = (byte) type;
   chan->data[bus][1] = response;
   memcpy( &chan->data[bus][4], data, dataLen );
   chan->len = dataLen + 4;

   return Chan_SendToRemote( chan, rem, bus );
}

uint Msg_ConstructEntChunks( msg_entchunk_t** dest );

#endif
