#ifndef EMAT_H
#define EMAT_H

#include <common/common.h>

// EMAT's are basically per-entity instructions for the
// renderer, that tell it what it needs to draw.
typedef struct emat_s
{
   char     pic[32];
   uint     pic_handle;
}
emat_t;

#endif
