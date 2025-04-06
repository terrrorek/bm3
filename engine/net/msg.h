#ifndef MSG_H
#define MSG_H

#include <common/common.h>

enum msg_type_e
{
   MSG_TYPE_CONNECT = 1,
   MSG_TYPE_DISCONNECT,
};

#define MAX_MSG_DATA 63

typedef struct msg_s
{
   byte     type;
   byte     data[MAX_MSG_DATA];
}
msg_t;

#endif
