#include <common/common.h>

#define VEC(size)    \
   TYPEDEF_SIZE(vec, size)

typedef float  vec_t;
VEC(2);
VEC(3);
VEC(4);

#define VEC_LOOP_SIZE_OP(a, b, size, op)\
   for( int __##size = 0; __##size < size; __##size++ )\
      a[__##size] op b[__##size];

#define VEC_INLINE_FUNC(size, name, op)\
   inline static void VEC##size##_##name( vec##size##_t a, \
      vec##size##_t b, vec##size##_t* out )\
   {  \
      for( int i = 0; i < size; i++ )  \
      {  \
         (*out)[i] = a[i] op b[i];\
      }  \
   }

#define VEC2_COPY(a, b)    VEC_LOOP_SIZE_OP(a, b, 2, =)
#define VEC3_COPY(a, b)    VEC_LOOP_SIZE_OP(a, b, 3, =)
#define VEC4_COPY(a, b)    VEC_LOOP_SIZE_OP(a, b, 4, =)

VEC_INLINE_FUNC(2, ADD, +)
VEC_INLINE_FUNC(2, SUB, -)
VEC_INLINE_FUNC(2, MUL, *)
VEC_INLINE_FUNC(2, DIV, /)

VEC_INLINE_FUNC(3, ADD, +)
VEC_INLINE_FUNC(3, SUB, -)
VEC_INLINE_FUNC(3, MUL, *)
VEC_INLINE_FUNC(3, DIV, /)

VEC_INLINE_FUNC(4, ADD, +)
VEC_INLINE_FUNC(4, SUB, -)
VEC_INLINE_FUNC(4, MUL, *)
VEC_INLINE_FUNC(4, DIV, /)

#undef VEC

typedef struct transform_s
{
   vec3_t position, rotation, scale;
} transform_t;
