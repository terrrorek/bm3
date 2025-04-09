#include <common/cmd.h>
#include "scc.h"

const char* SCCGetDependentString( scc_t* scc, const char *name )
{
    int token = 0;

    // printf("%s\n", scc->point);
    while( (token = GetToken(scc)) != STK_UNKNOWN )
    {
        // printf("%s ?= %s (%u)\n", name, scc->decl, token);
        if( token == STK_STRING && !S_SafeCmp(name, scc->decl) )
            return scc->string;
    }
    return NULL;
}

float SCCGetDependentNumber( scc_t* scc, const char* name )
{
    int token = 0;

    while( (token = GetToken(scc)) != STK_UNKNOWN )
    {
        if( token == STK_NUMBER && !S_SafeCmp(name, scc->decl) )
            return scc->number;
    }
    return 0.f;
}

void SCCGetDependentVector( scc_t* scc, const char* name, scc_vector vec )
{
    int token = 0;

    while( (token = GetToken(scc)) != STK_UNKNOWN )
    {
        if( token == STK_VECTOR && !S_SafeCmp(name, scc->decl) )
        {
            VectorCopy(vec, scc->vector);
            return;
        }
    }
    VectorSet(vec, 0.f);
}

const char* SCCGetString( scc_t* scc, const char *name )
{
    scc->point = scc->start;
    return SCCGetDependentString(scc, name);
}

float SCCGetNumber( scc_t* scc, const char* name )
{
    scc->point = scc->start;
    return SCCGetDependentNumber(scc, name);
}

void SCCGetVector( scc_t* scc, const char* name, scc_vector vec )
{
    scc->point = scc->start;
    SCCGetDependentVector(scc, name, vec);
}

int SCCGotoNamedBody( scc_t* scc, const char* name )
{
    int token = 0;
    scc->point = scc->start + 1;

    while( (token = GetToken(scc)) != STK_UNKNOWN )
    {
        //printf("%s ?= %s\n", name, scc->string);
        if( token == STK_OPEN_BODY && !S_SafeCmp(name, scc->string) )
            return 0;
    }
    // Cmd_SoftErrorf( "failed to goto body \"%s\"\n", name );
    scc->point = scc->start;
    return -1;
}

int SCCGetDeclPosition( scc_t* scc, const char* name )
{
    int token = 0, i = 0;

    while( (token = GetToken(scc)) != STK_UNKNOWN )
        if( token == STK_DECL && !S_SafeCmp(name, scc->decl) )
            return i;
        else ++i;
    return -1;
}

int SCCGetStringPosition( scc_t* scc, const char* str )
{
    int token = 0, i = 0;

    while( (token = GetToken(scc)) != STK_UNKNOWN )
        if( token == STK_STRING && !S_SafeCmp(str, scc->string) )
            return i;
        else ++i;
    return -1;
}

void SCCResetPos( scc_t* scc )
{
    scc->point = scc->start;
}

extern int SCCCountToken( scc_t* scc, int token )
{
   int ntoken = 0, i = 0;

   while( (ntoken = GetToken(scc)) != STK_UNKNOWN )
      if( ntoken == token )
         ++i;
   return i;
}

extern int SCCGetTokenPosition( scc_t* scc, int token )
{
   int ntoken = 0, i = 0;

   while( (ntoken = GetToken(scc)) != STK_UNKNOWN )
      if( ntoken == token )
         return i;
      else ++i;
   return -1;
}

extern const char* SCCGetNamedBodyString( scc_t* scc, const char* body, const char* name )
{
   int token = 0;

   if( SCCGotoNamedBody(scc, body) == -1 )
   {
      //LOG(SCC, "failed to get body \"%s\" string \"%s\" (body not found)\n", body, name);
      return NULL;
   }

   const char* current_pos = scc->point;
   int body_end = SCCGetTokenPosition(scc, STK_CLOSE_BODY);

   if( body_end == -1 )
   {
      //LOG(SCC, "failed to get body \"%s\" string \"%s\" (body not closed)\n", body, name);
      return NULL;
   }

   scc->point = current_pos;
   int decl_pos = SCCGetDeclPosition(scc, name);

   if( decl_pos == -1 || decl_pos > body_end )
   {
      //LOG(SCC, "failed to get body \"%s\" string \"%s\" (decl not found in body)\n", body, name);
      return NULL;
   }

   scc->point = current_pos;;
   const char* str = SCCGetDependentString(scc, name);
   return str;
}

extern float SCCGetNamedBodyNumber( scc_t* scc, const char* body, const char* name )
{
   int token = 0;

   if( SCCGotoNamedBody(scc, body) == -1 )
   {
      //LOG(SCC, "failed to get body \"%s\" float \"%s\" (body not found)\n", body, name);
      return NAN;
   }

   const char* current_pos = scc->point;
   int body_end = SCCGetTokenPosition(scc, STK_CLOSE_BODY);

   if( body_end == -1 )
   {
      //LOG(SCC, "failed to get body \"%s\" float \"%s\" (body not closed)\n", body, name);
      return NAN;
   }

   scc->point = current_pos;
   int decl_pos = SCCGetDeclPosition(scc, name);

   if( decl_pos == -1 || decl_pos > body_end )
   {
      //LOG(SCC, "failed to get body \"%s\" float \"%s\" (decl not found in body)\n", body, name);
      return NAN;
   }

   scc->point = current_pos;
   float num = SCCGetDependentNumber(scc, name);
   return num;
}

extern void SCCGetNamedBodyVector( scc_t* scc, const char* body, const char* name, scc_vector vec )
{
   int token = 0;

   if( SCCGotoNamedBody(scc, body) == -1 )
   {
      //LOG(SCC, "failed to get body \"%s\" vector \"%s\" (body not found)\n", body, name);
      return;
   }

   const char* current_pos = scc->point;
   int body_end = SCCGetTokenPosition(scc, STK_CLOSE_BODY);

   if( body_end == -1 )
   {
      //LOG(SCC, "failed to get body \"%s\" vector \"%s\" (body not closed)\n", body, name);
      return;
   }

   scc->point = current_pos;
   int decl_pos = SCCGetDeclPosition(scc, name);

   if( decl_pos == -1 || decl_pos > body_end )
   {
      // LOG(SCC, "failed to get body \"%s\" vector \"%s\" (decl not found in body)\n", body, name);
      return;
   }

   scc->point = current_pos;
   SCCGetDependentVector(scc, name, vec);
}
