#ifndef COMMON_ALLOC_H
#define COMMON_ALLOC_H

#include <common/define.h>
#include <stdlib.h>

#define Mem_Alloc(__size__)   calloc( 1, __size__ )
#define Mem_Free(__ptr__)     free( __ptr__ )

#endif
