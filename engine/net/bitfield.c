#include <net/bitfield.h>

bitbuffer_t Bit_FromMessage( msg_t* msg ) {
   return (bitbuffer_t){
      .buffer = &msg->data[0],
      .offset = 0,
      .size = MAX_MSG_SIZE
   };
}

#define LENGTH_CHECK\
   if( buf->offset + sizeof(bitfield_t) + sizeof(value) >= buf->size ) return FALSE
#define WRITE_FIELD(field)\
   memcpy( buf->buffer + buf->offset, &field, sizeof(field) ); buf->offset += sizeof(field); \
   memcpy( buf->buffer + buf->offset, &value, sizeof(value) ); buf->offset += sizeof(value)

BIT_WRITE( byte, Byte )
{
   bitfield_t  field = {
      .length = sizeof(value),
      .type = BIT_BYTE
   };

   LENGTH_CHECK;
   WRITE_FIELD( field );

   return TRUE;
}

BIT_WRITE( int, Int )
{
   bitfield_t  field = {
      .length = sizeof(value),
      .type = BIT_INT
   };

   LENGTH_CHECK;
   WRITE_FIELD( field );

   return TRUE;
}

BIT_WRITE( short, Short )
{
   bitfield_t  field = {
      .length = sizeof(value),
      .type = BIT_SHORT
   };

   LENGTH_CHECK;
   WRITE_FIELD( field );

   return TRUE;
}

BIT_WRITE( float, Float )
{
   bitfield_t  field = {
      .length = sizeof(value),
      .type = BIT_FLOAT
   };

   LENGTH_CHECK;
   WRITE_FIELD( field );

   return TRUE;
}

BIT_WRITE( double, Double )
{
   bitfield_t  field = {
      .length = sizeof(value),
      .type = BIT_DOUBLE
   };

   LENGTH_CHECK;
   WRITE_FIELD( field );

   return TRUE;
}

BIT_WRITE( const char*, String )
{
   bitfield_t  field = {
      .length = strnlen( value, MAX_BITFIELD_STR_LEN )+1,
      .type = BIT_STRING
   };

   if( buf->offset + sizeof(bitfield_t) + field.length >= buf->size )
      return FALSE;

   memcpy(buf->buffer + buf->offset, &field, sizeof(field));
   buf->offset += sizeof(field);
   memcpy(buf->buffer + buf->offset, value, field.length);
   buf->offset += field.length;

   return TRUE;
}

BIT_READ( byte, Byte )
{
   bitfield_t* field = (bitfield_t*)(buf->buffer + buf->offset);

   if( buf->offset + sizeof(bitfield_t) + field->length >= buf->size )
      return ~0;
   if( field->type != BIT_BYTE )
      return ~0;

   buf->offset += sizeof(*field) + sizeof(byte);
   return *(byte*)((uintptr_t)field + sizeof(bitfield_t));
}

BIT_READ( int, Int )
{
   bitfield_t* field = (bitfield_t*)(buf->buffer + buf->offset);

   if( buf->offset + sizeof(bitfield_t) + field->length >= buf->size )
      return ~0;
   if( field->type != BIT_INT )
      return ~0;

   buf->offset += sizeof(*field) + sizeof(int);
   return *(int*)((uintptr_t)field + sizeof(bitfield_t));
}

BIT_READ( short, Short )
{
   bitfield_t* field = (bitfield_t*)(buf->buffer + buf->offset);

   if( buf->offset + sizeof(bitfield_t) + field->length >= buf->size )
      return ~0;
   if( field->type != BIT_SHORT )
      return ~0;

   buf->offset += sizeof(*field) + sizeof(short);
   return *(short*)((uintptr_t)field + sizeof(bitfield_t));
}

BIT_READ( float, Float )
{
   bitfield_t* field = (bitfield_t*)(buf->buffer + buf->offset);

   if( buf->offset + sizeof(bitfield_t) + field->length >= buf->size )
      return NAN;
   if( field->type != BIT_FLOAT )
      return NAN;

   buf->offset += sizeof(*field) + sizeof(float);
   return *(float*)((uintptr_t)field + sizeof(bitfield_t));
}

BIT_READ( double, Double )
{
   bitfield_t* field = (bitfield_t*)(buf->buffer + buf->offset);

   if( buf->offset + sizeof(bitfield_t) + field->length >= buf->size )
      return NAN;
   if( field->type != BIT_DOUBLE )
      return NAN;

   buf->offset += sizeof(*field) + sizeof(double);
   return *(double*)((uintptr_t)field + sizeof(bitfield_t));
}

BIT_READ( char*, String )
{
   bitfield_t* field = (bitfield_t*)(buf->buffer + buf->offset);

   if( buf->offset + sizeof(bitfield_t) + field->length >= buf->size )
      return NULL;
   if( field->type != BIT_STRING )
      return NULL;

   buf->offset += sizeof(*field);
   char* string = (char*)(buf->buffer + buf->offset);
   buf->offset += field->length;
   return string;
}
