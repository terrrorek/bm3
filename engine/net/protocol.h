#ifndef COMMON_PROTOCOL_H
#define COMMON_PROTOCOL_H
#include <common/define.h>
#include <net/msg.h>
#include <game/game.h>
#include <common/cmd.h>

/* cts - client to server, stc - server to client */
typedef enum protocol_msgtype_e
{
   PROTO_ANY_PING = 0,

   PROTO_CTS_CONNECT = 1,
   PROTO_CTS_DISCONNECT,

   PROTO_STC_CONNECT = 1001,
   PROTO_STC_DISCONNECT,
   PROTO_STC_ANNOUNCE,
}
protocol_msgtype_t;

/* protocol message
 *    2 byte      - msgtype
 *    2 byte      - pid
 *    n byte(s)   - msgdata
 */

/*
 * PROTO_CTS_CONNECT (oob)
 *    name     - string
 * PROTO_CTS_DISCONNECT
 *    reason   - string
 *
 * PROTO_STC_CONNECT
 *    pid      - int
 * PROTO_STC_DISCONNECT
 *    pid      - int
 *    reason   - string
 */

#endif
