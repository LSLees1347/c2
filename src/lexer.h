#define tokenCap 2048

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
    T_END
} tokenType;

typedef struct
{
    tokenType type;
    char value[64];
} token;

extern token tokens[tokenCap];
extern int tokenCount;

token* rLexer(const char* file);