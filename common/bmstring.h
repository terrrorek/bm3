
#ifndef BMSTRING_H
#define BMSTRING_H

#include <common/common.h>
#include <stdarg.h>

static inline int BM_strcmp( const char* l, const char* r )
{
   if( !l || !r )
      return INT32_MAX;
   return strcmp( l, r );
}

static inline void BM_strcpy( char* dest, const char* src )
{
   if( !dest || !src )
      return;
   strcpy( dest, src );
}

static inline void BM_strncpy( char* dest, const char* src, size_t amount )
{
   if( !dest || !src || !amount )
      return;
   strncpy( dest, src, amount );
}

static inline void BM_snprintf( char* buffer, size_t amount, const char* fmt, ... )
{
   if( !buffer || !amount || !fmt )
      return;

   va_list args;
   va_start( args, fmt );
   vsnprintf( buffer, amount, fmt, args );
   va_end( args );
}

#endif
