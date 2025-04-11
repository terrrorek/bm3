#ifndef NET_H
#define NET_H

#include <common/common.h>
#include <game/eobj.h>

#ifdef _WIN32
   #include <WinSock2.h>
   #include <WS2tcpip.h>

   #undef EWOULDBLOCK
   #define EWOULDBLOCK WSAEWOULDBLOCK
   #undef EMSGSIZE
   #define EMSGSIZE WSAEMSGSIZE
   #undef EINPROGRESS
   #define EINPROGRESS WSAEINPROGRESS

   #define BLOCK(s, x)\
      ioctlsocket( s, FIONBIO, (u_long*) &x )
#else
   #include <sys/socket.h>
   #include <netinet/in.h>
   #include <arpa/inet.h>

   #define closesocket close
   #define BLOCK(s, x)\
      if( x )\
         fcntl( s, F_SETFL, fcntl(s, F_GETFL) | O_NONBLOCK );\
      else\
         fcntl( s, F_SETFL, fcntl(s, F_GETFL) & (~O_NONBLOCK) )
#endif

#define MAX_CLIENTS 16

#ifdef _WIN32
#define WINERR\
   result = WSAGetLastError( )
#else
#define WINERR
#endif

typedef uintptr_t sockhandle_t;

typedef enum sock_flags_e
{
   SOCK_CONNECTED    = 0x1,
   SOCK_BOUND        = 0x2
}
sock_flags_t;

typedef struct sockclient_s
{
   sockhandle_t         message;

   uint32_t             hash; // name hash
   char                 name[32];
   eobj_t*              ent;

   struct sockaddr_in   address;
   bool                 closing;
}
sockclient_t;

typedef struct bmsock_s
{
   sockhandle_t   packet, message;
   sock_flags_t   flags;
   sockclient_t   clients[MAX_CLIENTS];

   uint32_t       address;
   uint16_t       port;
}
bmsock_t;

extern char       playername[32];
extern bmsock_t   gamesock, hostsock;

uint32_t Net_GetUserHash( void );

void Net_Init( void );
void Net_Quit( void );

void Net_Host( void );
void Net_EndHost( void );

void Net_InitSocket( bmsock_t* sock );
void Net_QuitSocket( bmsock_t* sock );
#define Net_ResetSocket( sock )\
   Net_QuitSocket( sock ); Net_InitSocket( sock )

void Net_ClientLoop( void );
void Net_ServerLoop( void );

void Net_Bind( bmsock_t* sock, const char* addr, uint16_t port );

// client

bool Net_Connect( bmsock_t* sock, const char* addr, uint16_t port );
void Net_Disconnect( bmsock_t* sock );

// server

void Net_Broadcast( sockclient_t* cl, const char* fmt, ... );
void Net_BroadcastAll( bmsock_t* sock, const char* fmt, ... );

#endif
