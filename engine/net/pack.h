#ifndef PACK_H
#define PACK_H

#include <common/common.h>

enum pack_type_e
{
   PACK_TYPE_CTOS, // client(s) to server
   PACK_TYPE_STOC, // server to client(s)
   PACK_TYPE_UNIFORM, // uniform
};

enum pack_format_t
{  __BEGINNING,
   // uniform formats
   PACK_FMT_MESSAGE,

   // client formats
   PACK_FMT_INPUT, // player input

   // server formats
   PACK_FMT_ENTS, // server entities
};

#define MAX_PACK_SIZE 256

typedef struct packet_s
{
   byte     type;
   byte     format;

   short    sender; /*   <-- client id or 0 for server | */
   short    receiver; /* <-----------------------------| */
   uint32_t hash; // msg data hash (blake2s)

   byte     data[MAX_PACK_SIZE];
}
packet_t;

void Net_ConstructPacket( packet_t* msg,
   enum pack_type_e type, enum pack_format_t format,
   void* data, uint datasize );
void Net_DissolvePacket( packet_t* msg );

#endif
