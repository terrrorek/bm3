#include <common/event.h>
#include <common/cmd.h>

event_t event_stack[MAX_EVENT_STACK] = {0};

static event_t* Event_Allocate( void )
{
   uint  i;

   for( i = 0; i < MAX_EVENT_STACK; i++ )
   {
      if( event_stack[i].kind == EVENT_DUMMY )
         return &event_stack[i];
   }

   return NULL;
}

void Event_Push( event_t* ev )
{
   event_t* aev = Event_Allocate();
   if( !aev )
   {
      Cmd_Printf( "can't push - event journal full\n" );
      return;
   }
   *aev = *ev;
}

void Event_PopKind( event_kind_t kind, event_t* ev )
{
   int i;

   for( i = MAX_EVENT_STACK-1; i >= 0; i-- )
   {
      if( event_stack[i].kind == kind )
      {
         *ev = event_stack[i];
         WIPE( event_stack[i] );
         return;
      }
   }

   WIPE( *ev );
   return;
}
