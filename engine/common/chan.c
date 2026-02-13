
#include <common/chan.h>
#include <common/cmd.h>

channel_t  channels[MAX_CHANNELS] = {0};
remote_t   remotes[MAX_REMOTES] = {0};

static channel_t* Chan_FindEmptyChannel( void )
{
   int i = 0;
   do
   {
      if( !channels[i].index )
      {
         return &channels[i];
      }
      ++i;
   }
   while( i < MAX_CHANNELS );
   return NULL;
}

static remote_t* Chan_FindEmptyRemote( void )
{
   int i = 0;
   do
   {
      if( !remotes[i].index )
      {
         return &remotes[i];
      }
      ++i;
   }
   while( i < MAX_REMOTES );
   return NULL;
}

remote_t* Chan_OpenRemote( channel_t* parent )
{
   remote_t* rem = Chan_FindEmptyRemote( );
   if( !rem )
   {
      Cmd_Errorf( "reached max remotes\n" );
   }

   WIPEPTR( rem );
   rem->index = (rem - remotes) + 1;
   rem->parent = parent;

   return rem;
}

channel_t* Chan_OpenChannel( chan_flags_t type, bool allocRemote )
{
   channel_t* chan = Chan_FindEmptyChannel( );
   if( !chan )
   {
      Cmd_Errorf( "reached max channels\n" );
   }

   WIPEPTR( chan );
   chan->type = type;
   chan->index = (chan - channels) + 1;
   
   if( allocRemote )
   {
      chan->remotes = malloc( ++chan->remoteCount * sizeof( remote_t* ) );
      chan->remotes[0] = Chan_OpenRemote( chan );
   }

   return chan;
}

void Chan_CloseRemote( remote_t* remote )
{
   if( !remote )
      return;
   if( !remote->index )
      return;

   if( remote->channel )
      for( byte i = 0; i < CHANNEL_BUS_AMOUNT; i++ )
         remote->channel->pending[i] = FALSE;

   // TODO: close connections and all that stuff

   WIPEPTR( remote );
}

void Chan_CloseChannel( channel_t* channel )
{
   Cmd_Printf( "%i\n", channel->index );
   if( !channel )
      return;
   if( !channel->index )
      return;

   if( channel->remotes )
   {
      for( uint i = 0; i < channel->remoteCount; i++ )
         Chan_CloseRemote( channel->remotes[i] ); 
      free( channel->remotes );
   }
   WIPEPTR( channel );
}

void Chan_SendRemote( channel_t* channel, remote_t* remote, byte bus )
{
   if( remote->channel )
      if( !remote->channel->pending[bus] && remote->channel->type == channel->type )
      {
         remote->channel->len = channel->len;
         memcpy( remote->channel->data[bus], channel->data[bus], CHANNEL_DATA_SIZE );
         remote->channel->pending[bus] = TRUE;
      }
   if( remote->address && remote->port )
   {

   }
}

bool Chan_SendToRemote( channel_t* channel, uint remote, byte bus )
{
   if( channel->pending[bus] || !channel->remoteCount )
      return FALSE;
   if( remote >= channel->remoteCount )
      return FALSE;
   
   Chan_SendRemote( channel, channel->remotes[remote], bus );
   return TRUE;
}

bool Chan_SendToRemotes( channel_t* channel, byte bus )
{
   if( channel->pending[bus] || !channel->remoteCount )
      return FALSE;

   for( uint i = 0; i < channel->remoteCount; i++ )
   {
      remote_t* rem = channel->remotes[i];
      Chan_SendRemote( channel, rem, bus );
   }
   return TRUE;
}

uint Chan_AddRemote( channel_t* channel, remote_t* remote )
{
   // check for empty ones
   if( channel->remoteCount > 0 )
   {
      uint i = 0;
      do
      {
         if( !channel->remotes[i]->index )
         {
            channel->remotes[i] = remote;
            return i;
         }
         ++i;
      }
      while( i < channel->remoteCount );
   }

   channel->remotes = realloc( channel->remotes, ++channel->remoteCount * sizeof(remote_t*) );
   channel->remotes[channel->remoteCount - 1] = remote;
   return channel->remoteCount - 1;
}


uint Chan_GetRemoteIndex( channel_t* channel, remote_t* remote )
{
   uint i = 0;
   do
   {
      if( channel->remotes[i] == remote )
         return i;
      ++i;
   }
   while( i < channel->remoteCount );
   return 0;
}

bool Chan_IsDataPending( channel_t* channel, byte bus )
{
   return channel->pending[bus] ? TRUE : FALSE;
}

