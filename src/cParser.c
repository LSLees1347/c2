#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cParser.h"

static token* tokens = NULL;
static int tCount = 0;
static int pos = 0;

static token* peek()
{
    if (pos < tCount) return &tokens[pos];
    return NULL;
}

static token* advance()
{
    if (pos < tCount) pos++;
    return &tokens[pos];
}

static void expect(tokenType type)
{
    if (!peek() || peek()->type != type)
    {
        printf("Syntax Error - %d - %d\n", type, peek() ? peek()->type : -1);
        exit(1);
    }
    advance();
}

astNode* makeNode(nodeType type, const char* value)
{
    astNode* node = malloc(sizeof(astNode));
    node->type = type;
    node->left = node->right = node->body = NULL;

    if (value)
    {
        strncpy(node->value, value, sizeof(node->value));
    }
    else
    {
        node->value[0] = '\0';
    }

    return node;
}

astNode* makeReturn(astNode* expr)
{
    astNode* node = makeNode(N_RETURN, NULL);
    node->body = expr;
    return node;
}

astNode* makeFunction(const char* name, astNode* body)
{
    astNode* node = makeNode(N_FUNCTION, name);
    node->body = body;
    return node;
}

void astPrint(astNode* node, int indent)
{
    if (!node) return;

    for (int i = 0; i < indent; i++) printf("  ");

    switch (node->type)
    {
        case N_FUNCTION:
            printf("FUNCTION: %s\n", node->value);
            if (node->body)
                astPrint(node->body, indent + 1);
            break;

        case N_RETURN:
            printf("Return\n");
            if (node->body)
                astPrint(node->body, indent + 1);
            break;

        case N_CONSTANT:
            printf("Constant: %s\n", node->value);
            break;

        case N_IDENTIFIER:
            printf("Identifier: %s\n", node->value);
            break;

        default:
            printf("Unknown node type\n");
            break;
    }
}

astNode* rParser(token* t, int c)
{
    tokens = t;
    tCount = c;
    pos = 0;

    expect(T_INT);

    if (peek()->type != T_IDENTIFIER)
    {
        printf("Syntax error - 'int'\n");
        return NULL;
    }

    char function[64];
    strncpy(function, peek()->value, sizeof(function));
    function[sizeof(function) - 1] = '\0';
    advance();

    expect(T_OPAR);
    expect(T_VOID);
    expect(T_CPAR);

    expect(T_OBRACE);

    expect(T_RETURN);

    if (peek()->type != T_CONSTANT)
    {
        printf("Syntax error - 'return'\n");
        return NULL;
    }

    astNode* nConstant = makeNode(N_CONSTANT, peek()->value);
    advance();

    expect(T_SCOLON);
    expect(T_CBRACE);

    astNode* nReturn = makeReturn(nConstant);
    astNode* nFunction = makeFunction(function, nReturn);
    
    return nFunction;
}