#include <stdio.h>
#include <ctype.h>
#include <string.h>


void chararcter(FILE* f, int c)
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
        printf("kw_int\n");
    }
    else if (strcmp(buffer, "return") == 0)
    {
        printf("kw_ret\n");
    }
    else
    {
        printf("kw_identifier\n");
    }

    //dont overread
    if (c != EOF)
    {
        ungetc(c, f);
    }
}

void digit(FILE* f, int c)
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

    printf("kw_constant (%s) \n", buffer);

    // dont overread
    if (c != EOF)
    {
        ungetc(c, f);
    }
}

int rLexer(const char* file)
{
    FILE* f = fopen(file, "r");
    int c;

    if (f)
    {
        while ((c = fgetc(f)) != EOF)
        {
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
                case '(':
                {
                    printf("kw_oPar\n");
                    break;
                }
                case ')':
                {
                    printf("kw_cPar\n");
                    break;
                }
                case '{':
                {
                    printf("kw_oBrace\n");
                    break;
                }
                case '}':
                {
                    printf("kw_cBrace\n");
                    break;
                }
                case ';':
                {
                    printf("kw_sColon\n");
                    break;
                }
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
                        printf("invalid regex\n");
                    }
                }
            }
        }
    }
    else
    {
        printf("failed to open preproc");
        return 1;
    }

    fclose(f);
    return 0;
}