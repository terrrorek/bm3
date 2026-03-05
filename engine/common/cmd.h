#ifndef COMMON_CMD_H
#define COMMON_CMD_H

#include <common/define.h>
#include <common/alloc.h>
#include <common/cvar.h>

void Cmd_Printf( const char* fmt, ... );
void Cmd_Errorf( const char* fmt, ... );
void Cmd_SoftErrorf( const char* fmt, ... );

typedef struct cmdarg_s
{
   char  name[32];
   char  string[64];
   float number;
}
cmdarg_t;

extern cmdarg_t*  cmdargs;
extern uint       cmdargs_count;
extern cvar_t     cmd_devmode;

void Cmd_Args( int argc, char** argv );

#endif
