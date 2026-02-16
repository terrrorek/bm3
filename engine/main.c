#include <stdio.h>
#include <video/vid.h>

bool running = true;

int main( void )
{
   printf( "Hello, World!\n" );

   Vid_Init( );

   while( running )
   {
      Vid_Update( );
   }

   Vid_Quit( );
   return 0;
}
