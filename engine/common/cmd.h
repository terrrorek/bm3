#ifndef COMMON_CMD_H
#define COMMON_CMD_H

#include <common/define.h>
#include <common/alloc.h>

void Cmd_Printf( const char* fmt, ... );
void Cmd_Errorf( const char* fmt, ... );
void Cmd_SoftErrorf( const char* fmt, ... );

#endif
