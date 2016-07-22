Steps to use homework assignment 3:

William Young and Tyler Gauntlett
COP3402-16Summer C001


1. Ensure homework3.c and input.txt are in the same folder
2. Run gcc -o compiler homework3.c
3. Run ./compile -1 to view array
4. Run ./compile -2 to parse the code for mistakes
5. 3 files (compile, lexemelist.txt and cleaninput.txt) are created.

Program halts after the first error.

Compile must follow the EBNF of Appendix B:

program ::= block "." .block ::= const-declaration var-declaration procedure-declaration statement. constdeclaration ::= ["const" ident "=" number {"," ident "=" number} ";"]. var-declaration ::= [ "var "ident {"," ident} “;"].procedure-declaration ::= { "procedure" ident ";" block ";" }statement::= [ ident ":=" expression | "call" ident| "begin" statement { ";" statement } "end"| "if" condition "then" statement ["else" statement] | "while" condition "do" statement| "read" ident| "write" ident|e].condition ::= "odd" expression| expression rel-op expression.rel-op ::= "="|“<>"|"<"|"<="|">"|">=“.expression ::= [ "+"|"-"] term { ("+"|"-") term}.term ::= factor {("*"|"/") factor}.factor ::= ident | number | "(" expression ")“.number ::= digit {digit}.ident ::= letter {letter | digit}.digit ;;= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9“. letter ::= "a" | "b" | ... | "y" | "z" | "A" | "B" | ... | "Y" | "Z".