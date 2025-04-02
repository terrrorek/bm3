#ifndef EMAT_H
#define EMAT_H

#include <common/common.h>

// EMAT's are basically per-entity instructions for the
// renderer, that tell it what it needs to draw.
typedef struct emat_s
{
   uint  pic;
}
emat_t;

#endif
