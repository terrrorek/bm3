
#include "game.hh"

eng_exports_t* engFuncs = {0};

EXPORT int Game_LoadFuncs( eng_exports_t* exports )
{
   engFuncs = exports;

   return 0;
}
