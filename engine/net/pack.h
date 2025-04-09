#ifndef PACK_H
#define PACK_H

#include <common/common.h>

typedef enum pack_type_e
{
   PACK_ENTITY = 1, // server entities
   PACK_MOVE, // client movement
}
pack_type_t;

#define MAX_PACKET_SIZE 1024

typedef struct packet_s
{
   byte     type;
   uint32_t hash;
   char     data[MAX_PACKET_SIZE];
}
packet_t;

#endif
