# C-Compiler

A Compiler from C to x86 GCC-8.1.0 64-bit assembly, written in C.

<H2>About</H2>
A simple C compiler based on the grammar and implementation timeline from the blog series [Writing a C Compiler]("https://norasandler.com/2017/11/29/Write-a-Compiler.html") by Nora Sandler, wrapped in a web editor which translates C code entered and in turn runs the newly translated assembly and shows its output and return value. 

It supports a basic subset of C:
⋅⋅* Functions
⋅⋅* Loops
⋅⋅* If statements
⋅⋅* Many types of unary and binary arithmetic and logical operators
⋅⋅* With only 1 type, int

<H2>Usage</H2>
Clone files into a local server (I used XAMPP) and open index.php. (Requires GCC) (This was written for windows and may not work on other OS)

<H2>Grammar</H2>
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

_Note I also added the ability to use common increment operators (++,*=, etc) by modifying their tokens in the lexing stage_

<H2>Timeline</H2>
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
