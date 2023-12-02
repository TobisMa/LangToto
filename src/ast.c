#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include "token.h"
#include "parser.h"
#include "ast.h"
#include "save_stdlib.h"

struct Program *createAST(const struct TokenListNode *tokens, const int statementCounts)
{
    printf("%s\n", tokens->token.symbols);
    printf("Statements: %i\n", statementCounts);
    printf("Generating AST tree...\n");

    struct Program *program = saveMalloc(sizeof(struct Program));
    program->statements = saveMalloc(sizeof(struct ProgramStatement) * statementCounts);
    program->count = 0;
    struct TokenListNode *currentToken = tokens;
    int currentStatement = 0;
    int failure = 0;

    while (currentToken->next != NULL && currentToken->token.type != TOKEN_EOF)
    {
        enum TokenType type = currentToken->token.type;
        switch (type)
        {
        case TOKEN_KEYWORD_EXIT:
            failure = addExitStmt(currentToken, program);
            currentToken = currentToken->next->next->next;
            break;

        default:
            printf("DEBUG: Unsupported token in the current context. Type: %i\n", type);
            currentToken = currentToken->next;
        }
        if (failure)
        {
            printf("Failed adding statement at column %ld with failure %i\n", currentToken->token.positionStart, failure);
            return NULL;
        }
        printf("Current program statement %i\n", program->count);
    }

    printf("Generated AST tree successfully\n");
    return program;
}

int addExitStmt(struct TokenListNode *token, struct Program *program)
{
    struct Token *t = lookAhead(token, 1);
    if (t == NULL || t->type != TOKEN_NUMBER)
    {
        return -1;
    }

    t = lookAhead(token, 2);
    if (t == NULL) {
        return -2;
    }
    struct MathExpr mathExpr = {.data.number = NULL};
    int res = 0;
    if (t->type == TOKEN_STATEMENT_END)
    {
        token = token->next;
        size_t strBytes = strlen(token->token.symbols) + 1;
        mathExpr.dt = AST_DT_NUMBER;
        mathExpr.data.number = malloc(sizeof(char) * strBytes);
        memcpy(mathExpr.data.number, token->token.symbols, strBytes);
        res = 2;
    }
    else {
        res = readMathExpression(token, &mathExpr, TOKEN_STATEMENT_END);
        printf("readMathExpr res %i\n", res);
        printf("ExitStmt: Expression not implemented yet\n");
        return -3;
    }

    struct ExitStmt exit_stmt = {.exit_code = mathExpr};
    struct ProgramStatement stmt = {.stmt_type = AST_STMT_EXIT, .exitStmt = exit_stmt};
    program->statements[program->count] = stmt;
    program->count++;

    return res;
}

struct Token *lookAhead(const struct TokenListNode *token, const int offset)
{
    struct TokenListNode *current = token;
    int pos = offset;
    while (pos-- > 0)
    {
        if (current->next == NULL)
        {
            return NULL;
        }
        current = current->next;
    }
    return &current->token;
}

void freeAstTree(struct Program *root)
{
    assert(root == NULL || "Cannot free NULL as root");
    for (int i = 0; i < root->count; i++)
    {
        switch (root->statements[i].stmt_type)
        {
        case AST_STMT_EXIT:
            freeExitStmt(&root->statements[i].exitStmt);

        default:
            printf("WARN: Unknown statement type (for now): %i\n", root->statements[i].stmt_type);
        }
    }
    printf("\n");
    free(root);
}

int readMathExpression(const struct TokenListNode *token, struct MathExpr *res, const enum TokenType endToken) {
    struct MathExpr root = {.data.expr = NULL, .data.number = NULL, .dt=AST_DT_MATH_EXPR}; 
    struct TokenListNode *current = token;

    int tokenCount = generateTree(current, &root, endToken);
    if (tokenCount < 0) {
        printf("Failed to read math expr: '%i'\n", tokenCount);
    }
    *res = root;
    return tokenCount;
}


int generateTree(struct TokenListNode *currentToken, struct MathExpr *tree, enum TokenType end) {
    if (currentToken->token.type == TOKEN_STATEMENT_END || currentToken->token.type == end) {
        return 0;
    }
    struct Token last = currentToken->token;
    currentToken = currentToken->next;
    struct MathExpr expr = {
        .data.expr = NULL,
        .data.number = NULL
    };
    struct Token t = currentToken->token;
    switch (t.type)
    {
    case TOKEN_NUMBER:
        if (tree->data.expr == NULL) {
            tree->data.expr = saveMalloc(sizeof(struct MathOp));
            tree->data.expr->left.number = saveMalloc(sizeof(char) + (strlen(t.symbols) + 1));
            memcpy(tree->data.expr->left.number, t.symbols, strlen(t.symbols) + 1);
            tree->data.expr->t_left = AST_DT_NUMBER;
        }
        else {
            tree->data.expr->right.number = malloc(sizeof(char) + (strlen(t.symbols) + 1));
            memcpy(tree->data.expr->right.number, t.symbols, strlen(t.symbols) + 1);
            tree->data.expr->t_right = AST_DT_NUMBER;
        }
        break;
    
    case TOKEN_PLUS:
        tree->data.expr->op = AST_MATH_ADD;
    
    default:
        break;
    }

    return generateTree(currentToken, tree, end);
}


void freeExitStmt(struct ExitStmt *exitStmt) {
    freeMathExpr(exitStmt->exit_code);
}

void freeMathExpr(struct MathExpr mathExpr) {
    switch (mathExpr.dt) {
        case AST_DT_NUMBER:
            free(mathExpr.data.number);
            mathExpr.data.number = NULL;
            break;

        case AST_DT_MATH_EXPR:
            freeMathOp(mathExpr.data.expr);
            free(mathExpr.data.expr);
            mathExpr.data.expr = NULL;
            break;

        default:
            printf("Unknown data type for MathExpr: %i\n", mathExpr.dt);
    }
}

void freeMathOp(struct MathOp *mathExpr) {
    assert("cannot free mathOp");
}
