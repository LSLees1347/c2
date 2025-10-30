#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "cDebug.h"
#include "cLexer.h"

extern unsigned char flags;

static token tokens[tokenCap];
static int tokenCount = 0;

static void addToken(tokenType type, const char* value)
{
    if (tokenCount >= tokenCap)
    {
        printf("\nTOKEN CAP HIT\n");
        return;
    }

    tokens[tokenCount].type = type;

    if (value)
    {
        strncpy(tokens[tokenCount].value, value, sizeof(tokens[tokenCount].value) - 1);
        tokens[tokenCount].value[sizeof(tokens[tokenCount].value) - 1] = '\0';
    }
    else
    {
        tokens[tokenCount].value[0] = '\0';
    }

    tokenCount++;
}

static void character(FILE* f, int c)
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
        if (flags & fDEBUG) printf(" - T_INT\n");
        addToken(T_INT, NULL);
    }
    else if (strcmp(buffer, "return") == 0)
    {
        if (flags & fDEBUG) printf(" - T_RETURN\n");
        addToken(T_RETURN, NULL);
    }
    else if (strcmp(buffer, "void") == 0)
    {
        if (flags &fDEBUG) printf(" - T_VOID\n");
        addToken(T_VOID, NULL);
    }
    else
    {
        addToken(T_IDENTIFIER, buffer);
        if (flags & fDEBUG) printf(" - T_IDENTIFIER (%s)\n", buffer);
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
        printf(" - T_CONSTANT (%s)\n", buffer);
    }

    addToken(T_CONSTANT, buffer);

    // dont overread
    if (c != EOF) ungetc(c, f);
}

token* rLexer(const char* file, int* count)
{
    FILE* f = fopen(file, "r");
    int c;

    tokenCount = 0;

    if (!f)
    {
        printf("FAILED TO OPEN PREPROC\n");
        return NULL;
    }

    if (flags & fDEBUG)
    {
        printf("\nLEXED TOKENS: \n");
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

        if (isspace(c)) continue;
        if (isalpha(c) || c == '_') character(f, c);
        else if (isdigit(c)) digit(f, c);
        else
        {
            switch (c)
            {
                case '(':
                {
                    addToken(T_OPAR, NULL);
                    printf(" - T_OPAR\n");
                    break;
                }

                case ')':
                {
                    addToken(T_CPAR, NULL);
                    printf(" - T_CPAR\n");
                    break;
                }

                case '{':
                {
                    addToken(T_OBRACE, NULL);
                    printf(" - T_OBRACE\n");
                    break;
                }

                case '}':
                {
                    addToken(T_CBRACE, NULL);
                    printf(" - T_CBRACE\n");
                    break;
                }

                case ';':
                {
                    addToken(T_SCOLON, NULL);
                    printf(" - T_SCOLON\n");
                    break;
                }

                default: printf("\nINVALID SYNTAX\n");
            }
        }
    }

    fclose(f);
    addToken(T_END, NULL);
    if (count) *count = tokenCount;
    return tokens;
}