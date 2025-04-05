#ifndef MSG_H
#define MSG_H

#include <common/common.h>

typedef enum
{
   MSG_TYPE_STOC, // server to clients,
   MSG_TYPE_CTOS, // client to server,
   MSG_TYPE_UNI // uniform message, process by both
}
msg_type_t;

typedef enum
{
   MSG_FIELD_BYTE,
   MSG_FIELD_SHORT,
   MSG_FIELD_INT,
   MSG_FIELD_LONG, // int types

   MSG_FIELD_FLOAT,
   MSG_FIELD_DOUBLE, // float types

   MSG_FIELD_STRING, // string type

   MSG_FIELD_MAX,
}
msg_field_t;

typedef struct msg_header_s
{
   byte           type;
   uint           fieldnum;
   byte*          fields;
}
msg_header_t;

void  Net_BeginMessage( msg_type_t type );
void* Net_EndMessage( void );

#endif
