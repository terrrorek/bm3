#include <game/state.h>
#include <game/eobj.h>
#include <game/edecl.h>
#include <game/dll.h>
#include <common/bmstring.h>
#include <common/cmd.h>
#undef bool

static int edeclCount = 0;
edecl_t edecls[MAX_EDECL] = {0};

static int eobjCount = 0;
eobj_t eobjs[MAX_EOBJ] = {0};

extern uint64_t secondDur;

cvar_t   r_interp = {
   .name = "r_interp",
   .flags = CVAR_NUMBER,
   .number = TRUE,
};

// static

static eobj_t* Ent_FindEmptyEnt( void )
{
   int i = 0;
   do
   {
      if( !eobjs[i].edecl )
         return &eobjs[i];
      ++i;
   }
   while( i < MAX_EOBJ );
   return NULL;
}

static void Ent_FindDeclFuncs( edecl_t* edecl )
{
   char nameBuf[96];

   BM_snprintf( nameBuf, 96, "%s_create", edecl->name );
   edecl->create = (newentfunc_t) Game_GetFuncHandle( nameBuf );
   BM_snprintf( nameBuf, 96, "%s_spawn", edecl->name );
   edecl->spawn = (entfunc_t) Game_GetFuncHandle( nameBuf );
   BM_snprintf( nameBuf, 96, "%s_update", edecl->name );
   edecl->update = (updateentfunc_t) Game_GetFuncHandle( nameBuf );
   BM_snprintf( nameBuf, 96, "%s_kill", edecl->name );
   edecl->kill = (entfunc_t) Game_GetFuncHandle( nameBuf );

   if( !edecl->create || !edecl->spawn ||
       !edecl->update || !edecl->kill )
   {
      Cmd_Errorf( "invalid edecl \"%s\" (not all funcs present)\n", edecl->name );
   }
}

static bool Ent_DeclExists( const char* name )
{
   char nameBuf[96];
   BM_snprintf( nameBuf, sizeof(nameBuf), "%s_create", name );
   return Game_GetFuncHandle( nameBuf ) ? TRUE : FALSE;
}

static bool Ent_IsDeclared( const char* name )
{
   return Ent_GetDecl( name ) ? TRUE : FALSE;
}

// edecl

edecl_t* Ent_DeclEnt( const char* name )
{
   if( edeclCount >= MAX_EDECL )
   {
      Cmd_Errorf( "reached max edecls\n" );
   }

   if( !Ent_DeclExists( name ) )
   {
      Cmd_SoftErrorf( "failed to make edecl \"%s\" (not found)\n", name );
      return NULL;
   }

   edecl_t* edecl = &edecls[edeclCount++];
   WIPEPTR( edecl );
   BM_strncpy( edecl->name, name, sizeof(edecl->name) );
   Ent_FindDeclFuncs( edecl );

   return edecl;
}

edecl_t* Ent_GetDecl( const char* name )
{
   int i = 0;
   do
   {
      int r = BM_strcmp( name, edecls[i].name );
      if( !r && (r ^ INT32_MAX) )
      {
         return &edecls[i];
      }
      ++i;
   }
   while( i < MAX_EDECL );
   return NULL;
}

// eobj

eobj_t* Ent_Create( const char* classname )
{
   eobj_t* eobj = Ent_FindEmptyEnt( );
   if( !eobj )
   {
      Cmd_Errorf( "reached max eobjs\n" );
   }

   WIPEPTR( eobj );

   if( !Ent_IsDeclared( classname ) && Ent_DeclExists( classname ) )
   {
      Ent_DeclEnt( classname );
   }

   eobj->edecl = Ent_GetDecl( classname );
   if( !eobj->edecl )
   {
      Cmd_Errorf( "ent creation failed (edecl \"%s\" not found)\n", classname );
   }

   eobj->index = eobjCount - 1;
   eobj->edecl->create( &eobj->clazz, (void*) eobj );

   return eobj;
}

void Ent_Update( eobj_t* ent, bool client )
{
   if( !ent )
      return;
   if( !ent->edecl )
      return;

   if( ent->chances > MAX_EOBJ_CHANCES )
      Ent_Destroy( ent );

   if( !ent->clazz )
   {
      Cmd_SoftErrorf( "failed to update ent \"%s\" (clazz is NULL)\n", ent->data.name );
      ++ent->chances;
      return;
   }

   if( !ent->data.spawned )
   {
      ent->edecl->spawn( ent->clazz );
      ent->data.spawned = TRUE;
      return;
   }


   ent->interp = 0;
   memcpy( &ent->lastdata, &ent->data, sizeof(edata_t) );
   ent->edecl->update( ent->clazz, client );
}

bool Ent_Destroy( eobj_t* ent )
{
   if( !ent )
      return false;

   if( ent->clazz && ent->edecl )
      ent->edecl->kill( ent->clazz );

   WIPEPTR( ent );
   return true;
}

void Ent_DestroyAll( void )
{
   uint i = 0;
   do
   {
      Ent_Destroy( &eobjs[i] );
      ++i;
   }
   while( i < MAX_EOBJ );
}

void Ent_Process( bool client )
{
   int i = 0;
   do
   {
      eobj_t* ent = &eobjs[i];
      Ent_Update( ent, client );
      ++i;
   }
   while( i < MAX_EOBJ );
}

uint Ent_Count( void )
{
   uint i = 0, count = 0;

   do
   {
      if( eobjs[i].edecl )
         ++count;
      ++i;
   }
   while( i < MAX_EOBJ );
   return count;
}

extern double nowTime, lastUpdate;
extern cvar_t eng_rate;

static void Ent_InterpolateTransform( eobj_t* ent, vec3* pos, vec3* rot, vec3* scale )
{
   if( ent->interp > 1.0 )
      return;

   glm_vec3_lerp(
      ent->lastdata.transform.position,
      ent->data.transform.position,
      ent->interp, *pos );

   glm_vec3_lerp(
      ent->lastdata.transform.rotation,
      ent->data.transform.rotation,
      ent->interp, *rot );

   glm_vec3_lerp(
      ent->lastdata.transform.scale,
      ent->data.transform.scale,
      ent->interp, *scale );

   ent->interp = (nowTime - lastUpdate) * eng_rate.number;
}

void Ent_Draw( void )
{
   uint     i = 0;
   eobj_t*  obj;

   if(!( r_interp.flags & CVAR_REGISTERED ))
      CVar_Register( &r_interp );

   do
   {
      obj = &eobjs[i];
      if( !obj->data.spawned )
      {
         ++i;
         continue;
      }

      vec3  pos, rot, scale;

      if( !r_interp.number )
         obj->interp = 1.f;

      Ent_InterpolateTransform( obj, &pos, &rot, &scale );

      // TEMP: drawing flat sprites
      if( obj->edecl && obj->mat.pic )
      {
         rendImports.Rend_DrawPic( obj->mat.pic,
            pos[0], pos[1], scale[0], scale[1] );
      }
      ++i;
   }
   while( i < MAX_EOBJ );
}
