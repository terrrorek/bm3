
#include "gl.h"
#include <common/bmstring.h>

gltexture_t    textures[MAX_GLTEXTURES];

static void    Rend_UploadTexture( gltexture_t* tex, pic_t* pic )
{
   tex->width = pic->width;
   tex->height = pic->height;

   glGenTextures( 1, &tex->handle );

   glBindTexture( GL_TEXTURE_2D, tex->handle );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
   glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8,
      pic->width, pic->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pic->buffer );
   glBindTexture( GL_TEXTURE_2D, 0 );
}

static gltexture_t*  Rend_GetTexture( uint index )
{
   if( index > MAX_GLTEXTURES )
      engFuncs->Cmd_Errorf( "tried getting invalid texture.\n" );

   return &textures[index];
}

gltexture_t*   Rend_FindTexture( const char* name )
{
   int            i = 0;
   gltexture_t*   gltex = &textures[i];

   while( i < MAX_GLTEXTURES )
   {
      if(!BM_strcmp( name, gltex->name ))
         return gltex;
      gltex = &textures[++i];
   }

   return NULL;
}

gltexture_t*   Rend_AllocTexture( void )
{
   int            i = 0;
   gltexture_t*   gltex = &textures[i];

   while( i < MAX_GLTEXTURES )
   {
      if( !gltex->handle )
         return gltex;
      gltex = &textures[++i];
   }

   engFuncs->Cmd_Errorf( "failed to allocate gl texture.\n" );
   return NULL;
}

uint Rend_CreateTexture( char* name, pic_t* pic )
{
   gltexture_t*   gltex;

   if(( gltex = Rend_FindTexture(name) ))
      return gltex->index;

   gltex = Rend_AllocTexture( );
   BM_strncpy( gltex->name, name, sizeof(gltex->name) );
   gltex->index = gltex - textures;
   Rend_UploadTexture( gltex, pic );
   gltex->pic = pic;

   return gltex->index;
}

void Rend_DestroyTexture( uint index )
{
   gltexture_t*   gltex;

   if( index > MAX_GLTEXTURES )
      engFuncs->Cmd_Errorf( "tried destroying invalid texture.\n" );

   gltex = &textures[index];
   if( gltex->pic )
   {
      if( gltex->pic->buffer )
         engFuncs->Mem_Free( gltex->pic->buffer );
      engFuncs->Mem_Free( gltex->pic );
   }

   glDeleteTextures( 1, &gltex->handle );

   WIPEPTR( gltex );
}

static void Rend_GetCharOffset( gltexture_t* tex, char x, int* chx, int* chy )
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

   *chx = tx;
   *chy = ty;
}

static void Rend_DrawPicImpl( gltexture_t* gltex, int x, int y, int w, int h,
   int ofx, int ofy, int scx, int scy )
{
   vec3           pos, scale;
   float          xpix, ypix;

   xpix = 1.f / (float)state->width;
   ypix = 1.f / (float)state->height;

   pos[0] = (x * xpix * 2);
   pos[1] = -(y * ypix * 2);
   pos[2] = 0.f;

   scale[0] = (float)w / state->width;
   scale[1] = -((float)h / state->height);
   scale[2] = 1.f;

   memcpy( u_raw_wh, (ivec2){gltex->width, gltex->height}, sizeof(u_raw_wh) );
   memcpy( u_xywh, (ivec4){x, y, w, h}, sizeof(u_xywh) );
   memcpy( u_tex_xywh, (ivec4){ofx, ofy, scx, scy}, sizeof(u_tex_xywh) );

   glBindVertexArray( quadArray );
   glBindTexture( GL_TEXTURE_2D, gltex->handle );
   glUseProgram( picShader );
   Rend_2DProjection( );
   Rend_ModelMatrix( pos, (vec4){0,0,0}, scale );
   Rend_UpdateUniforms( picShader );
   glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL );
   glBindVertexArray( 0 );
   glUseProgram( 0 );
   glBindTexture( GL_TEXTURE_2D, 0 );
}

void Rend_DrawChar(uint pic, char ch, int x, int y, int w, int h)
{
   gltexture_t*   tex;
   int            chx, chy;

   tex = Rend_GetTexture( pic );
   if( !tex )
      return;
   if( !(tex->pic->flags & PIC_FONT) )
      return;

   Rend_GetCharOffset( tex, ch, &chx, &chy );

   if( chx > tex->pic->width || chy > tex->pic->height )
      return;

   Rend_DrawPicImpl( tex, x, y, w, h, chx, chy, tex->pic->font.width, tex->pic->font.height );
}

void Rend_DrawPic( uint pic, int x, int y, int w, int h )
{
   gltexture_t*   tex;

   tex = Rend_GetTexture( pic );
   if( !tex )
      return;

   Rend_DrawPicImpl( tex, x, y, w, h, 0, 0, w, h );
}
