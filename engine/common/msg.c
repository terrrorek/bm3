
#include <common/cmd.h>
#include <common/msg.h>
#include <game/eobj.h>

uint Msg_ConstructEntChunks( msg_entchunk_t** dest )
{
   uint amount = 1 + ( Ent_Count( ) / MAX_ENTCHUNK_ENTS );
   msg_entchunk_t* chunks = (msg_entchunk_t*) malloc( sizeof(msg_entchunk_t) * amount );
   memset( chunks, 0, sizeof(msg_entchunk_t) * amount );

   int i = 0, j = 0, ch = 0;
   do
   {
      if( j - 1 == MAX_ENTCHUNK_ENTS )
      {
         j = 0;
         ++ch;
      }
      msg_entchunk_t* chunk = &chunks[ch];

      if( eobjs[j].edecl )
      {
         int c = 0;
         edecl_t* decl = NULL;
         for( ; c < chunk->declCount; c++ )
         {
            if( !strcmp( 
               chunk->decls[c].name, eobjs[j].edecl->name ) )
               decl = (edecl_t*) c;
         }

         if( !decl )
            chunk->decls[chunk->declCount++] = edecls[i];
         
         chunk->objs[chunk->objCount] = eobjs[i];
         chunk->objs[chunk->objCount++].edecl = decl;

         ++j;
      }

      ++i;
   }
   while( i < MAX_EDECL );

   i = 0, j = 0;
   do
   {
      for( j = 0; j < chunks[i].declCount; j++ )
      {
         edecl_t* decl = &chunks[i].decls[j];
         Cmd_Printf( "chunk %i: decl %s at %i\n", i, decl->name, j ); 
      }
      for( j = 0; j < chunks[i].objCount; j++ )
      {
         eobj_t* obj = &chunks[i].objs[j];
         edecl_t* decl = &chunks[i].decls[(int)obj->edecl];
         Cmd_Printf( "chunk %i: obj %s (decl %s) at %i\n", i,
            obj->data.name, decl->name, j );
      }

      ++i;
   }
   while( i < (int) amount );

   *dest = chunks;
   return amount;
}

