
#ifndef CMD_H
#define CMD_H

#include <common/common.h>
#include <stdarg.h>

void Cmd_Printf( const char* fmt, ... );
void Cmd_Debugf( const char* fmt, ... );
void Cmd_Errorf( const char* fmt, ... );
void Cmd_SoftErrorf( const char* fmt, ... );

#endif
