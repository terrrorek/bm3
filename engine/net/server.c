#include <net/net.h>
#include <net/bitfield.h>
#include <net/msg.h>
#include <common/bmstring.h>
#include <game/console.h>

extern double nowTime;

sockcl_t net_serverclients[MAX_PLAYERS] = {0};

static void SNet_Broadcast( const char* fmt, ... )
{
   char message[MAX_BITFIELD_STR_LEN];
   va_list args;

   msg_t msg = {
      .type = MSGT_BROADCAST,
   };
   int i;
   bitbuffer_t buffer = Bit_FromMessage(&msg);

   va_start( args, fmt );
   vsnprintf( message, MAX_BITFIELD_STR_LEN, fmt, args );
   va_end( args );

   Bit_WriteString( &buffer, message );
   for( i = 0; i < MAX_PLAYERS; i++ )
   {
      sockcl_t* cl = &net_serverclients[i];
      if( cl->hash )
         Net_Send( &net_server, &msg, sizeof(msg), &cl->address );
   }
   Con_Printf( "b: %s", message );
}
static inline void SNet_Disconnect( sockcl_t* cl, const char* reason )
{
   msg_t msg = {
      .type = MSGT_DISCONNECT
   };
   bitbuffer_t buffer = Bit_FromMessage(&msg);
   Bit_WriteString( &buffer, reason );
   Net_Send( &net_server, &buffer, sizeof(buffer), &cl->address );
   SNet_Broadcast( "%s disconnected.\n", cl->name );
   WIPEPTR( cl );
}

static sockcl_t* SNet_FindClientByAddress( addr_t addr )
{
   int i;

   for( i = 0; i < MAX_PLAYERS; i++ )
      if( net_serverclients[i].address.sin_addr.s_addr == addr.sin_addr.s_addr )
         return &net_serverclients[i];
   return NULL;
}
static sockcl_t* SNet_FindClientByHash( uint32_t hash )
{
   int i;

   for( i = 0; i < MAX_PLAYERS; i++ )
      if( net_serverclients[i].hash == hash )
         return &net_serverclients[i];
   return NULL;
}
static sockcl_t* SNet_FindEmptySlot( void )
{
   int i;

   for( i = 0; i < MAX_PLAYERS; i++ )
      if( !net_serverclients[i].hash )
         return &net_serverclients[i];
   return NULL;
}

static void SNet_ProcessConnect( msg_t* msg, addr_t* addr )
{
   msg_t response;
   bitbuffer_t resp_buffer = {.buffer = response.data, .size = MAX_MSG_SIZE};

   bitbuffer_t buffer = {.buffer = msg->data, .size = MAX_MSG_SIZE};
   sockcl_t* cl;
   char* reason = "", * name;

   if(( SNet_FindClientByHash( msg->hash )))
      return;

   name = Bit_ReadString( &buffer );
   if( !name )
   {
      reason = "invalid name sent";
      goto disconnect;
   }

   if(!( cl = SNet_FindEmptySlot()))
   {
      reason = "server is full";
      goto disconnect;
   }

   cl->hash = msg->hash;
   cl->address = *addr;
   BM_strncpy( cl->name, name, MAX_PLAYERNAME );
   SNet_Broadcast( "%s connected.\n", cl->name );

   response.type = MSGT_CONNECT;
   Net_Send( &net_server, &response, sizeof(response), addr );

   return;
disconnect:
   response.type = MSGT_DISCONNECT;
   Bit_WriteString( &resp_buffer, reason );
   Net_Send( &net_server, &response, sizeof(response), addr );
}

static void SNet_ProcessDisconnect( msg_t* msg, sockcl_t* cl )
{
   bitbuffer_t buffer = Bit_FromMessage(msg);

   if( !cl ) return;
   SNet_Broadcast( "%s disconnected. (%s)\n", cl->name, Bit_ReadString( &buffer ));
   WIPEPTR( cl );
}

void     SNet_Host( void )
{
   addr_t   address;
   address.sin_family = AF_INET;
   address.sin_port = htons( 37215 );
   inet_pton( AF_INET, "0.0.0.0", &address.sin_addr.s_addr );

   Net_Bind( &net_server, &address );
}

void     SNet_Update( void )
{
   int      result, i;
   msg_t    msg;
   addr_t   addr;

   result = Net_Receive( &net_server, &msg, sizeof(msg), &addr );
   WINERR;
   if( result != 0 && result != EWOULDBLOCK )
   {
      Cmd_Printf( "failed to receive client message. (%i)\n", result );
      return;
   }
   if( result == EWOULDBLOCK )
      return;

   sockcl_t* cl = SNet_FindClientByHash( msg.hash );
   if( cl )
   {
      cl->last_packet = nowTime;
   }

   switch( msg.type )
   {
   case MSGT_CONNECT:
      SNet_ProcessConnect( &msg, &addr );
      break;
   case MSGT_DISCONNECT:
      SNet_ProcessDisconnect( &msg, cl );
      break;
   default:
      break;
   }

   // // check for inactive users
   // for( i = 0; i < MAX_PLAYERS; i++ )
   // {
   //    sockcl_t* cl = &net_serverclients[i];
   //    if( !cl->hash )
   //       continue;
   //    if( cl->last_packet + MAX_INACTIVITY_TIME >= nowTime )
   //       SNet_Disconnect( cl, "user was packet inactive" );
   // }
}

void     SNet_EndHost( void )
{
   WIPEOUT( net_serverclients );
   Net_Close( &net_server );
   Net_Open( &net_server );
}
