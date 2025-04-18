#ifndef PACK_H
#define PACK_H

#include <common/common.h>
#include <game/eobj.h>

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
   byte     data[MAX_PACKET_SIZE];
}
packet_t;

typedef struct ent_packet_s
{
   bool     valid;
   uint64_t id;
   uint     index;
   edata_t  data;
   emat_t   mat;
   edecl_t  decl;
   uint32_t player;
}
ent_packet_t;

#endif
