#include <net/net.h>
#include <common/bmstring.h>
#include <common/cvar.h>
#include <game/console.h>
#include <3rdparty/blake2/blake2.h>

#ifdef _WIN32
static WSADATA wsaData;
#endif

char     playername[MAX_PLAYERNAME];
sock_t   net_client, net_server;

static byte       playerKey[16] = "bartusmastus";
static uint32_t   playerHash = 0;

static void Net_Playername_f( const char* name )
{
   if( !name )
      return;
   if( strnlen( name, MAX_PLAYERNAME ) >= MAX_PLAYERNAME )
      Con_Printf( "can't set playername: name too long. cropping. (max length %i)\n", MAX_PLAYERNAME );
   BM_strncpy( playername, name, MAX_PLAYERNAME );
   playerHash = 0;
}
static cvar_t _playername = {
   .name = "playername",
   .flags = CVAR_FUNCTION,
   .function = Net_Playername_f
};

uint32_t Net_GetUserHash( void )
{
   if( !playerHash )
   {
      blake2s( &playerHash, sizeof(playerHash),
            playername, sizeof(playername),
            playerKey,  sizeof(playerKey) );
   }
   return playerHash;
}

void Net_Init( void )
{
   #ifdef _WIN32
   WSAStartup( MAKEWORD(2,0), &wsaData );
   #endif

   CVar_Register( &_playername );

   WIPEOUT( net_client );
   WIPEOUT( net_server );

   Net_Open( &net_client );
   Net_Open( &net_server );
}

void Net_Quit( void )
{
   Net_Close( &net_client );
   Net_Close( &net_server );
}

void     Net_Open( sock_t* sock )
{
   WIPEPTR( sock );
   int type = SOCK_DGRAM;

   #ifndef _WIN32
   type |= SOCK_NONBLOCK;
   #endif

   sock->handle = socket( AF_INET, type, IPPROTO_UDP );
   if( !sock->handle )
   {
      Cmd_SoftErrorf( "failed to create socket" );
      return;
   }

   #ifdef _WIN32
   u_long mode = 1;
   ioctlsocket( sock->handle, FIONBIO, &mode );
   #endif
}

int      Net_Send( sock_t* sock, void* data, size_t size, addr_t* addr )
{
   int result = sendto( sock->handle, (char*) data, size, 0, (struct sockaddr*) addr, sizeof(addr_t) );
   return result;
}

int      Net_Receive( sock_t* sock, void* data, size_t size, addr_t* addr )
{
   int fromlen = sizeof(addr_t);
   int result = recvfrom( sock->handle, (char*) data, size, 0, (struct sockaddr*) addr, &fromlen );
   return result;
}

void     Net_Bind( sock_t* sock, addr_t* addr )
{
   int   result;

   sock->bound = TRUE;
   result = bind( sock->handle, (struct sockaddr*) addr, sizeof(addr_t) );
   if( result != 0 )
   {
      WINERR;
      Cmd_SoftErrorf( "failed to bind socket" );
   }
}

void     Net_Close( sock_t* sock )
{
   #ifdef _WIN32
   closesocket( sock->handle );
   #else
   close( sock->handle );
   #endif
   WIPEPTR( sock );
}
