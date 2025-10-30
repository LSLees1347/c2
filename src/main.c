#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>

#include "cDebug.h"
#include "cLexer.h"
#include "cParser.h"

unsigned char flags = 0;

int rPreproc(const char* s, const char* pp)
{
    if (_mkdir("output") != 0 && errno != EEXIST) return 1;

    char command[512];
    snprintf(command, sizeof(command), "gcc -E %s -o %s", s, pp);
    int call = system(command);

    if (call != 0)
    {
        printf("\nPREPROC FAILED - %d\n", call);
        return 1;
    }

    if (flags & fDEBUG) printf("\nPREPROC: \n - %s\n\n", pp);

    return 0;
}

void freeAst(astNode* node)
{
    if (!node) return;
    freeAst(node->left);
    freeAst(node->right);
    freeAst(node->body);
    free(node);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("NULL SOURCE\n");
        return 1;
    }

    if (argc >= 3 && strcmp(argv[2], "-d") == 0)
    {
        flags |= fDEBUG;
    }

    const char* ogsource = argv[1];
    const char* preproc = "output\\preproc.i";

    if (rPreproc(ogsource, preproc)) return 1;

    int tCount;
    token* lexed = rLexer(preproc, &tCount);
    if (!lexed) return 1;

    printf("\n");
    astNode* root = rParser(lexed, tCount);
    if (!root) return 1;
    if (flags & fDEBUG)
    {
        printf("\nROOT -\n");
        astPrint(root, 0);
    }

    freeAst(root);

    return 0;
}