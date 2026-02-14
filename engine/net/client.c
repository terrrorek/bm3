#include <net/net.h>
#include <net/msg.h>
#include <net/bitfield.h>
#include <net/chan.h>
#include <client/client.h>
#include <game/state.h>

void Net_ClientLoop( void )
{
   byte           buffer[MAX_CHANNEL_BUF_SIZE - sizeof(msg_header_t)];
   msg_header_t*  header = (msg_header_t*) &clientChan.chan.incoming[0];
   msg_header_t   sendheader = {0};

   bitbuffer_t    bitbuf = {
      .buffer = &clientChan.chan.incoming[sizeof(msg_header_t)],
      .size = MAX_CHANNEL_BUF_SIZE - sizeof(msg_header_t),
      .offset = 0
   };
   bitbuffer_t    sendbitbuf = {
      .buffer = buffer,
      .size = sizeof(buffer),
      .offset = 0
   };

   Chan_Receive( &clientChan.chan );
   while( clientChan.chan.inavail )
   {
      switch( header->type )
      {
      default:
         break;
      }

      Chan_Receive( &clientChan.chan );
   }

   clientChan.seq++;
}
