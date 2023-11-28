#pragma once
#ifndef H_AST

#include "parser.h"

enum AST_STATMENTS
{
    AST_STMT_EXIT  // 0
};

struct ExitStmt
{
    char *exit_code;
};

struct ProgramStatement
{
    enum AST_STATMENTS stmt_type;
    union
    {
        struct ExitStmt exitStmt;
    };
};

struct Program
{
    struct ProgramStatement *statements;
    int count;
};

struct Program *createAST(const struct TokenListNode *tokenListStart, const int statementCount);
struct Token *lookAhead(const struct TokenListNode *token, const int offset);

int addExitStmt(struct TokenListNode *token, struct Program *program);

void freeAstTree(struct Program *root);

#endif // H_AST
