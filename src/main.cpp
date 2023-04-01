#include "lexer.h"
#include <any>
#include <iostream>
#include <vector>

int main(void)
{
    Lexer l = Lexer("{'somekey': 'somevalue', 'somenumb': 123, 'somearray': ['1st', '2nd', '3rd', '4th', false, true, "
                    "], "
                    "'someobj': {}}");

    std::vector<LEXED_TOKEN> tokens = l.lex();

    l.print_tokens(tokens);
}
