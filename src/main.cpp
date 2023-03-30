#include "lexer.h"
#include <iostream>

int main(void)
{
    Lexer l = Lexer("'string 1' not a string 'string 2' ' some other string'");

    l.lex();
}
