#ifndef COMMON_EVENT_H
#define COMMON_EVENT_H

#include <common/define.h>
#include <net/net.h>

#define MAX_EVENT_STACK 1024

typedef enum event_kind_e
{
   EVENT_DUMMY       = 0x0,
   EVENT_QUIT        = 0x1,
   EVENT_CL_PACKET   = 0x2,
   EVENT_SV_PACKET   = 0x3,
}
event_kind_t;
typedef struct event_s
{
   event_kind_t   kind;
   union
   {
      struct
      {
         char* reason;
      } quit;
      struct
      {
         netsock_t   sock;
         netaddr_t   addr;
         byte        buffer[768];
      } packet;
   }
   data;
}
event_t;

extern event_t event_stack[MAX_EVENT_STACK];

void  Event_Push( event_t* ev );
void  Event_PopKind( event_kind_t kind, event_t* ev );

#endif
