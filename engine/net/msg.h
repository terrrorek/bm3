
#ifndef MSG_H
#define MSG_H

#include <common/common.h>
#include <net/bitfield.h>

typedef enum msg_type_e
{
   // cls - client to server
   MSG_CLS_CONNECT = 1,
   MSG_CLS_DISCONNECT,

   // svc - server to client
   MSG_SVC_DISCONNECT,
}
msg_type_t;
typedef struct msg_header_s
{
   uint32_t    seq;
   uint32_t    ackseq;
   uint32_t    clid;
   uint16_t    type;
   uint8_t     ack;
   uint8_t     __pad;
}
msg_header_t;

void Msg_Construct(
   msg_header_t* header,
   bitbuffer_t* bitbuf,
   byte* buffer, size_t buflen
);

#endif
