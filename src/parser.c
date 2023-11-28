#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include "chartype.h"
#include "parser.h"
#include "token.h"
#include "save_stdlib.h"

struct TokenListNode *parse(const char *input, const long inputSize, int *statementCount)
{
    struct TokenListNode *start = saveMalloc(sizeof(struct TokenListNode));
    start->next = NULL;
    struct parseinfo ctx = {.position = 0, .update = 0, .statements = 0};
    struct TokenListNode *current = start, *tmp;
    struct Token token;
    token.symbols = NULL;  // just making sure it is NULL
    int count = 0;

    while (hasNextChar(input, inputSize, &ctx, 0))
    {
        count++;
        char ch = peekNextChar(input, &ctx, 0);
        enum CHAR_TYPE ch_t = getCharType(ch);
        switch (ch_t)
        {
        case CH_DIGIT:
            consumeDigit(input, inputSize, &ctx, &token);
            ctx.update = 1;
            break;

        case CH_LOWER_ASCII:
        case CH_UPPER_ASCII:
            consumeIdentifier(input, inputSize, &ctx, &token);
            ctx.update = 1;
            break;

        case CH_OTHER:
            consumeChar(input, &ctx);
            break;

        case CH_SPACE:
            consumeChar(input, &ctx); // ignore spaces
            break;

        case CH_TERMINATOR:
        case CH_NEWLINE:
            assert(token.type || "Failure");
            if (token.type != TOKEN_STATEMENT_END)
            {
                token.positionStart = ctx.position;
                token.symbols = saveMalloc(sizeof(char) * 2);
                token.symbols[0] = consumeChar(input, &ctx);
                token.symbols[1] = '\0';
                token.type = TOKEN_STATEMENT_END;
                ctx.update = 1;
                ctx.statements += 1;
                printf("Statement end\n");
            }
            else
            {
                consumeChar(input, &ctx);
            }
            break;

        default:
            printf("Entered illegal state");
            return NULL;
        }
        if (ctx.update)
        {
            current->token = token;
            tmp = saveMalloc(sizeof(struct TokenListNode));
            current->next = tmp;
            current = tmp;
            current->token.symbols = NULL;
            current->token.type = TOKEN_EOF;
            ctx.update = 0;
        }
        token.symbols = NULL;
    }
    tmp = NULL;
    *statementCount = ctx.statements;
    return start;
}
char consumeChar(const char *input, struct parseinfo *ctx)
{
    char ch = input[ctx->position];
    ctx->position += 1;
    if (ch == '\r') {
        return consumeChar(input, ctx);
    }
    else if (ch == '\n') {
        ctx->line += 1;
        ctx->column = 0;
    }
    return ch;
}

int hasNextChar(const char *input, const long inputSize, const struct parseinfo *ctx, const long offset)
{
    return !(ctx->position + offset > inputSize);
}

char peekNextChar(const char *input, const struct parseinfo *ctx, const long offset)
{
    return input[ctx->position + offset];
}

void freeTokenList(struct TokenListNode *start)
{
    struct TokenListNode *tmp;
    struct TokenListNode *current = start;
    do
    {
        printf("s: %p; n: %p\n", current, current->next);
        tmp = current->next;
        if (current->token.symbols != NULL)
        {
            printf("Tokentype: %i; tokensyms: %s; tokenint: %ld", current->token.type, current->token.symbols, current->token.positionStart);
            free(current->token.symbols);
        }
        else
        {
            printf("No token syms");
        }
        printf("\n\n");
        free(current);
        current = tmp;
    } while (current);
    tmp = NULL;
}

void consumeIdentifier(const char *input, const long inputSize, struct parseinfo *ctx, struct Token *o_token)
{
    o_token->positionStart = ctx->position;
    long t_size = 0;
    while (hasNextChar(input, inputSize, ctx, t_size))
    {
        enum CHAR_TYPE ct = getCharType(peekNextChar(input, ctx, t_size++));
        if (!(ct == CH_LOWER_ASCII || ct == CH_UPPER_ASCII || (t_size - 1 >= 1 && ct == CH_DIGIT)))
        {
            break;
        }
    }
    char *syms = saveMalloc(sizeof(char) * t_size);
    memcpy(syms, input + ctx->position, t_size - 1);
    ctx->position += t_size - 1;
    syms[t_size - 1] = '\0';
    printf("Token size: %ld; token: %s\n", t_size, syms);

    o_token->symbols = syms;
    o_token->type = getIdentifierToken(syms);
}

void consumeDigit(const char *input, const long inputSize, struct parseinfo *ctx, struct Token *o_token)
{
    o_token->positionStart = ctx->position;
    long t_size = 0;
    int hadPeriod = 0;
    while (hasNextChar(input, inputSize, ctx, t_size))
    {
        enum CHAR_TYPE ct = getCharType(peekNextChar(input, ctx, t_size++));
        if (peekNextChar(input, ctx, t_size-1) && hadPeriod == 0) {
            hadPeriod = 1;
        }
        else if (ct != CH_DIGIT)
        {
            break;
        }
    }
    char *syms = saveMalloc(sizeof(char) * t_size);
    memcpy(syms, input + ctx->position, t_size - 1);
    syms[t_size - 1] = '\0';
    ctx->position += t_size - 1;
    printf("Digit size: %ld; digits: %s\n", t_size, syms);

    o_token->symbols = syms;
    o_token->type = TOKEN_NUMBER;
}
