# C-Compiler

A Compiler from C to x86 GCC-8.1.0 64-bit assembly, written in C.

## About
A simple C compiler based on the grammar and implementation timeline from the blog series [Writing a C Compiler](https://norasandler.com/2017/11/29/Write-a-Compiler.html/) by Nora Sandler, which can be used via a web editor which translates C code entered, displays the assembly created, runs the assembly and shows its output and return value. 

It supports a basic subset of C:
* Functions.
* Loops.
* If statements.
* Many types of unary and binary arithmetic and logical operators.
* Int type.
* Single file programs.
* Calls to C's standard library can also be made, providing you add a declaration of the function being used in your program and that the functions return and parameter types are ints (e.g. putchar).

## Usage
### Web version
Clone branch "main" into a local server (e.g. XAMPP) and open index.php (Requires GCC to run the assembly) (This was written for windows and may not work on other OS).
### Console version
Clone branch "console-version". Write your C code into "file.txt", then run code_generator.exe and the assembly will be written to "assembly.s". If you wish to run this assembly, enter "gcc assembly.s -o out", then run "out.exe" (Requires GCC).

## Grammar
```
<program> ::= { <function> | <declaration> }
<function> ::= "int" <id> "(" ")" "{" { <block-item> } "}"
<block-item> ::= <statement> | <declaration>
<declaration> ::= "int" <id> [ = <exp> ] ";"
<statement> ::= "return" <exp> ";"
              | <exp-option> ";"
              | "if" "(" <exp> ")" <statement> [ "else" <statement> ]
              | "{" { <block-item> } "}
              | "for" "(" <exp-option> ";" <exp-option> ";" <exp-option> ")" <statement>
              | "for" "(" <declaration> <exp-option> ";" <exp-option> ")" <statement>
              | "while" "(" <exp> ")" <statement>
              | "do" <statement> "while" "(" <exp> ")" ";"
              | "break" ";"
              | "continue" ";"
<exp-option> ::= <exp> | ""
<exp> ::= <id> "=" <exp> | <conditional-exp>
<conditional-exp> ::= <logical-or-exp> [ "?" <exp> ":" <conditional-exp> ]
<logical-or-exp> ::= <logical-and-exp> { "||" <logical-and-exp> }
<logical-and-exp> ::= <equality-exp> { "&&" <equality-exp> }
<equality-exp> ::= <relational-exp> { ("!=" | "==") <relational-exp> }
<relational-exp> ::= <additive-exp> { ("<" | ">" | "<=" | ">=") <additive-exp> }
<additive-exp> ::= <term> { ("+" | "-") <term> }
<term> ::= <factor> { ("*" | "/") <factor> }
<factor> ::= "(" <exp> ")" | <unary_op> <factor> | <int> | <id>
<unary_op> ::= "!" | "~" | "-"
```
_Note I also added the ability to use common increment operators (++,*=, etc) by modifying their tokens in the lexing stage_

## Implementation timeline
1. Integers and basic Main structure.
2. Unary operators.
3. Binary arithmetic operators.
4. Binary logical operators.
5. Local variables.
6. Conditional statements
7. Compound statements.
8. Loops.
9. Functions.
10. Global variables.
