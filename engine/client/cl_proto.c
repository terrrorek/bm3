#include <client/client.h>
#include <net/msg.h>
#include <net/protocol.h>

void CL_DataPacketUpdate( byte* buf, netaddr_t* from )
{
   msgbuf_t             msgbuf;
   protocol_msgtype_t   msgtype;
   uint                 pid;

   MSG_Init( &msgbuf, buf, NET_PACKET_SIZE );
}

void CL_MsgPacketUpdate( byte* buf, netaddr_t* from )
{
   msgbuf_t             msgbuf;
   protocol_msgtype_t   msgtype;
   uint                 pid;

   MSG_Init( &msgbuf, buf, NET_PACKET_SIZE );
}

void CL_PacketFrame( void )
{

}
