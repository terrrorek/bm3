#ifndef INPUT_H
#define INPUT_H

#include <common/common.h>
#include <SDL3/SDL.h>
#undef bool

typedef struct input_s
{
   char name[16];
   byte keycode;
   bool repeat, mouse;
   bool whenCapture;

   bool _pressed, _last_pressed;
   struct input_s* prev, * next;
}
input_t;

typedef struct inmove_s
{
   float forward;
   float right;
   float up;
}
inmove_t;

void IN_InitMove( void );
void IN_RegisterInput( input_t* input );
void IN_ProcessInput( void );
bool IN_IsInputPressed( const char* name );
void IN_Event( SDL_Keycode key );

extern bool captureInput;
extern char lastKey;
extern inmove_t inmove;

#endif
