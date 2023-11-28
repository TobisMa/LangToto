#pragma once
#ifndef H_CHARTYPE
#define H_CHARTYPE 


enum CHAR_TYPE {
    CH_DIGIT,
    CH_LOWER_ASCII,
    CH_UPPER_ASCII,
    CH_NEWLINE,
    CH_SPACE,
    CH_TERMINATOR,
    CH_OTHER
};

enum CHAR_TYPE getCharType(const char c);

#endif
