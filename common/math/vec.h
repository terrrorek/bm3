
#ifndef MATH_VEC_H
#define MATH_VEC_H

#include <common/common.h>

#if (!defined(__SSE2__) && !defined(__SSE2_MATH__)) && !defined( __FLOAT_MATH__ )
   #error SSE2 must be supported!
#endif

#ifdef __FLOAT_MATH__
   #include "vec_float.h"
#else
  #include "vec_sse2.h"
#endif // FLOAT_MATH

#endif // HEADER GUARD
