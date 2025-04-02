
#include "soft.h"
#include <common/bmstring.h>

softtex_t   textures[MAX_SOFTTEXTURES];

static softtex_t* Rend_AllocTexture( void )
{
   softtex_t*  tex;
   int         i;

   for( i = 0; i < MAX_SOFTTEXTURES; i++ )
   {
      tex = &textures[i];
      if( !tex->pic )
         return tex;
   }

   engFuncs->Cmd_Errorf( "failed to allocate software texture.\n" );
   return NULL;
}

static softtex_t* Rend_GetTexture( uint index )
{
   if( index > MAX_SOFTTEXTURES )
      engFuncs->Cmd_Errorf( "tried getting invalid texture.\n" );

   return &textures[index];
}

static softtex_t* Rend_FindTexture( const char* name )
{
   softtex_t*  tex;
   int         i;

   for( i = 0; i < MAX_SOFTTEXTURES; i++ )
   {
      tex = &textures[i];
      if( !BM_strcmp(tex->name, name) )
         return tex;
   }
   return NULL;
}

static void Rend_GetCharOffset( softtex_t* tex, char x, int* chx, int* chy )
{
   int chw;
   int mch;

   int tx = 0, ty = 0, tty = 0;

   chw = floor((float)tex->width / (float)(tex->pic->font.width + tex->pic->font.margin));
   // chh = floor((float)tex->height / (float)(tex->pic->font.height + tex->pic->font.margin));

   // map char
   if( x >= 'a' && x <= 'z' )
      mch = x - 'a';
   else if( x >= 'A' && x <= 'Z' )
      mch = x - 'A';
   else if( x >= '0' && x <= '9' )
      mch = (x - '0') + 26;
   else if( x >= '!' && x <= '/' )
      mch = (x - '!') + 36;
   else if( x >= ':' && x <= '@' )
      mch = (x - ':') + 51;
   else if( x >= '[' && x <= '`' )
      mch = (x - '[') + 58;
   else if( x >= '{' && x <= '~' )
      mch = (x - '{') + 64;
   else
   {
      *chx = INT32_MAX;
      *chy = INT32_MAX;
      return;
   }

   tty = floor((float)mch / (float)chw);
   ty = (tty * tex->pic->font.height) + (tty * tex->pic->font.margin);
   tx = (mch * tex->pic->font.width) + (mch * tex->pic->font.margin) -
      (((tex->pic->font.width * chw) + (tex->pic->font.margin * chw)) * tty);
   tx = tx < 0 ? 0 : tx;

   // engFuncs->Cmd_Printf( "%i %i %i\n", tx, ty, chw );
   // engFuncs->Cmd_Printf( "%i %i\n", chw, tty );
   // engFuncs->Cmd_Printf( "%i %i %i\n", tex->pic->font.width, tex->pic->font.height, tex->pic->font.margin );
   // engFuncs->Cmd_Printf( "%i %i\n", tx, ty );

   *chx = tx;
   *chy = ty;
}

static void Rend_DrawPicImpl( pic_t* pic, int x, int y, int w, int h,
   int ofx, int ofy, int scx, int scy )
{
   int      nx, ny;
   rgba_t   col;

   for( ny = 0; ny < h; ny++ )
      for( nx = 0; nx < w; nx++ )
      {
         memcpy( col, pic->buffer[
            RAWPOS(
               ((nx + (ofx * w / scx)) * scx / w),
               ((ny + (ofy * h / scy)) * scy / h), pic->width)], sizeof(col) );

         if( col[3] <= 1 )
            continue;

         Rend_SetBuffer( x + nx, y + ny, col );
      }
}

uint Rend_CreateTexture( char* name, pic_t* pic )
{
   softtex_t*  stex;

   if(( stex = Rend_FindTexture(name) ))
   {
      if( pic->buffer )
         engFuncs->Mem_Free( pic->buffer );
      engFuncs->Mem_Free( pic );
      return stex->index;
   }

   stex = Rend_AllocTexture( );
   stex->pic = pic;
   stex->width = pic->width;
   stex->height = pic->height;
   BM_strncpy( stex->name, name, sizeof(stex->name ) );
   stex->index = stex - textures;

   return stex->index;
}

void Rend_DestroyTexture( uint index )
{
   softtex_t*  stex;

   if( index > MAX_SOFTTEXTURES )
      engFuncs->Cmd_Errorf( "tried destroying invalid texture\n" );

   stex = &textures[index];
   if( stex->pic )
   {
      if( stex->pic->buffer )
         engFuncs->Mem_Free( stex->pic->buffer );
      engFuncs->Mem_Free( stex->pic );
      stex->pic = NULL;
   }

   WIPEPTR( stex );
}

void Rend_DrawChar( uint pic, char ch, int x, int y, int w, int h )
{
   softtex_t*  tex;
   int         chx, chy;

   tex = Rend_GetTexture( pic );
   if( !tex )
      return;
   if( !tex->pic )
      return;
   if( !(tex->pic->flags & PIC_FONT) )
      return;

   Rend_GetCharOffset( tex, ch, &chx, &chy );
   if( chx > tex->pic->width || chy > tex->pic->height )
      return;

   Rend_DrawPicImpl( tex->pic, x, y, w, h, chx, chy, tex->pic->font.width, tex->pic->font.height );
}

void Rend_DrawString( uint pic, int x, int y, int w, int h, const char* fmt, ... )
{
   int         cx, cy, length;
   char*       buffer, * ptr;
   va_list     args;
   int         chx, chy;
   softtex_t*  tex;

   tex = Rend_GetTexture( pic );
   if( !tex )
      return;
   if( !tex->pic )
      return;
   if( !(tex->pic->flags & PIC_FONT) )
      return;

   va_start( args, fmt );
   length = vsnprintf( NULL, 0, fmt, args );
   if( length <= 0 )
   {
      va_end( args );
      return;
   }

   buffer = engFuncs->Mem_Alloc( length + 1 );
   vsnprintf( buffer, length + 1, fmt, args );

   va_end( args );

   for( ptr = buffer, cx = 0, cy = 0; *ptr; cx += w + 1, ptr++ )
   {
      if( x + cx > state->width )
      {
         cy += h + 1;
         cx = 0;
      }

      Rend_GetCharOffset( tex, *ptr, &chx, &chy );
      if( chx > tex->pic->width || chy > tex->pic->height )
         continue;

      Rend_DrawPicImpl( tex->pic, x + cx, y + cy, w, h, chx, chy, tex->pic->font.width, tex->pic->font.height );
   }
}

void Rend_DrawPic( uint pic, int x, int y, int w, int h )
{
   softtex_t*  tex;

   tex = Rend_GetTexture( pic );
   if( !tex )
      return;
   if( !tex->pic )
      return;

   Rend_DrawPicImpl( tex->pic, x, y, w, h, 0, 0, tex->pic->width, tex->pic->height );
}
