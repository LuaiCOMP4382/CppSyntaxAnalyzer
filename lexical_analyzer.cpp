#include <string>
#include <fstream>
#include <deque>
#include <unordered_set>
#include "lexical_analyzer.h"
#include "token.h"

using namespace std;

// Initialize reservedWords.
unordered_set<string> LexicalAnalyzer::reservedWords(
    {"module", "const", "var", "integer", "task", "begin", "end", "input", "out", "if", "fi", "then", "else", "while", "do"}
);

deque<Token> LexicalAnalyzer::parseTokens(ifstream& stream) {
    // Return value.
    deque<Token> tokens;
    
    // Mid-scanning vars.
    char nextCharacter;
    unsigned int currentColumnIndex = 1;
    unsigned int currentLineNumber = 1;

    while (stream.get(nextCharacter)) {
        string nextTokenValue = "";

        if (isalpha(nextCharacter))
            nextTokenValue = getNextAlpha(stream, nextCharacter);
        else if (isdigit(nextCharacter))
            nextTokenValue = getNextNumeric(stream, nextCharacter);
        else if (isSpecialCharacter(nextCharacter))
            nextTokenValue = getNextSpecial(stream, nextCharacter);
        else if (nextCharacter == '\n') {
            currentColumnIndex = 0;
            currentLineNumber++;
            continue;
        } else // None matches
            throw "Unknown character";

        Token nextToken(nextTokenValue, currentColumnIndex++, currentLineNumber);
        tokens.push_back(move(nextToken));
    } 
    
    return tokens;
}

string LexicalAnalyzer::getNextAlpha(ifstream& stream, char nextCharacter) {
    string nextAlpha(1, nextCharacter);

    stream.get(nextCharacter);

    // All following alpha and digit characters are part of one word.
    while (isalpha(nextCharacter) || isdigit(nextCharacter)) {
        nextAlpha += nextCharacter;
        stream.get(nextCharacter);
    }

    stream.unget();
    
    if (reservedWords.find(nextAlpha) != reservedWords.end())
        // Return reserved word.
        return nextAlpha;

    return "user defined name";
}

string LexicalAnalyzer::getNextNumeric(ifstream& stream, char nextCharacter) {
    // Keep going until character type changes.
    while (isdigit(nextCharacter))
        stream.get(nextCharacter);

    stream.unget();

    return "integer value";
}

string LexicalAnalyzer::getNextSpecial(ifstream& stream, char nextCharacter) {
    // Check for special words that might span 2 characters.
    if (nextCharacter == '<') {
        // Possibilites: '<', '<>', '<='

        stream.get(nextCharacter);
        if (nextCharacter == '>')
            return "<>";
        
        if (nextCharacter == '=')
            return "<=";

        stream.unget();
        
        return "<";
    } else if (nextCharacter == '>') {
        // Possibilites: '>', '>='

        stream.get(nextCharacter);

        if (nextCharacter == '=')
            return ">=";

        stream.unget();

        return ">";
    } else if (nextCharacter == ':') {
        // Possibilites: ':', ':='

        stream.get(nextCharacter);

        if (nextCharacter == '=')
            return ":=";

        stream.unget();

        return ":";
    }

    // Character always comes alone.
    return string(1, nextCharacter);
}

bool LexicalAnalyzer::isSpecialCharacter(char character) {
    return character == '.' || character == ';' || character == '=' || character == ':' || character == ','
        || character == '(' || character == ')' || character == '+' || character == '-' || character == '*'
        || character == '/' || character == '<' || character == '>';
}
