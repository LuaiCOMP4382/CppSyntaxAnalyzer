#include <deque>
#include "token.h"

using namespace std;

class SyntaxParser {
    public:
        // Tokens to parse their syntax.
        deque<Token> tokens;

        SyntaxParser(deque<Token> tokens) : tokens(tokens) {}

        virtual void parseSyntax() = 0;

        /**
         * Throws a string exception containing data from token and 
         * expected value(s).
         * 
         * @param token Token to get data from.
         * @param expectedValues string value(s) that should come after.
         */
        void error(Token &token, const string expectedValues);
};

class RecursiveDescentParser : public SyntaxParser {
    public:
        RecursiveDescentParser(deque<Token> tokens) : SyntaxParser(tokens) {}

        /**
         * Apply recursive descent parsing methods recursively on tokens, and on error,
         * throw an exception.
         */
        void parseSyntax() override;
    private:
        /**
         * Check if the iterator's token is present and has the correct value.
         * 
         * @param tokenPtr The iterator pointer that has the Token.
         * @param expectedValue The value that the token must have.
         */
        bool isValidToken(deque<Token>::iterator &tokenPtr, const string expectedValue);

        // The following are all methods named explicitly after the production rules, and are used directly in parsing.
        void module_declaration(deque<Token>::iterator &tokenPtr);
        void module(deque<Token>::iterator &tokenPtr);
        void module_heading(deque<Token>::iterator &tokenPtr);
        void declarations(deque<Token>::iterator &tokenPtr);
        void const_decl(deque<Token>::iterator &tokenPtr);
        void const_item(deque<Token>::iterator &tokenPtr);
        void var_decl(deque<Token>::iterator &tokenPtr);
        void var_item(deque<Token>::iterator &tokenPtr);
        void name_list(deque<Token>::iterator &tokenPtr);
        void task_decl(deque<Token>::iterator &tokenPtr);
        void task_heading(deque<Token>::iterator &tokenPtr);
        void compound_stmt(deque<Token>::iterator &tokenPtr);
        void stmt_list(deque<Token>::iterator &tokenPtr);
        void statement(deque<Token>::iterator &tokenPtr);
        void ass_stmt(deque<Token>::iterator &tokenPtr);
        void arith_exp(deque<Token>::iterator &tokenPtr);
        void term(deque<Token>::iterator &tokenPtr);
        void factor(deque<Token>::iterator &tokenPtr);
        void name_value(deque<Token>::iterator &tokenPtr);
        void add_sign(deque<Token>::iterator &tokenPtr);
        void mul_sign(deque<Token>::iterator &tokenPtr);
        void inout_stmt(deque<Token>::iterator &tokenPtr);
        void if_stmt(deque<Token>::iterator &tokenPtr);
        void else_part(deque<Token>::iterator &tokenPtr);
        void while_stmt(deque<Token>::iterator &tokenPtr);
        void bool_exp(deque<Token>::iterator &tokenPtr);
        void relational_oper(deque<Token>::iterator &tokenPtr);
};
