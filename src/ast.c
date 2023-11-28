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
        return 1;
    }

    t = lookAhead(token, 2);
    if (t == NULL || t->type != TOKEN_STATEMENT_END)
    {
        return 2;
    }

    token = token->next;

    size_t strBytes = strlen(token->token.symbols) + 1;
    struct ExitStmt exit_stmt = {.exit_code = saveMalloc(sizeof(char) * strBytes)};
    memcpy(exit_stmt.exit_code, token->token.symbols, strBytes);

    struct ProgramStatement stmt = {.stmt_type = AST_STMT_EXIT, .exitStmt = exit_stmt};
    program->statements[program->count] = stmt;
    program->count++;
    return 0;
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
            if (root->statements[i].exitStmt.exit_code != NULL)
            {
                free(root->statements[i].exitStmt.exit_code);
            }
            break;

        default:
            printf("WARN: Unknown statement type (for now): %i\n", root->statements[i].stmt_type);
        }
    }
    printf("\n");
    free(root);
}
