#pragma once
#define tokenCap 512

typedef enum
{
    T_IDENTIFIER,
    T_CONSTANT,
    T_INT,
    T_RETURN,
    T_OPAR,
    T_CPAR,
    T_OBRACE,
    T_CBRACE,
    T_SCOLON,
    T_VOID,
    T_END
} tokenType;

typedef struct
{
    tokenType type;
    char value[64];
} token;

token* rLexer(const char* file, int* count);