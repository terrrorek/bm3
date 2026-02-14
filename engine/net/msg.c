
#include <net/msg.h>
#include <common/cmd.h>
#include <game/eobj.h>

void Msg_Construct(
   msg_header_t* header, bitbuffer_t* bitbuf,
   byte* buffer, size_t buflen
)
{
   if( buflen < sizeof(msg_header_t) )
      return;

   memset( buffer, 0, buflen );
   memcpy( buffer, header, sizeof(msg_header_t) );
   memcpy(
      buffer + sizeof(msg_header_t),
      bitbuf->buffer,
      bitbuf->size >= (buflen - sizeof(msg_header_t)) ?
         (buflen - sizeof(msg_header_t)) : bitbuf->size
   );
}
