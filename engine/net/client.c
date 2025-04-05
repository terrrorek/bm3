#include <net/net.h>
#include <net/msg.h>
#include <net/pack.h>
#include <common/cmd.h>

static void Net_WaitForAcceptance( socket_t* sock )
{
   if( Net_GetPendingType(sock) == 1 )
   {
      msg_t msg;
      Net_RecvMessage( sock, &msg );
      Cmd_Printf( "%i\n", msg.type );
   }
}

void Net_ClientLoop( socket_t* sock )
{
   if(!( sock->flags & SOCK_ACCEPTED ))
   {
      Net_WaitForAcceptance( sock );
      return;
   }

}
