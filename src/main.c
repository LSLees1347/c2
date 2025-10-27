#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>

#include "lexer.c"



int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "invalid args\n");
        return 1;
    }

    const char* ogsource = argv[1];
    const char* preproc = "output\\preproc.i";

    if (_mkdir("output") != 0 && errno != EEXIST)
        return 1;

    // run preproc
    char command[512];
    snprintf(command, sizeof(command), "gcc -E %s -o %s", ogsource, preproc);
    int scall = system(command);

    if (scall != 0)
    {
        fprintf(stderr, "preproc failed: %d\n", scall);
        return 1;
    }

    printf("output: %s\n", preproc);

    return 0;
}