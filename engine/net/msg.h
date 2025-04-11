#ifndef MSG_H
#define MSG_H

#include <common/common.h>

typedef enum msg_type_e
{
   MSGT_CONNECT = 1,
   MSGT_DISCONNECT,
   MSGT_BROADCAST,
   MSGT_INFO,
}
msg_type_t;

#define MAX_MSG_SIZE 512

typedef struct msg_s
{
   byte        type;
   uint32_t    hash;
   byte        data[MAX_MSG_SIZE];
}
msg_t;

#endif
