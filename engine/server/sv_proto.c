#include <server/server.h>
#include <net/msg.h>
#include <net/protocol.h>

void SV_DataPacketUpdate( byte* buf, netaddr_t* from )
{
   msgbuf_t             msgbuf;
   protocol_msgtype_t   msgtype;
   uint                 pid;

   MSG_Init( &msgbuf, buf, NET_PACKET_SIZE );
   msgtype = MSG_ReadShort( &msgbuf );
   pid = MSG_ReadShort( &msgbuf );
}

void SV_MsgPacketUpdate( byte* buf, netaddr_t* from )
{
   msgbuf_t             msgbuf;
   protocol_msgtype_t   msgtype;
   uint                 pid;

   MSG_Init( &msgbuf, buf, NET_PACKET_SIZE );
   msgtype = MSG_ReadShort( &msgbuf );
   pid = MSG_ReadShort( &msgbuf );
}

void SV_PacketFrame( void )
{

}
