
#ifdef _WIN32
   #define EXPORT __declspec(dllexport)
#else
   #define EXPORT __attribute__((__visibility__("default")))
#endif

#define CLASS(x, ...)\
   typedef struct x##_s\
   { eobj_t* obj; __VA_ARGS__ } x##_t;

#define CREATE(x, c)\
   EXPORT void x##_create( void** ret, eobj_t* obj )\
   {\
      x##_t* this;\
      *ret = Mem_Alloc( sizeof( x##_t ) );\
      memset( *ret, 0, sizeof( x##_t ) );\
      this = (x##_t*)(*ret);\
      this->obj = obj;\
      c\
   }

#define SPAWN(x)\
   EXPORT void x##_spawn( x##_t* this )

#define UPDATE(x)\
   EXPORT void x##_update( x##_t* this, bool client )

#define KILL(x)\
   EXPORT void x##_kill( x##_t* this )
