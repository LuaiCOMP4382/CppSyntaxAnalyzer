# Custom Syntax Analyzer in C++

## Project description
The capstone project for C++ nanodegree program. The program takes source code input from a text file, parses tokens and shows if the syntax of the code is correct. The project uses Recursive Descent Parsing method to analyze syntax, baesd on specific grammar and production rules, provided below.

In general, compilers go through 3 main processes: Lexical, syntax and semantic analysis.
In the first process, the text is scanned to extract list of tokens and create several data structures to store and prepare them for the next process. Then, to analyze syntax, the tokens' order is checked to ensure it follows the language's grammar correctly, which is defined by a set of production rules. If it succeeds, the last process checks the "semantics" of the text based on several rules enforced by the language, to ensure deterministic and correct behavior.

This project goes only through the first two processes.

Grammar (in EBNF form). Any word between ```"``` is terminal, and any reserved word is between ```**```:


```
module-declaration  →   module          "."
module              →   module-heading  declarations      compound-stmt
module-heading      →   **module**      "name"            ";"
declarations        →   const-decl      var-decl          task-decl
const-decl          →   **const**       (const-item       ";")+       |   λ
const-item          →   "name"          =                 "integer-value"
var-decl            →   **var**         (var-item         ";")+       |   λ
var-item            →   name-list       ":"               **integer**
name-list           →   "name"          (","              "name")* 
task-decl           →   task-heading    declarations      compound-stmt   ;            |   λ
task-heading        →   **task**        "name"            ";"
compund-stmt        →   **begin**       stmt-list         **end**
stmt-list           →   (statement      ";")*
statement           →   ass-stmt    |   inout-stmt  |     if-stmt    |    while-stmt   |  compound-stmt | λ
ass-stmt            →   ”name”          ":="              arith-exp
arith-exp           →   term            (add-sign         term)*
term                →   factor          (mul-sign         factor)*
factor              →   "("             arith-exp         ")"        |    name-value
name-value          →   "name"      |   "integer-value"
add-sign            →   "+"         |   "-"
mul-sign            →   "*"         |   "/"
inout-stmt          →   **input**       "("               "name"          ")"          |  **out**         "("   name-value   ")"
if-stmt             →   **if**          bool-exp          **then**        statement       else-part       **fi**
else-part           →   **else**        statement  |      λ
while-stmt          →   **while**       bool-exp          **do**          statement
bool-exp            →   name-value      relational-oper               name-value 
relational-oper     →   "="     |       "<>"     |        "<"    |        "<="     |      ">"    |        ">="
```

## Classes:
- **Util**: Contains static method to get ifstream. `util.h` and `util.cpp`.
- **Token**: Holds string value, line number and word column index. `token.h` and `token.cpp`.
- **LexicalAnalyzer**: Has static methods to parse from `ifstream` to `deque` of **Token**s, and a set of reserved words. `lexical_analyzer.h` and `lexical_analyzer.cpp`.
- **SyntaxParser**: An abstract class that holds `deque` of **Token**s, and methods for parsing and handling errors. `syntax_parser.h` and `syntax_parser.cpp`.
- **RecursiveDescentParser**: Inherits **SyntaxParser**, and contains all methods used in recursion in the parsing method. `syntax_parser.h` and `syntax_parser.cpp`.

## How to build and execute:
1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SyntaxAnalyzer`.

In `main.cpp`, it gets stream from file `"./source_code.txt"`. Add your file in the same directory and modify if necessary. If syntax is correct, it prints `Correct Syntax`, else prints the error details.

## Examples:
Examples of source codes and their syntax analysis results:
```
module something;
begin
end.
```
Correct Syntax

```
module 
something; 					begin
						end.
```
Correct Syntax

```
module sum;
	const
		NINE = 9
		BILLION = 1000;
	var
		a : integer;
		b : integer;
	task sum;
	begin
		a := 1000;
		if a >= BILLION then
			b := 9000
		fi;
  end;
begin
input (test);
end.
```
Error at line 4, word index 1<br />
Token found: user defined name<br />
Expected: ;

```
LONELY
```
Error at line 1, word index 1<br />
Token found: user defined name<br />
Expected: module

```
module dummy321412321 ;
const good=55; baaaaad     = 9000
;var jvoijsiofhsdou :integer; marco : integer; whilee : integer;
task something; const dude=   90; begin
sdfsd := (fadsfs) + 59;
if 55>=lookang then while 90 <> hi do fi
;
end;
```
Error at end<br />
Expected: begin

```
module dummy321412321 ;
const good=55; baaaaad     = 9000
;var jvoijsiofhsdou :integer; marco : integer; whilee : integer;
task something; const dude=   90; begin
sdfsd := (fadsfs) + 59;
if 55>=lookang then while 90 <> hi do fi
;
end;begin end.
```
Correct Syntax

## Rubric points criteria addressed:
- **The project demonstrates an understanding of C++ functions and control structures**: **LexicalAnalyzer** and **RecursiveDescentParser** classes are good examples.
- **The project reads data from a file and process the data, or the program writes data to a file**: In `main.cpp` with `./source_code.txt`, line 10.
- **The project uses Object Oriented Programming techniques**: **LexicalAnalyzer**, **RecursiveDescentParster** and **Token** classes.
- **Classes use appropriate access specifiers for class members**: **RecursiveDescentParster** hides the recursive methods.
- **Classes abstract implementation details from their interfaces**: **LexicalAnalyzer**'s `parseTokens`, **SyntaxParser**'s `parseSyntax`.
- **Classes encapsulate behavior**: **LexicalAnalyzer**'s `parseTokens`, `getNextAlpha`, `getNextNumeric`, `getNextSpecial`, `isSpecialCharacter`, and the class is all about extracting tokens. **RecursiveDescentParster**'s `parseSyntax` is the trigger for all recursive methods.
- **Classes follow an appropriate inheritance hierarchy**: **SyntaxParser** is abstract in nature, and **RecursiveDescentParser** "IS A" **SyntaxParser**.
- **Derived class functions override virtual base class functions**: `parseSyntax` is the overriden virtual method in **SyntaxParser** by **RecursiveDescentParser**.`
- **The project makes use of references in function declarations**: `parseTokens` in `lexical_analyzer.h` line 20, and `error` in `syntax_parser.h` line 22, and all recursive methods in **RecursiveDescentParser**.
- **The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate**.
- **The project uses move semantics to move data, instead of copying it, where possible**: `lexical_analyzer.h` line 41.
