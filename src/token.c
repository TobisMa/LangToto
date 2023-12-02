#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

static const char KEYWORD_EXIT_LITERAL[] = "Exit";
static const char KEYWORD_SET_LITERAL[] = "set";
static const char KEYWORD_TO_LITERAL[] = "to";

enum TokenType getIdentifierToken(const char *identifier)
{
    if (strcmp(identifier, KEYWORD_EXIT_LITERAL) == 0)
    {
        return TOKEN_KEYWORD_EXIT;
    }
    else if (strcmp(identifier, KEYWORD_SET_LITERAL) == 0)
    {
        return TOKEN_KEYWORD_SET;
    }
    else if (strcmp(identifier, KEYWORD_TO_LITERAL) == 0)
    {
        return TOKEN_KEYWORD_TO;
    }
    return TOKEN_IDENTIFIER;
}
