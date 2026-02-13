#include <net/net.h>
#include <game/console.h>
#include <net/bitfield.h>
#include <net/msg.h>
#include <game/state.h>

addr_t   net_serveraddress = {0};

int      CNet_Connect( addr_t* addr )
{
   int tries = 0, result;
   msg_t msg = {
      .type = MSGT_CONNECT,
      .hash = Net_GetUserHash(),
      .data = {0}
   };
   bitbuffer_t buffer = {
      .buffer = msg.data,
      .offset = 0,
      .size = MAX_MSG_SIZE
   };

   net_serveraddress = *addr;
   connect( net_client.handle, (const struct sockaddr*) addr, sizeof(addr_t) );
   Bit_WriteString( &buffer, playername );

   while( tries < 5 )
   {
      msg_t       recvmsg = {0};
      bitbuffer_t buffer = {.buffer = recvmsg.data, .size = MAX_MSG_SIZE};
      addr_t      address;

      result = Net_Send( &net_client, &msg, sizeof(msg), &net_serveraddress );
      WINERR;
      if( result != 0 )
      {
         Con_Printf( "failed to connect: send failure (%i)\n", result );
         return FALSE;
      }
      Sleep( 300 );

      result = Net_Receive( &net_client, &recvmsg, sizeof(recvmsg), &address );
      WINERR;
      if( result != 0 && result != EWOULDBLOCK )
      {
         Con_Printf( "failed to connect: recv failure (%i)\n", result );
         WIPEOUT( net_serveraddress );
         return FALSE;
      }
      if( result == EWOULDBLOCK )
         goto next;

      if( recvmsg.type == MSGT_DISCONNECT )
      {
         char* reason = Bit_ReadString( &buffer );
         Con_Printf( "failed to connect: server declined \"%s\"\n", reason );
         WIPEOUT( net_serveraddress );
         return FALSE;
      }
      else if( recvmsg.type == MSGT_CONNECT )
      {
         net_client.bound = TRUE;
         break;
      }

   next:
      Sleep( 300 );
      ++tries;
   }

   if( !net_client.bound )
   {
      Con_Printf( "failed to connect: max tries reached.\n" );
      WIPEOUT( net_serveraddress );
      return FALSE;
   }

   return TRUE;
}

static void CNet_ProcessDisconnect( msg_t* msg )
{
   bitbuffer_t buffer = Bit_FromMessage(msg);
   Con_Printf( "disconnecting. server requested disconnect: \"%s\"\n", Bit_ReadString( &buffer ));
   Game_Exit();
}

void     CNet_Update( void )
{
   msg_t msg;
   int result;
   addr_t address;
   bitbuffer_t buffer = Bit_FromMessage(&msg);

   if( !net_client.bound )
      return;

   result = Net_Receive( &net_client, &msg, sizeof(msg), &address );
   WINERR;
   if( result != 0 && result != EWOULDBLOCK )
   {
      Con_Printf( "error receiving server message (%i)\n", result );
      Game_Exit();
      return;
   }

   switch( msg.type )
   {
   case MSGT_DISCONNECT:
      CNet_ProcessDisconnect( &msg );
      break;
   default:
      break;
   }
}

void     CNet_Disconnect( void )
{
   msg_t msg = {
      .type = MSGT_DISCONNECT,
      .hash = Net_GetUserHash(),
   };
   bitbuffer_t buffer = Bit_FromMessage(&msg);
   Bit_WriteString( &buffer, "user requested" );
   Net_Send( &net_client, &msg, sizeof(msg), &net_serveraddress );
   Sleep( 50 );

   WIPEOUT( net_serveraddress );
   if( net_client.handle )
      Net_Reset( &net_client );
}
