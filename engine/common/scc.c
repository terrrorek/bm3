#include "scc.h"

static sccalloc_t   salloc = malloc;
static sccfree_t    sfree = free;
static sccrealloc_t srealloc = realloc;

// #define SCC_DEBUG
#define NOT_END ( ptr <= scc->end )
#define NOT_STREND(__ptr__) ( __ptr__ < (scc->str_storage + scc->str_size) )
#ifdef SCC_DEBUG
    #define DEBUG(...) printf( "SCC: " __VA_ARGS__ )
#else
    #define DEBUG(...)
#endif

int S_CountChars( const char* str, char del )
{
    char *p = (char*) str;
    int c = 0;
    while( *p )
        if( *p == del )
            c++, p++;
    return c;
}

char S_IsWhitespace( char del )
{
    if( del == ' ' || del == '\t' || del == '\r' || del == '\n' )
        return 1;
    else return 0;
}

void SCCFreeStrStorage( scc_t* scc )
{
    scc->str_size = 0, scc->str_pointer = 0;
    if( scc->str_storage )
    {
        sfree(scc->str_storage);
        scc->str_storage = 0;
    }
    SCCResizeStrStorage(scc, 64);
    scc->str_pointer = scc->str_storage;
}

void InitSCC( scc_t* scc, const char* source )
{
    memset(scc, 0, sizeof(*scc));
    scc->start = scc->point = (char*) source;
    scc->end = (char*)( source + strlen(source) + 1 );
    SCCFreeStrStorage(scc);
}

void FreeSCC( scc_t* scc )
{
    scc->point = scc->start;
    SCCFreeTokens(scc);
    SCCFreeStrStorage(scc);
}

void SCCResizeStrStorage( scc_t* scc, int new_size )
{
    if( new_size >= SCC_MAX_STORAGESIZE )
    {
        if( scc->str_storage )
            sfree(scc->str_storage);
        scc->str_size = SCC_MAX_STORAGESIZE - new_size + 64;
        scc->str_storage = scc->str_pointer = salloc(scc->str_size);
        scc->decl = scc->string = 0;
    }
    else
    {
        int ptr_offset = scc->str_pointer - scc->str_storage;
        int decl_offset = scc->decl - scc->str_storage;
        int str_offset = scc->string - scc->str_storage;

        scc->str_size = new_size;
        scc->str_storage = srealloc(scc->str_storage, new_size);
        scc->str_pointer = scc->str_storage + ptr_offset;

        if( !scc->str_storage )
           abort();

        if( scc->string )
           scc->string = scc->str_storage + str_offset;
        if( scc->decl )
           scc->decl = scc->str_storage + decl_offset;
    }

    char a = (scc->str_pointer < scc->str_storage), b = (scc->str_pointer > (scc->str_storage + scc->str_size));
    if( a || b )
       scc->str_pointer = scc->str_storage;
}

int SCCAddToken( scc_t* scc, scc_token_e token )
{
    if( scc->tokens )
    {
       ++scc->token_amount;
       scc->tokens = srealloc(scc->tokens, sizeof(token) * scc->token_amount);
       scc->tokens[scc->token_amount-1] = token;
    }
    return token;
}

void SetAllocFuncs( sccalloc_t alloc, sccfree_t free, sccrealloc_t realloc )
{
    salloc = alloc, sfree = free, srealloc = realloc;
}

char* ExtractString( scc_t* scc, char* point )
{
    char* ptr = ++point;
    int length = 0;

    while( *ptr != '\"' && NOT_END )
        ++length, ++ptr;
    scc->point = ++ptr;
    ptr = point;

    if( length <= 0 )
    {
        printf("failed to extract string: length is invalid\n");
        return NULL;
    }

    unsigned int new_size = scc->str_size + length + 2;
    SCCResizeStrStorage(scc, new_size);

    char* str = scc->str_pointer;
    while( *ptr != '\"' && NOT_END && NOT_STREND(scc->str_pointer) )
        *scc->str_pointer++ = *ptr++;
    *scc->str_pointer++ = 0;
    // printf("%s\n", str);
    return str;
}

char* ExtractDecl( scc_t* scc, char* point )
{
#define DECL_POINTS(p) ( *p == ':' || *p == '=' )

    char* ptr = point;
    int length = 0;

    while( !DECL_POINTS(ptr) && NOT_END )
        ++length, ++ptr;
    scc->point = ++ptr;
    ptr = point;

    if( length <= 0 )
    {
        printf("failed to extract decl: length is invalid\n");
        return NULL;
    }

    unsigned int new_size = scc->str_size + length + 1;
    SCCResizeStrStorage(scc, new_size);

    char* str = scc->str_pointer;
    while( !DECL_POINTS(ptr) && NOT_END && NOT_STREND(scc->str_pointer))
        *scc->str_pointer++ = *ptr++;
    *scc->str_pointer++ = 0;
    // HexDump(str, length + 1);
    return str;

#undef DECL_POINTS
}

float ExtractNum( scc_t* scc, char* point )
{
    char* ptr = point;
    int length = 0;

    while( !S_IsWhitespace(*ptr) && NOT_END && NOT_STREND(scc->str_pointer))
        ++length, ++ptr;
    scc->point = ++ptr;
    ptr = point;

    if( length <= 0 )
    {
        printf("failed to extract number: length is invalid\n");
        return 0.f;
    }

    unsigned int new_size = scc->str_size + length + 2;
    SCCResizeStrStorage(scc, new_size);

    char* str = scc->str_pointer;
    while( !S_IsWhitespace(*ptr) && NOT_END )
        *scc->str_pointer++ = *ptr++;
    *scc->str_pointer++ = 0;
    // HexDump(str, length + 1);
    return atof(str);
}

void ExtractVec( scc_t* scc, char* point )
{
    char* ptr = ++point;
    int length = 0;

    while( *ptr != '>' && NOT_END )
        ++length, ++ptr;
    scc->point = ++ptr;
    ptr = point;

    if( length <= 0 )
    {
        printf("failed to extract vector: length is invalid\n");
        return;
    }

    unsigned int new_size = scc->str_size + length + 4;
    SCCResizeStrStorage(scc, new_size);

    char* str = scc->str_pointer;
    while( *ptr != '>' && NOT_END && NOT_STREND(scc->str_pointer) )
        *scc->str_pointer++ = *ptr++;
    *scc->str_pointer++ = 0;
    // HexDump(str, length + 1);
    (void)sscanf(str, "%f %f %f", &scc->vector[0], &scc->vector[1], &scc->vector[2]);
}

int S_SafeCmp( const char* s1, const char* s2 )
{
    if (s1 && s2)
        return strcmp(s1, s2);
    return -1;
}

void S_NLPrint( const char* str )
{
    char* p = (char*) str;
    while( *p != '\n' )
    {
       putc(*p, stdout);
       ++p;
    }
}

int GetToken( scc_t* scc )
{
    #define SKIP_WHITESPACE while( S_IsWhitespace(*ptr) ) ++ptr

    if( scc->point >= scc->end )
        scc->point = scc->start;
    char* ptr = scc->point;

    while( NOT_END )
    {
        // whitespace
        SKIP_WHITESPACE;

        // comments
        if( *ptr == ';' )
        {
        #ifdef SCC_DEBUG
            printf("SCC: comment ");
            S_NLPrint(ptr);
            printf("\n");
        #endif
            while( *ptr != '\n' && NOT_END )
                ++ptr;
            scc->point = ++ptr;
            return SCCAddToken(scc, STK_COMMENT);
        }

        // bodies
        if( *ptr == '{' )
        {
            scc->point = ++ptr;
            return SCCAddToken(scc, STK_OPEN_BODY);
        }
        if( *ptr == '}' )
        {
            scc->point = ++ptr;
            return SCCAddToken(scc, STK_CLOSE_BODY);
        }

        // strings
        if( *ptr == '\"' )
        {
            SKIP_WHITESPACE;
            scc->string = ExtractString(scc, ptr);
            DEBUG("\tstr \"%s\"\n", scc->string);
            return SCCAddToken(scc, STK_STRING);
        }

        // numbers
        if( *ptr >= '0' && *ptr <= '9' )
        {
            SKIP_WHITESPACE;
            scc->number = ExtractNum(scc, ptr);
            DEBUG("\tnum %.4f\n", scc->number);
            return SCCAddToken(scc, STK_NUMBER);
        }

        // vectors
        if( *ptr == '<' )
        {
            ExtractVec(scc, ptr);
            DEBUG("\tvec <%f %f %f>\n", scc->vector[0], scc->vector[1],
                    scc->vector[2]);
            return SCCAddToken(scc, STK_VECTOR);
        }

        // declarations
        if( *ptr >= 'A' && *ptr <= 'z' )
        {
            SKIP_WHITESPACE;
            scc->decl = ExtractDecl(scc, ptr);
            DEBUG("decl \"%s\"\n", scc->decl);
            return SCCAddToken(scc, STK_DECL);
        }

        break;
    }
    return STK_UNKNOWN;

    #undef SKIP_WHITESPACE
}

void SCCFreeTokens( scc_t* scc )
{
    scc->token_amount = 0;
    if( scc->tokens )
    {
        sfree(scc->tokens);
        scc->tokens = 0;
    }
}
