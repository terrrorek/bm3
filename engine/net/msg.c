#include <net/msg.h>

#define SIZE_CHECK if( buf->cur >= buf->size ) return
#define READ_SIZE_CHECK(r) if( buf->cur >= buf->size ) return r

void MSG_Init( msgbuf_t* buf, byte* buffer, uint size )
{
   buf->buffer = buffer;
   buf->size = size;
   buf->cur = 0;
}

void MSG_WriteByte( msgbuf_t* buf, byte value )
{
   SIZE_CHECK;
   *(byte*)(buf->buffer + buf->cur) = value;
   buf->cur += sizeof(value);
}
void MSG_WriteShort( msgbuf_t* buf, uint16_t value )
{
   SIZE_CHECK;
   *(uint16_t*)(buf->buffer + buf->cur) = value;
   buf->cur += sizeof(value);
}
void MSG_WriteInt( msgbuf_t* buf, uint32_t value )
{
   SIZE_CHECK;
   *(uint32_t*)(buf->buffer + buf->cur) = value;
   buf->cur += sizeof(value);
}
void MSG_WriteString( msgbuf_t* buf, char* string, uint32_t length )
{
   uint  i, _length;

   SIZE_CHECK;
   _length = !length ? strlen(string) : length;
   MSG_WriteShort( buf, _length );
   for( i = 0; i < _length; i++ )
      MSG_WriteByte( buf, string[i] );
}

byte MSG_ReadByte( msgbuf_t* buf )
{
   byte     value;
   READ_SIZE_CHECK(0);

   value = *(byte*)( buf->buffer + buf->cur );
   buf->cur += sizeof(value);
   return value;
}
uint16_t MSG_ReadShort( msgbuf_t* buf )
{
   uint16_t value;
   READ_SIZE_CHECK(0);

   value = *(uint16_t*)( buf->buffer + buf->cur );
   buf->cur += sizeof(value);
   return value;
}
uint32_t MSG_ReadInt( msgbuf_t* buf )
{
   uint32_t value;
   READ_SIZE_CHECK(0);

   value = *(uint32_t*)( buf->buffer + buf->cur );
   buf->cur += sizeof(value);
   return value;
}
char* MSG_ReadString( msgbuf_t* buf, uint32_t* length )
{
   static char buffer[2048] = {0};
   uint        i, _length;

   READ_SIZE_CHECK(buffer);

   _length = MSG_ReadShort( buf );
   for( i = 0; i < _length; i++ )
      buffer[i] = MSG_ReadByte( buf );
   if( length ) *length = _length;
   return buffer;
}
