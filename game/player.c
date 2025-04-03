#include <game/eobj.h>
#include <common/alloc.h>
#include <game/helpers.h>

PHYSCLASS( player,
)

CREATE( player,
)

SPAWN( player )
{
   (void) this;
}
UPDATE( player )
{
   (void) this, (void) client;
}
KILL( player )
{
   (void) this;
}
