#pragma once
#include "cLexer.h"

typedef struct astNode astNode;

typedef enum
{
    N_FUNCTION,
    N_RETURN,
    N_CONSTANT,
    N_IDENTIFIER
} nodeType;

struct astNode
{
    nodeType type;
    char value[64];
    astNode* left;
    astNode* right;
    astNode* body;
};

astNode* makeNode(nodeType type, const char* value);
astNode* makeReturn(astNode* expr);
astNode* makeFunction(const char* name, astNode* body);
void astPrint(astNode* node, int indent);
astNode* rParser(token* t, int c);