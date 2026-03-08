#ifndef COMMON_MSG_H
#define COMMON_MSG_H
#include <common/define.h>

typedef struct msgbuf_s
{
   byte* buffer;
   uint  size;
   uint  cur;
}
msgbuf_t;

void MSG_Init( msgbuf_t* buf, byte* buffer, uint size );

void MSG_WriteByte( msgbuf_t* buf, byte value );
void MSG_WriteShort( msgbuf_t* buf, uint16_t value );
void MSG_WriteInt( msgbuf_t* buf, uint32_t value );
void MSG_WriteString( msgbuf_t* buf, char* string, uint32_t length );

byte MSG_ReadByte( msgbuf_t* buf );
uint16_t MSG_ReadShort( msgbuf_t* buf );
uint32_t MSG_ReadInt( msgbuf_t* buf );
char* MSG_ReadString( msgbuf_t* buf, uint32_t* length );

#endif
