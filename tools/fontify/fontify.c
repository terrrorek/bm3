#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ShowHelp( void )
{
   printf( "fontify - add font metadata to a TGA\n" );
   printf( "\texample: fontify <file> -w <char width> -h <char height>\n" );
   printf( "\tparams:\n" );
   printf( "\t\t-w or -width  = character width\n" );
   printf( "\t\t-h or -height = character height\n" );
   printf( "\t\t-m or -margin = character margin\n" );
}

int   width = 0, height = 0, margin = 0;
char* file = NULL;

void ParseArgs( int argc, char** argv )
{
   int   i;

   file = argv[1];
   for( i = 2; i < argc; i++ )
   {
      if(( !strcmp( argv[i], "-w" ) || !strcmp( argv[i], "-width" )) && i+1 < argc)
         { width = atoi(argv[++i]); continue; }
      if(( !strcmp( argv[i], "-h" ) || !strcmp( argv[i], "-height" )) && i+1 < argc)
         { height = atoi(argv[++i]); continue; }
      if(( !strcmp( argv[i], "-m" ) || !strcmp( argv[i], "-margin" )) && i+1 < argc)
         { margin = atoi(argv[++i]); continue; }
   }
}

int main( int argc, char** argv )
{
   char*          error = "?";
   unsigned char  buffer[5];

   if( argc < 2 )
      goto help;

   ParseArgs( argc, argv );

   if( !width || !height )
      goto help;

   FILE* fp = fopen( file, "rb+" );
   if( !fp )
   {
      error = "file open failure";
      goto error;
   }

   memset( buffer, 0, sizeof(buffer) );
   buffer[0] = margin;
   buffer[1] = width;
   buffer[2] = height;

   fseek( fp, 3, SEEK_CUR );
   fwrite( buffer, 5, 1, fp );
   fclose( fp );

   return 0;
error:
   printf( "ERROR: %s\n", error );
   if( fp )
      fclose( fp );
   return 1;
help:
   ShowHelp( );
   return 1;
}
