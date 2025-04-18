#include <common/common.h>
#include <common/args.h>
#include <common/cmd.h>
#include <video/vid.h>
#include <client/input.h>
#include <game/dll.h>
#include <game/edecl.h>
#include <game/eobj.h>
#include <host/host.h>
#include <client/client.h>
#include <common/alloc.h>
#include <video/pic.h>
#include <filesys/fsys.h>
#include <game/console.h>
#include <common/cvar.h>
#include <game/state.h>
#include <net/net.h>
#include <game/cfg.h>

#include <signal.h>
#ifdef _WIN32
   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>
   #include <conio.h>
#else
   #include <unistd.h>
#endif

#undef bool

bool shouldExit = FALSE;
bool isDedicated = FALSE, isHeadless = FALSE;

double   nowTime = 0.0, deltaTime = 0.0, gameTime = 0.0;
double   lastFrame = 0.0, lastUpdate = 0.0;
uint64_t secondDur = 0;

game_state_t   gameState = GS_MENU;

cvar_t   eng_rate = {
   .name = "eng_rate",
   .flags = CVAR_NUMBER,
   .number = 60
};
cvar_t   vid_fps = {
   .name = "vid_fps",
   .flags = CVAR_NUMBER,
   .number = 60
};

static void IntHandler( int _ )
{
   shouldExit = TRUE;
}

static uint64_t CalibrateCpuFreq( void )
{
   uint64_t cpuFreq = 0;

   #ifdef _WIN32
   uint32_t* lowb = (uint32_t*) &cpuFreq;
   uint32_t* highb = (uint32_t*)((uintptr_t)&cpuFreq + sizeof(uint32_t));
   __asm
   {
      pusha
      mov ebx, lowb
      mov ecx, highb
      rdtsc
      mov [ebx], eax
      mov [ecx], edx
      popa
   }
   #elif !defined(__ARM_ARCH)
   uint32_t* lowb = (uint32_t*) &cpuFreq;
   uint32_t* highb = (uint32_t*)((uintptr_t)&cpuFreq + sizeof(uint32_t));
   #else
   __asm__(
      "mrs %0, cntvct_el0; isb;" : "=r" (cpuFreq) :: "memory"
   );
   #endif

   return cpuFreq;
}

static void CalculateSecondCycles( void )
{
   uint64_t begin = CalibrateCpuFreq( );
   #ifdef _WIN32
      Sleep( 1000 );
   #else
      sleep( 1 );
   #endif
   uint64_t end = CalibrateCpuFreq( );
   secondDur = end - begin;
   Cmd_Debugf( "second dur: %llu\n", end - begin );
}

int main( int argc, char** argv )
{
   CalculateSecondCycles( );
   signal( SIGINT, IntHandler );

   Cmd_InitArgs( argc, argv );
   isDedicated = Cmd_IsVarPresent( "-dedicated" );
   isHeadless = Cmd_IsVarPresent( "-headless" );

   // if( !(HeapSize = Cmd_GetIntArg("-heapsize")) )
   //    HeapSize = 12;

   // HeapSize *= (1<<20); // into mibibytes

   // Heap = malloc( HeapSize );
   // if( !Heap )
   //    Cmd_Errorf( "failed to allocate heap memory! (%i)\n", HeapSize );

   // Mem_Init( Heap, HeapSize );

   File_Init( );

   CVar_Register( &eng_rate );
   CVar_Register( &vid_fps );

   Game_SetupExports( );
   IN_InitMove( );

   if( !isDedicated && !isHeadless )
   {
      #ifdef _WIN32
      HWND con = GetConsoleWindow( );
      ShowWindow( con, 0 );
      #endif
      Vid_Init( );
   }

   Con_Init( );
   Net_Init( );
   Ent_Init( );
   Game_Init( );

   Cfg_Exec( "default.cfg" );

   Host_Init( );
   if( !isDedicated )
      Client_Init( );
   else
   {
      Game_Host( );
      gameLocal = TRUE;
      gameState = GS_GAME;
   }

   while( !shouldExit )
   {
      nowTime     = (double) SDL_GetTicks( ) / 1000.0;
      gameTime    = nowTime - lastUpdate;
      deltaTime   = nowTime - lastFrame;

      if( !isDedicated && !isHeadless )
         Vid_Update( );

      if( (nowTime - lastUpdate) >= (1.0 / eng_rate.number) )
      {
         Host_Update( );
         if( !isDedicated )
            Client_Update( );

         lastUpdate = nowTime;
      }

      if( (nowTime - lastFrame) >= (1.0 / vid_fps.number) && (!isDedicated && !isHeadless) )
      {
         // input
         IN_ProcessInput( );

         // drawing
         rendImports.Rend_Clear( true );

         // local interpolation
         if( gameLocal )
            lastInterp = lastUpdate;
         nowInterp = nowTime;

         switch( gameState )
         {
         case GS_GAME:
            Ent_Draw( );

         case GS_MENU:
            break;
         }

         Con_Process( );
         Con_Draw( );
         Vid_Draw( );

         lastFrame = nowTime;
      }

      if( isDedicated || isHeadless )
      {
         conOpened = TRUE;

         if( _kbhit() )
            lastKey = _getch( );
         if( lastKey == '\x3' )
            shouldExit = TRUE;

         putc( lastKey, stdout );
         Con_Process( );
      }
   }

   // Mem_Print( );
   Cmd_Printf( "Exiting...\n" );

   Host_Quit( );
   if( !isDedicated )
   {
      Client_Quit( );
      Vid_Quit( );
   }
   Net_Quit( );

   // if( Heap )
   //    free( Heap );

   return 0;
}
