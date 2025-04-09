#ifndef __SCC_H
#define __SCC_H

#include <common/common.h>

typedef float scc_vector[3];
#define VectorCopy(a, b)\
    a[0] = b[0]; a[1] = b[1]; a[2] = b[2]
#define VectorSet(a, f)\
    a[0] = f; a[1] = f; a[2] = f
#define VectorZero(v)\
    VectorSet(v, .0f)

#define SCC_MAX_STORAGESIZE 16384

typedef enum
{
   STK_OPEN_BODY = 1,
   STK_CLOSE_BODY,
   STK_OPEN_ARRAY,
   STK_CLOSE_ARRAY,

   STK_COMMENT,
   STK_DECL,
   STK_STRING,
   STK_NUMBER,
   STK_VECTOR,

   STK_UNKNOWN = 0xffff,
}
scc_token_e;

typedef struct
{
   char*           start;
   char*           end;
   char*           point;

   scc_token_e*    tokens;
   unsigned int    token_amount;

   char*           decl;
   char*           string;
   float           number;
   scc_vector      vector;

   // storage
   char*           str_storage;
   unsigned int    str_size;
   char*           str_pointer;
}
scc_t;

typedef void*   (*sccalloc_t)(size_t);
typedef void    (*sccfree_t)(void*);
typedef void*   (*sccrealloc_t)(void*, size_t);

// str funcs
extern char     S_IsWhitespace( char del );
extern int      S_CountChars( const char* str, char del );
extern int      S_SafeCmp( const char* s1, const char* s2 );

extern void     InitSCC( scc_t* scc, const char* source );
extern void     FreeSCC( scc_t* scc );
extern void     SCCFreeTokens( scc_t* scc );
extern void     SCCResizeStrStorage( scc_t* scc, int new_size );
extern void     SCCFreeStrStorage( scc_t* scc );
extern int      SCCAddToken( scc_t* scc, scc_token_e token );
extern int      GetToken( scc_t* scc );
extern void     SetAllocFuncs( sccalloc_t alloc, sccfree_t free, sccrealloc_t realloc );

// scclib
extern const char* SCCGetString( scc_t* scc, const char *name );
extern float SCCGetNumber( scc_t* scc, const char* name );
extern void SCCGetVector( scc_t* scc, const char* name, scc_vector vec );

extern const char* SCCGetDependentString( scc_t* scc, const char *name );
extern float SCCGetDependentNumber( scc_t* scc, const char* name );
extern void SCCGetDependentVector( scc_t* scc, const char* name, scc_vector vec );

extern int SCCGotoNamedBody( scc_t* scc, const char* name );
extern void SCCResetPos( scc_t* scc );
extern int SCCGetDeclPosition( scc_t* scc, const char* name );
extern int SCCGetStringPosition( scc_t* scc, const char* str );
extern int SCCGetTokenPosition( scc_t* scc, int token );
extern int SCCCountToken( scc_t* scc, int token );

extern const char* SCCGetNamedBodyString( scc_t* scc, const char* body, const char* name );
extern float SCCGetNamedBodyNumber( scc_t* scc, const char* body, const char* name );
extern void SCCGetNamedBodyVector( scc_t* scc, const char* body, const char* name, scc_vector vec );

#endif
