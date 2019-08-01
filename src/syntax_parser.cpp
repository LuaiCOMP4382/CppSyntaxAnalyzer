#include <iostream>
#include <string>
#include <deque>
#include "syntax_parser.h"
#include "token.h"

using namespace std;

void SyntaxParser::error(Token &token, const string expectedValues) {
    string errorMessage = "";

    if (token.getColumnIndex() == 0 && token.getLineNumber() == 0)
        errorMessage = 
        "Error at end\nExpected: " + expectedValues + "\n";
    else
        errorMessage = 
        "Error at line " + to_string(token.getLineNumber()) + ", word index " + to_string(token.getColumnIndex()) +
        "\nToken found: " + token.value + 
        "\nExpected: " + expectedValues + "\n";

    throw errorMessage;
}

void RecursiveDescentParser::parseSyntax() {
    deque<Token>::iterator tokenPtr = tokens.begin();

    // Start the recursive descent parsing process
    module_declaration(tokenPtr);
}

bool RecursiveDescentParser::isValidToken(deque<Token>::iterator &tokenPtr, const string expectedValue) {
    // Token is present and has the correct value.
    return tokenPtr != tokens.end() && tokenPtr->value == expectedValue;
}

void RecursiveDescentParser::module_declaration(deque<Token>::iterator &tokenPtr) {
    module(tokenPtr);
    
    if (isValidToken(tokenPtr, "."))
        // Correct syntax!
        return;
    
    error(*tokenPtr, ".");
}

void RecursiveDescentParser::module(deque<Token>::iterator &tokenPtr) {
    module_heading(tokenPtr);
    declarations(tokenPtr);
    compound_stmt(tokenPtr);
}


void RecursiveDescentParser::module_heading(deque<Token>::iterator &tokenPtr) {
    if (isValidToken(tokenPtr, "module")) {
        tokenPtr++;
        if (isValidToken(tokenPtr, "user defined name")) {
            tokenPtr++;
            if (isValidToken(tokenPtr, ";")) {
                tokenPtr++;
                return;
            } else
                error(*tokenPtr, ";");
        } else
            error(*tokenPtr, "user defined name");
    } else
        error(*tokenPtr, "module");
}

void RecursiveDescentParser::declarations(deque<Token>::iterator &tokenPtr) {
    const_decl(tokenPtr);
    var_decl(tokenPtr);
    task_decl(tokenPtr);
}

void RecursiveDescentParser::const_decl(deque<Token>::iterator &tokenPtr) {
    if (isValidToken(tokenPtr, "const")) {
        tokenPtr++;
        if (isValidToken(tokenPtr, "user defined name")) {
            const_item(tokenPtr);
            if (isValidToken(tokenPtr, ";")) {
                tokenPtr++;
                if (isValidToken(tokenPtr, "user defined name"))
                    while (isValidToken(tokenPtr, "user defined name")) {
                        const_item(tokenPtr);
                        if (isValidToken(tokenPtr, ";"))
                            tokenPtr++;
                        else
                            error(*tokenPtr, ";");
                    }
            } else
                error(*tokenPtr, ";");
        } else
            error(*tokenPtr, "user defined name");
    } else if (!isValidToken(tokenPtr, "var") && !isValidToken(tokenPtr, "task") && !isValidToken(tokenPtr, "begin"))
        error(*tokenPtr, "const, var, task, begin");
}

void RecursiveDescentParser::const_item(deque<Token>::iterator &tokenPtr) {
    if (isValidToken(tokenPtr, "user defined name")) {
        tokenPtr++;
        if (isValidToken(tokenPtr, "=")) {
            tokenPtr++;
            if (isValidToken(tokenPtr, "integer value"))
                tokenPtr++;
            else
                error(*tokenPtr, "integer value");
        } else
            error(*tokenPtr, "=");
    } else
        error(*tokenPtr, "user defined name");
}

void RecursiveDescentParser::var_decl(deque<Token>::iterator &tokenPtr) {
    if (isValidToken(tokenPtr, "var")) {
        tokenPtr++;
        if (isValidToken(tokenPtr, "user defined name")) {
            var_item(tokenPtr);
            if (isValidToken(tokenPtr, ";")) {
                tokenPtr++;
                if (isValidToken(tokenPtr, "user defined name"))
                    while (isValidToken(tokenPtr, "user defined name")) {
                        var_item(tokenPtr);

                        if (isValidToken(tokenPtr, ";"))
                            tokenPtr++;
                        else
                            error(*tokenPtr, ";");
                    }
            } else
                error(*tokenPtr, ";");
        } else
            error(*tokenPtr, "user defined name");
    } else if (!isValidToken(tokenPtr, "task") && !isValidToken(tokenPtr, "begin"))
        error(*tokenPtr, "var, task, begin");
}

void RecursiveDescentParser::var_item(deque<Token>::iterator &tokenPtr) {
    name_list(tokenPtr);
    if (isValidToken(tokenPtr, ":")) {
        tokenPtr++;
        if (isValidToken(tokenPtr, "integer"))
            tokenPtr++;
        else
            error(*tokenPtr, "integer");
    } else
        error(*tokenPtr, ":");

}

void RecursiveDescentParser::name_list(deque<Token>::iterator &tokenPtr) {
    if (isValidToken(tokenPtr, "user defined name")) {
        tokenPtr++;
        if (isValidToken(tokenPtr, ","))
            while (isValidToken(tokenPtr, ",")) {
                tokenPtr++;

                if (isValidToken(tokenPtr, "user defined name"))
                    tokenPtr++;
                else
                    error(*tokenPtr, "user defined name");
            }
    } else
        error(*tokenPtr, "user defined name");
}

void RecursiveDescentParser::task_decl(deque<Token>::iterator &tokenPtr) {
    if (isValidToken(tokenPtr, "task")) {
        task_heading(tokenPtr);
        declarations(tokenPtr);
        compound_stmt(tokenPtr);
        
        if (isValidToken(tokenPtr, ";"))
            tokenPtr++;
        else
            error(*tokenPtr, ";");
    } else if (!isValidToken(tokenPtr, "begin"))
        error(*tokenPtr, "task, begin");
}

void RecursiveDescentParser::task_heading(deque<Token>::iterator &tokenPtr) {
    if (isValidToken(tokenPtr, "task")) {
        tokenPtr++;
        if (isValidToken(tokenPtr, "user defined name")) {
            tokenPtr++;
            if (isValidToken(tokenPtr, ";"))
                tokenPtr++;
            else
                error(*tokenPtr, ";");
        } else
            error(*tokenPtr, "user defined name");
    } else
        error(*tokenPtr, "task");
}

void RecursiveDescentParser::compound_stmt(deque<Token>::iterator &tokenPtr) {
    if (isValidToken(tokenPtr, "begin")) {
        tokenPtr++;
        stmt_list(tokenPtr);

        if (isValidToken(tokenPtr, "end"))
            tokenPtr++;
        else
            error(*tokenPtr, "end");
    } else
        error(*tokenPtr, "begin");
}

void RecursiveDescentParser::stmt_list(deque<Token>::iterator &tokenPtr) {

    if (isValidToken(tokenPtr, "user defined name") || isValidToken(tokenPtr, "input") || isValidToken(tokenPtr, "out")
    || isValidToken(tokenPtr, "if") || isValidToken(tokenPtr, "while") || isValidToken(tokenPtr, "begin")
    || isValidToken(tokenPtr, ";")) {

        while (isValidToken(tokenPtr, "user defined name") || isValidToken(tokenPtr, "input") || isValidToken(tokenPtr, "out")
        || isValidToken(tokenPtr, "if") || isValidToken(tokenPtr, "while") || isValidToken(tokenPtr, "begin")
        || isValidToken(tokenPtr, ";")) {
            statement(tokenPtr);
            if (isValidToken(tokenPtr, ";"))
                tokenPtr++;
            else
                error(*tokenPtr, ";");
        }
    } else if (!isValidToken(tokenPtr, "end"))
        error(*tokenPtr, "user defined name, input, out, if, while, begin, ;, end");
}

void RecursiveDescentParser::statement(deque<Token>::iterator &tokenPtr) {
    if (isValidToken(tokenPtr, "user defined name"))
        ass_stmt(tokenPtr);
    else if (isValidToken(tokenPtr, "input") || isValidToken(tokenPtr, "out"))
        inout_stmt(tokenPtr);
    else if (isValidToken(tokenPtr, "if"))
        if_stmt(tokenPtr);
    else if (isValidToken(tokenPtr, "while"))
        while_stmt(tokenPtr);
    else if (isValidToken(tokenPtr, "begin"))
        compound_stmt(tokenPtr);
    else if (!isValidToken(tokenPtr, "else") && !isValidToken(tokenPtr, "fi") && !isValidToken(tokenPtr, ";"))
        error(*tokenPtr, "user defined name, input, out, if, while, begin, else, fi, ;");
}

void RecursiveDescentParser::ass_stmt(deque<Token>::iterator &tokenPtr) {
    if (isValidToken(tokenPtr, "user defined name")) {
        tokenPtr++;
        if (isValidToken(tokenPtr, ":=")) {
            tokenPtr++;
            arith_exp(tokenPtr);
        } else
            error(*tokenPtr, ":=");
    } else
        error(*tokenPtr, "user defined name");
}

void RecursiveDescentParser::arith_exp(deque<Token>::iterator &tokenPtr) {
    term(tokenPtr);
    if (isValidToken(tokenPtr, "+") || isValidToken(tokenPtr, "-"))
        while (isValidToken(tokenPtr, "+") || isValidToken(tokenPtr, "-")) {
            add_sign(tokenPtr);
            term(tokenPtr);
        }
}

void RecursiveDescentParser::term(deque<Token>::iterator &tokenPtr) {
    factor(tokenPtr);
    if (isValidToken(tokenPtr, "*") || isValidToken(tokenPtr, "/"))
        while (isValidToken(tokenPtr, "*") || isValidToken(tokenPtr, "/")) {
            mul_sign(tokenPtr);
            factor(tokenPtr);
        }
}

void RecursiveDescentParser::factor(deque<Token>::iterator &tokenPtr) {
    if (isValidToken(tokenPtr, "(")) {
        tokenPtr++;
        arith_exp(tokenPtr);
        if (isValidToken(tokenPtr, ")"))
            tokenPtr++;
        else
            error(*tokenPtr, ")");
    } else if (isValidToken(tokenPtr, "user defined name") || isValidToken(tokenPtr, "integer value"))
        name_value(tokenPtr);
    else
        error(*tokenPtr, "(, user defined name, integer-value");
}

void RecursiveDescentParser::name_value(deque<Token>::iterator &tokenPtr) {
    if (isValidToken(tokenPtr, "user defined name"))
        tokenPtr++;
    else if (isValidToken(tokenPtr, "integer value"))
        tokenPtr++;
    else
        error(*tokenPtr, "user defined name, integer-value");
}

void RecursiveDescentParser::add_sign(deque<Token>::iterator &tokenPtr) {
    if (isValidToken(tokenPtr, "+"))
        tokenPtr++;
    else if (isValidToken(tokenPtr, "-"))
        tokenPtr++;
    else
        error(*tokenPtr, "+, -");
}

void RecursiveDescentParser::mul_sign(deque<Token>::iterator &tokenPtr) {
    if (isValidToken(tokenPtr, "*"))
        tokenPtr++;
    else if (isValidToken(tokenPtr, "/"))
        tokenPtr++;
    else
        error(*tokenPtr, "-, /");
}

void RecursiveDescentParser::inout_stmt(deque<Token>::iterator &tokenPtr) {
    if (isValidToken(tokenPtr, "input")) {
        tokenPtr++;

        if (isValidToken(tokenPtr, "(")) {
            tokenPtr++;

            if (isValidToken(tokenPtr, "user defined name")) {
                tokenPtr++;

                if (isValidToken(tokenPtr, ")"))
                    tokenPtr++;
                else
                    error(*tokenPtr, ")");
            } else
                error(*tokenPtr, "user defined name");
        } else
            error(*tokenPtr, "(");
    } else if (isValidToken(tokenPtr, "out")) {
        tokenPtr++;

        if (isValidToken(tokenPtr, "(")) {
            tokenPtr++;
            
            name_value(tokenPtr);

            if (isValidToken(tokenPtr, ")"))
                    tokenPtr++;
                else
                    error(*tokenPtr, ")");
        } else
            error(*tokenPtr, "(");
    } else
        error(*tokenPtr, "input, out");
}

void RecursiveDescentParser::if_stmt(deque<Token>::iterator &tokenPtr) {
    if (isValidToken(tokenPtr, "if")) {
        tokenPtr++;

        bool_exp(tokenPtr);

        if (isValidToken(tokenPtr, "then")) {
            tokenPtr++;

            statement(tokenPtr);
            else_part(tokenPtr);

            if (isValidToken(tokenPtr, "fi"))
                tokenPtr++;
            else
                error(*tokenPtr, "fi");
        } else
            error(*tokenPtr, "then");
    } else
        error(*tokenPtr, "if");
}

void RecursiveDescentParser::else_part(deque<Token>::iterator &tokenPtr) {
    if (isValidToken(tokenPtr, "else")) {
        tokenPtr++;

        statement(tokenPtr);
    } else if (!isValidToken(tokenPtr, "fi"))
        error(*tokenPtr, "else, fi");
}

void RecursiveDescentParser::while_stmt(deque<Token>::iterator &tokenPtr) {
    if (isValidToken(tokenPtr, "while")) {
        tokenPtr++;

        bool_exp(tokenPtr);
        if (isValidToken(tokenPtr, "do")) {
            tokenPtr++;

            statement(tokenPtr);
        } else
            error(*tokenPtr, "do");
    } else
        error(*tokenPtr, "while");
}

void RecursiveDescentParser::bool_exp(deque<Token>::iterator &tokenPtr) {
    name_value(tokenPtr);
    relational_oper(tokenPtr);
    name_value(tokenPtr);
}

void RecursiveDescentParser::relational_oper(deque<Token>::iterator &tokenPtr) {
    if (isValidToken(tokenPtr, "="))
        tokenPtr++;
    else if (isValidToken(tokenPtr, "<>"))
        tokenPtr++;
    else if (isValidToken(tokenPtr, "<"))
        tokenPtr++;
    else if (isValidToken(tokenPtr, "<="))
        tokenPtr++;
    else if (isValidToken(tokenPtr, ">"))
        tokenPtr++;
    else if (isValidToken(tokenPtr, ">="))
        tokenPtr++;
    else
        error(*tokenPtr, "=, <>, <=, >, >=");
}
