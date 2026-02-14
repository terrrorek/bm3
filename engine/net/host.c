#include <net/net.h>
#include <net/msg.h>
#include <net/bitfield.h>
#include <net/chan.h>
#include <host/host.h>
#include <game/state.h>

void Net_HostLoop( void )
{
   byte           buffer[MAX_CHANNEL_BUF_SIZE - sizeof(msg_header_t)];
   msg_header_t*  header = (msg_header_t*) &hostChan.chan.incoming[0];
   msg_header_t   sendheader = {0};

   bitbuffer_t    bitbuf = {
      .buffer = &hostChan.chan.incoming[sizeof(msg_header_t)],
      .size = MAX_CHANNEL_BUF_SIZE - sizeof(msg_header_t),
      .offset = 0
   };
   bitbuffer_t    sendbitbuf = {
      .buffer = buffer,
      .size = sizeof(buffer),
      .offset = 0
   };

   hostChan.chan.inavail = FALSE;
   Chan_Receive( &hostChan.chan );

   while( hostChan.chan.inavail )
   {
      switch( header->type )
      {
      default:
         break;
      }

      Chan_Receive( &hostChan.chan );
   }

   hostChan.seq++;
}
