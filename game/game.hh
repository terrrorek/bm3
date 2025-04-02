
#ifndef GAME_HH
#define GAME_HH

#include <engine/game/dll.h>
#include <engine/game/eobj.h>

#if defined( _WIN32 )
   #define EXPORT extern "C" __declspec(dllexport)
#elif defined( __linux__ )
   #define EXPORT extern "C" __attribute__((visibility("default")))
#endif

#define EXPORT_CLASS(name, clazz) \
   EXPORT void __##name( void** dest, eobj_t* eobj ) \
   { clazz* cl = new clazz; cl->eobj = eobj; *dest = (void*) cl; } \
   EXPORT void __##name##_spawn( clazz* x ) \
   { if(x) x->Spawn( ); } \
   EXPORT void __##name##_update( clazz* x, bool client ) \
   { if(x) x->Update( client ); } \
   EXPORT void __##name##_kill( clazz* x ) \
   { if(x) x->Kill( ); } \
   EXPORT void __##name##_destroy( clazz* x ) \
   { if(x) delete x; }

extern eng_exports_t* engFuncs;

class CEntity
{
public:
   eobj_t*  eobj;

   virtual void Spawn( void )             {}
   virtual void Update( bool isClient )   {(void)isClient;}
   virtual void Kill( void )              {}
};

#endif
