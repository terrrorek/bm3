
#include <net/chan.h>
#include <net/net.h>
#include <common/alloc.h>
#include <common/cmd.h>

void Chan_OpenChannel( channel_t* chan, channel_flags_t flags )
{
   Chan_OpenBoundChannel( chan, flags, (addr_t){0} );
}

void Chan_OpenBoundChannel( channel_t* chan, channel_flags_t flags, addr_t addr )
{
   int err;

   WIPEPTR( chan );
   chan->flags = flags;
   chan->socket.handle = socket( AF_INET, SOCK_DGRAM, 0 );

#ifdef _WIN32
   u_long mode = 1;
   ioctlsocket( chan->socket.handle, FIONBIO, &mode );
#else
   int flags = fcntl( chan->socket.handle, F_GETFL, 0 );
   fcntl( chan->socket.handle, F_SETFL, flags | O_NONBLOCK );
#endif

   struct sockaddr_in sockaddr;
   sockaddr.sin_family = AF_INET;
   sockaddr.sin_addr.S_un.S_addr = addr.base;
   sockaddr.sin_port = addr.port;

   err = bind( chan->socket.handle, (struct sockaddr*) &sockaddr, sizeof(sockaddr) );
   if( err == -1 )
   {
      Cmd_Errorf( "failed to bind channel: %i\n", err );
   }
}

void Chan_CloseChannel( channel_t* chan )
{
   if( chan->socket.handle )
      closesocket( chan->socket.handle );
   WIPEPTR( chan );
}

void Chan_Receive( channel_t* chan )
{
   int err;
   int fromlen = sizeof(struct sockaddr_in);

   chan->inavail = FALSE;
   err = recvfrom( chan->socket.handle, (char*) chan->incoming, MAX_CHANNEL_BUF_SIZE, 0,
      (struct sockaddr*) &chan->inaddr, &fromlen );
   if( err == -1 )
   {
      err = NETERR;
      if( err == EWOULDBLOCK )
         return;
      Cmd_Errorf( "chan failed to receive! (%i) \n", err );
   }

   chan->inavail = TRUE;
}

void Chan_Send( channel_t* chan )
{
   struct sockaddr_in addr;
   int err;

   addr.sin_family = AF_INET;
   addr.sin_addr.S_un.S_addr = chan->dest.base;
   addr.sin_port = chan->dest.port;

   err = sendto( chan->socket.handle, (char*) chan->outcoming, MAX_CHANNEL_BUF_SIZE, 0,
      (struct sockaddr*) &addr, sizeof(addr) );
   if( err == -1 )
   {
      err = NETERR;
      if( err == EWOULDBLOCK )
         return;
      Cmd_Errorf( "chan failed to send! (%i) \n", err );
   }
}
