#pragma once
#ifndef H_PARSER
#define H_PARSER

#include <stdio.h>
#include <stdlib.h>
#include "token.h"

struct parseinfo
{
    long position;
    int line, column, statements;
    int update;
};

struct TokenListNode
{
    struct Token token;
    struct TokenListNode *next;
};

struct TokenListNode *parse(const char *input, const long inputSize, int *statementCount);
char consumeChar(const char *input, struct parseinfo *ctx);
int hasNextChar(const char *input, const long inputSize, const struct parseinfo *ctx, const long offset);
char peekNextChar(const char *input, const struct parseinfo *ctx, const long offset);

void consumeIdentifier(const char *input, const long inputSize, struct parseinfo *ctx, struct Token *o_token);
void consumeDigit(const char *input, const long inputSize, struct parseinfo *ctx, struct Token *o_token);

void freeTokenList(struct TokenListNode *start);

#endif // H_PARSER
