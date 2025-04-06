
#include <common/alloc.h>
#include <game/console.h>
#include <common/cmd.h>
#include <client/input.h>
#include <video/rend.h>
#include <video/pic.h>
#include <common/cvar.h>
#include <common/bmstring.h>

bool     conOpened = FALSE;

uint     conBackground = 0, conFont = 0, colortest = 0;

static input_t conInput = {
   .name = "console",
   .mouse = false,
   .repeat = false,
   .keycode = SDLK_GRAVE,
   .whenCapture = true,
};

char     consoleBuffer[512][1024]; // 512 lines that contain 1024 chars
int      consoleShift = 0; // draw upper lines
int      consoleLine = 0; // current line

int      consoleInputPtr = 0;
char     consoleInput[1024];

int      consoleHeight = 0;
char     consoleChanging = 0; // -1 is down | 1 is up
float    consoleLerp = 0;

void Con_DumpCVars_f( void )
{
   cvar_t*  cv = lastCVar;

   while( cv )
   {
      Con_Printf( "CVAR %s\n", cv->name );
      Con_Printf( "   flags: %04x\n", cv->flags );
      cv = cv->prev;
   }
}

void Con_Exit_f( void )
{
   shouldExit = true;
}

void Con_Clear_f( void )
{
   int   i;

   for( i = 0; i < 512; i++ )
      memset( &consoleBuffer[i][0], 0, 1024 );

   consoleShift = 0;
   consoleLine = 0;
}

void Con_Execute( const char* buf )
{
   cvar_t*  cv;
   char*    sep;

   Con_Printf( "] %s\n", buf );
   sep = strchr( buf, ' ' );

   if( sep ) // var
   {
      *sep++ = '\0';
      cv = CVar_Get( buf );

      if( !cv )
      {
         Con_Printf( "ERROR: couldn't find cvar \"%s\"\n", buf );
         return;
      }

      if( cv->flags & CVAR_LOCKED )
      {
         Con_Printf( "can't set cvar \"%s\", because it's locked\n", cv->name );
         return;
      }

      if( cv->flags & CVAR_NORMAL )
         cv->normal = atof( sep );
      else if( cv->flags & CVAR_NUMBER )
         cv->number = atoi( sep );
      else if( cv->flags & CVAR_STRING )
         BM_strncpy( cv->string, sep, 200 );
      else if( cv->flags & CVAR_FUNCTION && cv->function )
      {
         cv->function( sep );
      }
      else
         Con_Printf( "unknown cvar type %02X for \"\"\n", cv->flags, cv->name );

      cv->changed = true;
   }
   else // func
   {
      cv = CVar_Get( buf );

      if( !cv )
      {
         Con_Printf( "ERROR: couldn't find func \"%s\"\n", buf );
         return;
      }
      if( !(cv->flags & CVAR_FUNCTION) )
      {
         // print cvar
         switch( cv->flags )
         {
         case CVAR_NORMAL:
            Con_Printf( "%s: %f\n", cv->name, cv->normal );
            break;
         case CVAR_NUMBER:
            Con_Printf( "%s: %i\n", cv->name, cv->number );
            break;
         case CVAR_STRING:
            Con_Printf( "%s: %s\n", cv->name, cv->string );
            break;
         default: break;
         }

         return;
      }

      if( cv->function )
         cv->function( NULL );
   }
}

void Con_Printf( const char* fmt, ... )
{
   int      length;
   char*    buf;
   va_list  args;

   va_start( args, fmt );
   length = vsnprintf( NULL, 0, fmt, args );
   if( length <= 0 )
   {
      va_end( args );
      return;
   }

   buf = Mem_Alloc( length + 1 );
   vsnprintf( buf, length + 1, fmt, args );

   va_end( args );

   if( consoleLine + 1 > 511 )
      Con_Clear_f( );
   else consoleLine++;

   memcpy( &consoleBuffer[consoleLine][0], buf, length + 1 );
   Cmd_Printf( "CONSOLE: %s", buf );
   Mem_Free( buf );
}

cvar_t  clear_f = {
   .name = "clear",
   .flags = CVAR_FUNCTION
   // .function = Con_Clear_f,
};
cvar_t  exit_f = {
   .name = "exit",
   .flags = CVAR_FUNCTION
   // .function = Con_Exit_f,
   // msvc doesn't like that
};
cvar_t   dumpcv_f = {
   .name = "dumpcv",
   .flags = CVAR_FUNCTION,
};

void Con_Init( void )
{
   clear_f.function = (confunc_t)Con_Clear_f;
   exit_f.function = (confunc_t)Con_Exit_f;
   dumpcv_f.function = (confunc_t)Con_DumpCVars_f;

   IN_RegisterInput( &conInput );
   conBackground = R_LoadTexture( "ui/conbg.tga", 0 );
   colortest = R_LoadTexture( "colortest.tga", 0 );
   conFont = R_LoadTexture( "ui/font.tga", PIC_FONT );
   memset( consoleInput, 0, sizeof(consoleInput) );
   consoleHeight = -(vidState.height / 2);

   CVar_Register( &clear_f );
   CVar_Register( &exit_f );
   CVar_Register( &dumpcv_f );
   Con_Printf( "console initialized.\n" );
}

void Con_Process( void )
{
   if( conInput._pressed )
   {
      conOpened = !conOpened;
      consoleLerp = 0;
      consoleChanging = 1;
   }

   if( !conOpened )
   {
      SDL_StopTextInput( vidState.window );
      captureInput = false;
      consoleChanging = -1;
      return;
   }
   SDL_StartTextInput( vidState.window );
   captureInput = true;

   if( lastKey >= ' ' && lastKey < '~' && lastKey != '`' )
   {
      if( consoleInputPtr >= 1023 )
         return;
      consoleInput[consoleInputPtr++] = lastKey;
   }
   else
   {
      switch( lastKey )
      {
      default:
         break;

      case SDLK_BACKSPACE:
         if( consoleInputPtr > 0 )
            consoleInput[--consoleInputPtr] = 0;
         break;

      case SDLK_RETURN:
         if( strnlen( consoleInput, 1024) >= 1 )
            Con_Execute( consoleInput );
         memset( consoleInput, 0, sizeof(consoleInput) );
         consoleInputPtr = 0;
         break;
      }
   }

   lastKey = 0;
}

void Con_Draw( void )
{
   int   x, y, i, b;
   int   lines, linelen, chars;
   char* buf;
   int   conh;
   bool  lerped;

   conh = vidState.height / 2;

   if( consoleChanging == 1 && consoleHeight != 0 )
   {
      consoleLerp += 2 * deltaTime;
      consoleHeight = BM_lerp(-conh - 24, 0, consoleLerp);
   }
   else if( consoleChanging == -1 && consoleHeight > -conh )
   {
      consoleLerp += 2 * deltaTime;
      consoleHeight = BM_lerp(0, -conh - 24, consoleLerp);
   }

   consoleHeight = BM_clamp(consoleHeight, -conh - 24, 0);

   // finished lerping
   lerped = (consoleHeight == -conh - 24 || consoleHeight == 0);

   if( lerped || consoleLerp >= 1.0 )
   {
      consoleChanging = 0;
      consoleLerp = 0;
      if( !conOpened )
         return;
   }

   rendImports.Rend_DrawPic( conBackground, 0, consoleHeight, vidState.width, conh );

   lines = (vidState.height / 25) - 4;
   linelen = (vidState.width / 10) - 1;

   chars = lines * linelen;
   b = 0;
   buf = &consoleBuffer[b + consoleShift][0];

   for( i = 0, x = 11, y = 1; i < chars; i++, x += 9 )
   {
      if( x + 9 > vidState.width )
      {
         x = 1;
         y += 11;
      }

      if( *buf == 0 || *buf == '\n' || buf > &consoleBuffer[b + consoleShift][1023] )
      {
         if( b+1 + consoleShift < 511 )
         {
            buf = &consoleBuffer[++b + consoleShift][0];
            x = 1;
            y += 11;
         }
         continue;
      }

      if( y < (vidState.height / 2) - 24 )
      {
         rendImports.Rend_DrawChar( conFont, *buf++, x, y + consoleHeight, 7, 9 );
         // rendImports.Rend_DrawString( conFont, vidState.width - 96, y, 8, 12, "%i", b );
      }
   }

   rendImports.Rend_DrawChar( conFont, ']', 1, ((vidState.height / 2) - 14) + consoleHeight, 7, 9 );
   for( i = 0, y = (vidState.height / 2) - 14; i < linelen; i++ )
      rendImports.Rend_DrawChar( conFont, consoleInput[i], 22 + i * 9, y + consoleHeight, 7, 9 );

   if( consoleBuffer[b][0] != 0 )
      consoleShift++;
}
