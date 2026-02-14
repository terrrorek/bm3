#ifndef BITFIELD_H
#define BITFIELD_H

#include <common/common.h>
#include <game/eobj.h>

typedef struct bitbuffer_s
{
   byte*    buffer;
   uint     size;
   uint     offset;
}
bitbuffer_t;

typedef enum bittype_e
{
   BIT_BYTE = 1,
   BIT_INT,
   BIT_SHORT,
   BIT_FLOAT,
   BIT_DOUBLE,
   BIT_STRING,
}
bittype_t;

typedef struct bitfield_s
{
   byte  type;
   byte  length;
}
bitfield_t;

#define MAX_BITFIELD_STR_LEN 128

#define BIT_WRITE(_type, _name) \
   bool Bit_Write##_name( bitbuffer_t* buf, _type value )
#define BIT_READ(_type, _name) \
   _type Bit_Read##_name( bitbuffer_t* buf )

BIT_WRITE( byte, Byte );
BIT_WRITE( int, Int );
BIT_WRITE( short, Short );
BIT_WRITE( float, Float );
BIT_WRITE( double, Double );
BIT_WRITE( const char*, String );

BIT_READ( byte, Byte );
BIT_READ( int, Int );
BIT_READ( short, Short );
BIT_READ( float, Float );
BIT_READ( double, Double );
BIT_READ( char*, String );

#endif
