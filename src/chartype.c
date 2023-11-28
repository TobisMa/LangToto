#include "chartype.h"


enum CHAR_TYPE getCharType(const char c) {
    if (c == 0) {
        return CH_TERMINATOR;
    }
    else if (c == 32) {
        return CH_SPACE;
    }
    else if (c == 10) {
        return CH_NEWLINE;
    }
    else if (c >= 48 && c <= 58) {
        return CH_DIGIT;
    }
    else if (c >= 97 && c <= 122) {
        return CH_LOWER_ASCII;
    }
    else if (c >= 65 && c <= 90) {
        return CH_UPPER_ASCII;
    }
    return CH_OTHER;
}
