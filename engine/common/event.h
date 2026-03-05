#ifndef COMMON_EVENT_H
#define COMMON_EVENT_H

#include <common/define.h>

#define MAX_EVENT_STACK 1024

typedef enum event_kind_e
{
   EVENT_DUMMY    = 0x0,
   EVENT_QUIT     = 0x1,
   EVENT_COMMAND  = 0x2
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
         byte _;
      } command;
   }
   data;
}
event_t;

extern event_t event_stack[MAX_EVENT_STACK];

void  Event_Push( event_t* ev );
void  Event_PopKind( event_kind_t kind, event_t* ev );

#endif
