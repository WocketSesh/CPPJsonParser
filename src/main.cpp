#include "lexer.h"
#include <any>
#include <iostream>
#include <vector>

int main(void)
{
    Lexer l = Lexer("'string 1' not a string \n'string 2' ' some other string' 123 321 101 1.4 -1.4 .14 -.21 ");

    std::vector<LEXED_TOKEN> tokens = l.lex();

    l.print_tokens(tokens);
}
