
#ifndef ARGS_H
#define ARGS_H

#include <common/common.h>
#include <common/bmstring.h>

extern int     ArgCount;
extern char**  ArgStrings;

void Cmd_InitArgs( int argc, char** argv );
char* Cmd_GetStringArg( const char* name );
int Cmd_GetIntArg( const char* name );
bool Cmd_IsVarPresent( const char* name );

#endif
