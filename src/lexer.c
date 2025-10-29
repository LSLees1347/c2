#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "debug.h"
#include "lexer.h"

extern unsigned char flags;

token tokens[tokenCap];
int tokenCount = 0;


static void addToken(tokenType type, const char* value)
{
    if (tokenCount >= tokenCap)
    {
        printf("token cap hit");
        return;
    }

    tokens[tokenCount].type = type;

    if (value)
    {
        strncpy(tokens[tokenCount].value, value, sizeof(tokens[tokenCount].value));
    }
    else
    {
        tokens[tokenCount].value[0] = '\0';
    }

    tokenCount++;
}

static void chararcter(FILE* f, int c)
{
    char buffer[64];
    int len = 0;

    // collect word
    buffer[len++] = c;
    while ((c = fgetc(f)) != EOF && (isalnum(c) || c == '_'))
    {
        buffer[len++] = c;
    }

    buffer[len] = '\0';

    if (strcmp(buffer, "int") == 0)
    {
        if (flags & fDEBUG)
        {
            printf("T_INT\n");
        }
        addToken(T_INT, NULL);
    }
    else if (strcmp(buffer, "return") == 0)
    {
        if (flags & fDEBUG)
        {
            printf("T_RETURN\n");
        }
        addToken(T_RETURN, NULL);
    }
    else
    {
        addToken(T_IDENTIFIER, buffer);
        if (flags & fDEBUG)
        {
            printf("T_IDENTIFIER\n");
        }
    }

    //dont overread
    if (c != EOF)
    {
        ungetc(c, f);
    }
}

static void digit(FILE* f, int c)
{
    char buffer[32];
    int len = 0;

    //collect word
    buffer[len++] = c;
    while ((c = fgetc(f)) != EOF && isdigit(c))
    {
        buffer[len++] = c;
    }

    buffer[len] = '\0';

    if (flags & fDEBUG)
    {
        printf("T_CONSTANT (%s) \n", buffer);
    }

    addToken(T_CONSTANT, buffer);

    // dont overread
    if (c != EOF)
    {
        ungetc(c, f);
    }
}

token* rLexer(const char* file)
{
    FILE* f = fopen(file, "r");
    int c;

    if (!f)
    {
        printf("failed to open preproc\n");
        return NULL;
    }

    while ((c = fgetc(f)) != EOF)
    {
        // rm preproc heading
        if (c == '#')
        {
            while ((c = fgetc(f)) != EOF && c != '\n')
            {
                continue;
            }
        }

        if (isspace(c))
        {
            continue;
        }

        switch (c)
        {
            case '(': addToken(T_OPAR, NULL); if (flags & fDEBUG) printf("T_OPAR\n"); break;
            case ')': addToken(T_CPAR, NULL); if (flags & fDEBUG) printf("T_CPAR\n"); break;
            case '{': addToken(T_OBRACE, NULL); if (flags & fDEBUG) printf("T_OBRACE\n"); break;
            case '}': addToken(T_CBRACE, NULL); if (flags & fDEBUG) printf("T_CBRACE\n"); break;
            case ';': addToken(T_SCOLON, NULL); if (flags & fDEBUG) printf("T_SCOLON\n"); break;
            default:
            {
                if (isalpha(c) || c == '_')
                {
                    chararcter(f, c);
                    break;
                }
                else if (isdigit(c))
                {
                    digit(f, c);
                    break;
                }
                else
                {
                    printf("invalid regex: '%c'\n", c);
                }
            }
        }
    }

    fclose(f);
    addToken(T_END, NULL);
    return tokens;
}