#include <net/pack.h>
#include <net/net.h>
#include <common/cmd.h>
#include <common/alloc.h>
#include <3rdparty/blake2/blake2.h>

void Net_ConstructPacket( packet_t* pack,
   enum pack_type_e type, enum pack_format_t format,
   void* data, uint datasize )
{
   WIPEPTR( pack );

   pack->type = type;
   pack->format = format;

   memcpy( pack->data, data, datasize );
   blake2s( &pack->hash, sizeof(pack->hash), pack->data, datasize, netkey, sizeof(netkey) );
}

void Net_DissolvePacket( packet_t* pack )
{
   WIPEPTR( pack );
}
