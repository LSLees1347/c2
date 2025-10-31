#include <stdio.h>
#include "cGen.h"

static void genNode(FILE* out, astNode* node)
{
    if (!node) return;

    switch (node->type)
    {
        case N_FUNCTION:
        {
            fprintf(out, "    .globl %s\n", node->value);
            fprintf(out, "%s:\n", node->value);
            genNode(out, node->body);
            break;
        }
        case N_RETURN:
        {
            genNode(out, node->body);
            fprintf(out, "    ret\n");
            break;
        }
        case N_CONSTANT:
        {
            fprintf(out, "    movl $%s, %%eax\n", node->value);
            break;
        }
        default:
        {
            fprintf(out, "    # unknown node %d\n", node->type);
            break;
        }
    }
}

void rCodeGen(astNode* root, const char* file)
{
    FILE* out = fopen(file, "w");
    if (!out)
    {
        printf("OUTPUT FAILED: %s\n", file);
        return;
    }

    genNode(out, root);
    fclose(out);
}