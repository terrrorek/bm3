#include <common/common.h>
#include <game/eobj.h>
#include <game/state.h>
#include <common/alloc.h>
#include <game/helpers.h>

CLASS( template,
   // custom class defs here
)

CREATE( template,
   // custom create code here
)

SPAWN( template )
{
   (void) this;
}
UPDATE( template )
{
   (void) this, (void) client;
}
KILL( template )
{
   (void) this;
}
