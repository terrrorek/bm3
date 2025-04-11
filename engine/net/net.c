#include <net/net.h>
#include <common/cmd.h>
#include <common/cvar.h>
#include <common/bmstring.h>
#include <game/console.h>
#include <game/state.h>
#include <3rdparty/blake2/blake2.h>

bmsock_t          gamesock, hostsock;
char              playername[32];

static const char netkey[16] = "kolegabartek";
static uint32_t   playerhash = 0;

#ifdef _WIN32
WSADATA wsa;
#endif

uint32_t Net_GetUserHash( void )
{
   if( !playerhash )
      blake2s( &playerhash, sizeof(playerhash), playername, sizeof(playername), netkey, sizeof(netkey) );
   return playerhash;
}

static void Net_Playername_f( const char* name )
{
   if( gameState == GS_GAME )
   {
      Con_Errorf( "you can't change your name while in-game!\n" );
      return;
   }

   BM_strncpy( playername, name, 32 );
   playerhash = 0;
}
static cvar_t _playername = {
   .name = "playername",
   .flags = CVAR_FUNCTION,
   .function = Net_Playername_f
};

void Net_Init( void )
{
   #ifdef _WIN32
   WSAStartup( 2, &wsa );
   #endif

   CVar_Register( &_playername );

   Net_InitSocket( &gamesock );
   Net_InitSocket( &hostsock );
}

void Net_Quit( void )
{
   Net_QuitSocket( &gamesock );
   Net_QuitSocket( &hostsock );
}

void Net_InitSocket( bmsock_t* sock )
{
   #ifndef _WIN32
   int flags = SOCK_NONBLOCK;
   #else
   int flags = 0;
   #endif

   WIPEPTR( sock );

   if(( sock->packet = socket( AF_INET, SOCK_DGRAM | flags, IPPROTO_UDP )) == 0 )
   {
      Cmd_Errorf( "failed to init socket! (packet failure).\n" );
   }

   if(( sock->message = socket( AF_INET, SOCK_STREAM | flags, IPPROTO_TCP )) == 0 )
   {
      Cmd_Errorf( "failed to init socket! (message failure).\n" );
   }

   #ifdef _WIN32
   flags = 1;
   BLOCK( sock->packet, flags );
   BLOCK( sock->message, flags );
   #endif
}

void Net_QuitSocket( bmsock_t* sock )
{
   if( sock->packet )
      closesocket( sock->packet );
   if( sock->message )
      closesocket( sock->message );

   WIPEPTR( sock );
}

void Net_Bind( bmsock_t* sock, const char* address, uint16_t port )
{
   struct sockaddr_in   addr;
   int                  len = sizeof(addr);
   int                  result;

   inet_pton( AF_INET, address, &addr.sin_addr );
   addr.sin_port = htons( port );
   addr.sin_family = AF_INET;

   result = bind( sock->packet, (struct sockaddr*) &addr, len );
   if( result != 0 )
   {
      WINERR;
      Cmd_SoftErrorf( "failed to bind packet socket. (%i)\n", result );
      return;
   }

   addr.sin_port = htons( port + 1 );

   result = bind( sock->message, (struct sockaddr*) &addr, len );
   if( result != 0 )
   {
      WINERR;
      Cmd_SoftErrorf( "failed to bind message socket. (%i)\n", result );
      return;
   }

   sock->flags |= SOCK_BOUND;
   sock->address = addr.sin_addr.s_addr;
   sock->port = port;
}

void Net_Host( void )
{
   Net_ResetSocket( &hostsock );
   Net_Bind( &hostsock, "0.0.0.0", 21371 );
}

void Net_EndHost( void )
{
   Net_ResetSocket( &hostsock );
}
