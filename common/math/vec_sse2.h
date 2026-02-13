
#include <common/common.h>
#include <emmintrin.h>

typedef __m128    vec_t;
typedef vec_t     vec2_t;
typedef vec_t     vec3_t;
typedef vec_t     vec4_t;

#define VEC2_COPY(a, b) a = b
#define VEC3_COPY(a, b) a = b
#define VEC4_COPY(a, b) a = b

static void VEC_ADD( vec_t a, vec_t b, vec_t* out )
{
   *out = _mm_add_ps( a, b );
}

static void VEC_SUB( vec_t a, vec_t b, vec_t* out )
{
   *out = _mm_sub_ps( a, b );
}

static void VEC_MUL( vec_t a, vec_t b, vec_t* out )
{
   *out = _mm_mul_ps( a, b );
}

static void VEC_DIV( vec_t a, vec_t b, vec_t* out )
{
   *out = _mm_div_ps( a, b );
}

#define VEC2_ADD(a,b,out) VEC_ADD(a,b,out)
#define VEC2_SUB(a,b,out) VEC_SUB(a,b,out)
#define VEC2_MUL(a,b,out) VEC_MUL(a,b,out)
#define VEC2_DIV(a,b,out) VEC_DIV(a,b,out)

#define VEC3_ADD(a,b,out) VEC_ADD(a,b,out)
#define VEC3_SUB(a,b,out) VEC_SUB(a,b,out)
#define VEC3_MUL(a,b,out) VEC_MUL(a,b,out)
#define VEC3_DIV(a,b,out) VEC_DIV(a,b,out)

#define VEC4_ADD(a,b,out) VEC_ADD(a,b,out)
#define VEC4_SUB(a,b,out) VEC_SUB(a,b,out)
#define VEC4_MUL(a,b,out) VEC_MUL(a,b,out)
#define VEC4_DIV(a,b,out) VEC_DIV(a,b,out)
