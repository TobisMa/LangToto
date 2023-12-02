#pragma once
#ifndef H_AST

#include "parser.h"

enum AST_STATMENTS
{
    AST_STMT_EXIT // 0
};

enum AST_MATH_OPERATION
{
    AST_MATH_ADD, // 0
    AST_MATH_SUB, // 1
    AST_MATH_MUL, // 2
    AST_MATH_DIV, // 3
};

enum AST_CONTAINED_DATA_TYPE
{
    AST_DT_CHARACTERS,  // 0
    AST_DT_NUMBER,      // 1
    AST_DT_MATH_EXPR    // 2
};

struct MathOp;

union MathOptions {
    char* number;
    struct MathOp* expr;
};

struct MathOp {
    enum AST_MATH_OPERATION op;

    enum AST_CONTAINED_DATA_TYPE t_left;
    enum AST_CONTAINED_DATA_TYPE t_right;

    union MathOptions left;
    union MathOptions right;
};

struct MathExpr {
    enum AST_CONTAINED_DATA_TYPE dt;
    union MathOptions data;
};

struct ExitStmt
{
    struct MathExpr exit_code;
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
int readMathExpression(const struct TokenListNode *token, struct MathExpr *res, const enum TokenType endToken);
int generateTree(struct TokenListNode *currentToken, struct MathExpr *tree, enum TokenType end);

void freeAstTree(struct Program *root);
void freeExitStmt(struct ExitStmt *exitStmt);
void freeMathOp(struct MathOp *mathExpr);
void freeMathExpr(struct MathExpr mathExpr);

#endif // H_AST
