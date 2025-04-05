#include <net/pack.h>
#include <common/alloc.h>

void Net_ConstructPacket( packet_t* pack,
   enum pack_type_e type, enum pack_format_t format,
   void* data, uint datasize )
{
   WIPEPTR( pack );

   pack->type = type;
   pack->format = format;

   pack->data = Mem_Alloc( datasize );
   pack->datasize = datasize;

   memcpy( pack->data, data, datasize );
}

void Net_DissolvePacket( packet_t* pack )
{
   if( pack->data )
      Mem_Free( pack->data );
   WIPEPTR( pack );
}
