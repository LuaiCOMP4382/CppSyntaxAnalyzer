# CppSyntaxAnalyzer

The capstone project for C++ nanodegree program. The program takes source code input from a text file, parses tokens and shows if the syntax of the code is correct. The project uses Recursive Descent Parsing method to analyze syntax, baesd on specific grammar and production rules, provided below.

In general, compilers go through 3 main processes: Lexical, syntax and semantic analysis.
In the first process, the text is scanned to extract list of tokens and create several data structures to store and prepare them for the next process. Then, to analyze syntax, the tokens' order is checked to ensure it follows the language's grammar correctly, which is defined by a set of production rules. If it succeeds, the last process checks the "semantics" of the text based on several rules enforced by the language, to ensure deterministic and correct behavior.

This project goes only through the first two processes.

Examples of source codes and their syntax analysis results:
TO BE ADDED

Grammar (in EBNF form). Any word between ```"``` is terminal, and any reserved word is between ```**```:


```
module-declaration  →   module          "."
module              →   module-heading  declarations  compound-stmt
module-heading      →   **module**      "name"        ";"
declarations        →   const-decl      var-decl      task-decl
const-decl          →   **const**       (const-item   ";")+       |   λ
const-item          →   "name"          =             "integer-value"
var-decl            →   **var**         (var-item     ";")+       |   λ
var-item            →   name-list       ":"           **integer**
name-list           →   "name"          (","          "name")* 
task-decl           →   task-heading    declarations  compound-stmt | λ
task-heading        →   **task**        "name"        ";"
compund-stmt        →   **begin**       stmt-list     **end**
stmt-list           →   (statement      ";")*
statement           →   ass-stmt    |   inout-stmt  | if-stmt    |    while-stmt   |  compound-stmt | λ
ass-stmt            →   ”name”      ":="              arith-exp
arith-exp           →   term            (add-sign         term)*
term                →   factor          (mul-sign         factor)*
factor              →   "("             arith-exp         ")"        |    name-value
name-value          →   "name"      |   "integer-value"
add-sign            →   "+"         |   "-"
mul-sign            →   "*"         |   "/"
inout-stmt          →   **input**       "("           "name"          ")"          |  **out**         "("   name-value   ")"
if-stmt             →   **if**          bool-exp      **then**        statement       else-part       **fi**
else-part           →   **else**        statement  |  λ
while-stmt          →   **while**       bool-exp      **do**          statement
bool-exp            →   name-value      relational-oper               name-value 
relational-oper     →   "="     |       "<>"     |    "<"    |        "<="     |      ">"    |        ">="
```
