
#include <common/args.h>
#include <common/cmd.h>

int      ArgCount = 0;
char**   ArgStrings = NULL;

void Cmd_InitArgs( int argc, char** argv )
{
   ArgCount = argc;
   ArgStrings = argv;
}

char* Cmd_GetStringArg( const char* name )
{
   int i = 0;
   char* value = NULL;

   do
   {
      if( !BM_strcmp( name, ArgStrings[i] ) )
      {
         value = ArgStrings[i+1];
         break;
      }
      ++i;
   }
   while( i < ArgCount );

   return value;
}

int Cmd_GetIntArg( const char* name )
{
   int i = 0;
   int value = 0;

   do
   {
      if( !BM_strcmp( name, ArgStrings[i] ) )
      {
         value = atoi( ArgStrings[i+1] );
         break;
      }
      ++i;
   }
   while( i < ArgCount );

   return value;
}

bool Cmd_IsVarPresent( const char* name )
{
   int i = 0;

   do
   {
      if( !BM_strcmp( name, ArgStrings[i] ) )
         return TRUE;
      ++i;
   }
   while( i < ArgCount );

   return FALSE;
}
