#pragma once
#ifndef H_TOKEN
#define H_TOKEN

enum TokenType
{
    TOKEN_KEYWORD_EXIT,     // 0
    TOKEN_KEYWORD_SET,      // 1
    TOKEN_KEYWORD_TO,       // 2
    TOKEN_IDENTIFIER,       // 3
    TOKEN_NUMBER,           // 4
    TOKEN_STATEMENT_END,    // 5
    TOKEN_EOF,              // 6
    TOKEN_PLUS,             // 7
    TOKEN_MINUS,            // 8
    TOKEN_EQUALS,           // 9
    TOKEN_MUL,              // 10

};

struct Token
{
    char *symbols;
    long positionStart;
    enum TokenType type;
};

enum TokenType getIdentifierToken(const char *identifier);

#endif // H_TOKEN
