#include <engine/game/ent.h>
#include <common/dll.h>

static void player_spawn( econ_t* e )
{
}
static void player_update( econ_t* e )
{
}
static void player_end( econ_t* e )
{
}

static eclass_t player_eclass = {
   player_spawn, player_update, player_end
};
EXPORT_ECLASS( player, player_eclass )
