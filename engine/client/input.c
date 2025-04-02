#include <client/input.h>
#include <common/bmstring.h>
#include <common/cmd.h>

static input_t* last_input = NULL;
static bool* input_map = NULL;

bool     captureInput = false;
char     lastKey = 0;

inmove_t inmove = {0};

static input_t  forward, backward, moveleft, moveright;

void IN_InitMove( void )
{
   forward = (input_t){
      .name = "forward",
      .repeat = true,
      .keycode = SDLK_W,
      .mouse = false,
   };
   backward = (input_t){
      .name = "backward",
      .repeat = true,
      .keycode = SDLK_S,
      .mouse = false,
   };
   moveleft = (input_t){
      .name = "moveleft",
      .repeat = true,
      .keycode = SDLK_A,
      .mouse = false,
   };
   moveright = (input_t){
      .name = "moveright",
      .repeat = true,
      .keycode = SDLK_D,
      .mouse = false,
   };

   IN_RegisterInput( &forward );
   IN_RegisterInput( &backward );
   IN_RegisterInput( &moveleft );
   IN_RegisterInput( &moveright );
}

void IN_RegisterInput( input_t* input )
{
   if( last_input )
      last_input->next = input;
   input->prev = last_input;
   last_input = input;
}

void IN_ProcessInput( void )
{
   input_t*       input;
   SDL_Scancode   sc;
   bool           rp;

   if( !input_map )
      input_map = (bool*) SDL_GetKeyboardState( NULL );
   input = last_input;

   do
   {
      if( !input )
         break;
      if( captureInput && !input->whenCapture )
      {
         input->_pressed = false;
         input->_last_pressed = false;
         goto next;
      }

      if( input->mouse )
      {} // TODO: mouse
      else
      {
         sc = SDL_GetScancodeFromKey( input->keycode, NULL );
         rp = input_map[sc];

         if( !rp && input->repeat )
            { input->_pressed = false, input->_last_pressed = false; goto next; }
         else if( rp && input->repeat )
            { input->_pressed = true, input->_last_pressed = false; goto next; }
         else if( rp && !input->repeat && !input->_last_pressed )
            { input->_pressed = true, input->_last_pressed = true; goto next; }
         else if( rp && !input->repeat && input->_pressed )
            { input->_pressed = false; goto next; }
         else if( !rp && !input->repeat && !input->_pressed )
            { input->_pressed = false; input->_last_pressed = false; goto next; }
      }

   next:
      input = input->prev;
   }
   while( input );

   // keyboard inmove
   inmove.forward = (IN_IsInputPressed( "forward" )) + -(IN_IsInputPressed( "backward" ));
   inmove.right = (IN_IsInputPressed( "moveright" )) + -(IN_IsInputPressed( "moveleft" ));
   inmove.up = 0.f;

   // Cmd_Printf( "%f %f %f\n", inmove.forward, inmove.right, inmove.up );
}

bool IN_IsInputPressed( const char* name )
{
   input_t* in;

   in = last_input;
   do
   {
      if( !in )
         break;

      if( !BM_strcmp( in->name, name ) )
         return in->_pressed;
      in = in->prev;
   }
   while( in );

   return false;
}

void IN_Event( SDL_Keycode key )
{
   lastKey = key;
}
