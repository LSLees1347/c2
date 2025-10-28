#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>

#include "lexer.h"

#define fDEBUG (1 << 0)


unsigned char flags = 0;

int rPreproc(const char* s, const char* pp)
{
    if (_mkdir("output") != 0 && errno != EEXIST)
    {
        return 1;
    }

    char command[512];
    snprintf(command, sizeof(command), "gcc -E %s -o %s", s, pp);
    int call = system(command);

    if (call != 0)
    {
        printf("preproc failed: %d\n", call);
        return 1;
    }

    if (flags & fDEBUG)
    {
        printf("preproc output: %s\n", pp);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("invalid args\n");
        return 1;
    }

    if (argc >= 3 && strcmp(argv[2], "-d") == 0)
    {
        flags |= fDEBUG;
    }

    const char* ogsource = argv[1];
    const char* preproc = "output\\preproc.i";

    if (rPreproc(ogsource, preproc))
    {
        return 1;
    }

    if (rLexer(preproc))
    {
        return 1;
    }

    return 0;
}