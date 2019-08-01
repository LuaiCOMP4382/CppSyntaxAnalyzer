#pragma once

#include <string>

using namespace std;

class Token {
    public:
        Token(string value, unsigned int columnIndex, unsigned int lineNumber) : value(value), columnIndex(columnIndex), lineNumber(lineNumber) {}

        // The value it holds.
        const string value;
        unsigned int const getColumnIndex();
        unsigned int const getLineNumber();
    private:
        // Index from left to right.
        const unsigned int columnIndex;
        // Line number the token is in.
        const unsigned int lineNumber;
};
