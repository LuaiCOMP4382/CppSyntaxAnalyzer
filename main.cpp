#include <deque>
#include <iostream>
#include "util.h"
#include "lexical_analyzer.h"
#include "syntax_parser.h"

int main(int argc, char **argv) {

    // Prepare stream
    ifstream stream = Util::getStream("./source_code.txt");
    
    // Get deque of Tokens
    deque<Token> tokens = LexicalAnalyzer::parseTokens(stream);

    // Initialize RecursiveDescentParser
    RecursiveDescentParser recursiveDescentParser(tokens);

    // Parse code
    try {
        recursiveDescentParser.parseSyntax();
        cout << "Correct syntax\n";
    }
    catch (const string msg) {
        cout << msg;
    }

    return 0;
}
