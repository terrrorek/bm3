#include <common/cmd.h>
#include <game/ent.h>
#include <game/game.h>

econ_t game_entities[ENT_MAX_ENTITIES] = {0};

typedef eclass_t* (*geteclassfunc_t)(void);
static bool Ent_AcquireClass( const char* classname, eclass_t* eclass )
{
   char  buffer[128];
   void* sym;

   snprintf( buffer, sizeof(buffer), "__%s_eclass\n", classname );
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
      return ent;
   }

   return NULL;
}

void Ent_Free( econ_t* ent )
{
   WIPE( *ent );
}
