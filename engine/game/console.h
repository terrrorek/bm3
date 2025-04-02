#ifndef CONSOLE_H
#define CONSOLE_H

#include <common/common.h>

#define Con_Errorf( fmt, ... ) Con_Printf( "ERROR: " fmt, __VA_ARGS__ )

void Con_Init( void );
void Con_Process( void );
void Con_Draw( void );
void Con_Printf( const char* fmt, ... );

#endif
