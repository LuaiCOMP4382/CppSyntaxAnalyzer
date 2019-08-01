#pragma once

#include <string>
#include <fstream>
#include <deque>
#include <unordered_set>
#include "token.h"

using namespace std;

class LexicalAnalyzer {
    public:
        /**
         * Use file stream to read from source file and create a deque
         * holding Tokens in order.
         * 
         * @param stream The input file stream targeting the source file.
         * @return deque of Tokens in order.
         */
        static deque<Token> parseTokens(ifstream& stream);

    private:
        // Set of reserved words.
        static unordered_set<string> reservedWords;

        /**
         * Use file stream to extract a word that starts with alphabetical 
         * character that contains alphabetical and numeric characters.
         * Stops once it reaches a non-alphanumeric character.
         * 
         * If it is one of the reserved words, return it, else return
         * "user defined name".
         * 
         * @param stream The input file stream targeting the source file.
         * @param nextCharacter The first alphabitical character.
         * @return One of the reserved words, or "user defined name"/
         */
        static string getNextAlpha(ifstream& stream, char nextCharacter);
        
        /**
         * Use file stream to scan through the full numeric word. Stops
         * once it reaches a non-digit character. 
         * Then return "integer value"
         * 
         * @param stream The input file stream targeting the source file.
         * @param nextCharacter The first digit character.
         * @return "integer value"
         */
        static string getNextNumeric(ifstream& stream, char nextCharacter);

        /**
         * Use file stream to scan through the special character. It checks
         * maximum two consecutive characters. If the two are a valid
         * special word, return it, else return the character itself.
         * 
         * @param stream The input file stream targeting the source file.
         * @param nextCharacter The first special character.
         * @return Special word.
         */
        static string getNextSpecial(ifstream& stream, char nextCharacter);

        /**
         * Return true if the character is special
         * 
         * @param character Character to check.
         * @return True of character is special
         */
        static bool isSpecialCharacter(char character);
};
