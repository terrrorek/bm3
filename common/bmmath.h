
#ifndef BMMATH_H
#define BMMATH_H

#include <common/common.h>
#include <cglm/cglm.h>

typedef byte   bcol_t; // byte color
typedef float  ncol_t; // normalized float color

typedef bcol_t rgb_t[3];
typedef ncol_t frgb_t[3];

typedef bcol_t rgba_t[4];
typedef ncol_t frgba_t[4];

typedef struct transform_s
{
   vec3 position, rotation, scale;
}
transform_t;

static inline float BM_lerp(float a, float b, float f)
{
    return a * (1.0 - f) + (b * f);
}

static inline float BM_clamp(float x, float min, float max)
{
   if( x < min )
      return min;
   else if( x > max )
      return max;
   else
      return x;
}

#endif
