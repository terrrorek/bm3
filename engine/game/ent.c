#include <common/cmd.h>
#include <game/ent.h>
#include <game/game.h>

econ_t   game_entities[ENT_MAX_ENTITIES] = {0}; /* simulation entities (server, sim, etc...) */
econ_t   cl_entities[ENT_MAX_ENTITIES] = {0}; /* client entities (drawing, lerp, etc...) */
uint32_t farthest_entity = 0;

typedef eclass_t* (*geteclassfunc_t)(void);
static bool Ent_AcquireClass( const char* classname, eclass_t* eclass )
{
   char  buffer[128];
   void* sym;

   snprintf( buffer, sizeof(buffer), "__%s_eclass", classname );
   sym = Game_GetSymbol( buffer );
   if( !sym )
      return false;

   *eclass = *((geteclassfunc_t) sym)();
   return true;
}

econ_t* Ent_Allocate( const char* classname )
{
   uint  i;

   for( i = 0; i < ENT_MAX_ENTITIES; i++ )
   {
      econ_t*  ent = &game_entities[i];
      if( ent->classname[0] )
         continue;

      strncpy( ent->classname, classname, 32 );
      if(!( Ent_AcquireClass( ent->classname, &ent->eclass ) ))
      {
         Cmd_SoftErrorf( "failed to acquire eclass for classname \"%s\"\n", ent->classname );
         WIPE( *ent );
         return NULL;
      }

      if( (ent - game_entities) > farthest_entity )
         farthest_entity = (ent - game_entities);
      return ent;
   }

   return NULL;
}

void Ent_Free( econ_t* ent )
{
   uint32_t i;

   if( (ent - game_entities) == farthest_entity )
      for( i = ent - game_entities; i >= 0; i-- )
      {
         if( game_entities[i].classname[0] )
         {
            farthest_entity = i;
            break;
         }
      }
   WIPE( *ent );
}


void Ent_Spawn( econ_t* ent )
{
   if( !ent->eclass.spawn )
   {
      Cmd_SoftErrorf( "entity %u (%s) doesn't have a spawn function!\n",
         ent - game_entities, ent->classname );
      return;
   }

   ent->eclass.spawn( ent );
   ent->frozen = false;
}
void Ent_Update( econ_t* ent )
{
   if( !ent->eclass.update )
   {
      Cmd_SoftErrorf( "entity %u (%s) doesn't have a update function!\n",
         ent - game_entities, ent->classname );
      return;
   }

   ent->eclass.update( ent );
}
void Ent_End( econ_t* ent )
{
   if( !ent->eclass.end )
      return; /* not required */

   ent->eclass.end( ent );
}
