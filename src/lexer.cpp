#include "lexer.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

Lexer::Lexer(std::string input)
{
    this->input = input;
}

std::vector<LEXED_TOKEN> Lexer::lex(void)
{
    std::vector<LEXED_TOKEN> tokens;

    while (cur_char < input.length() - 1)
    {
        LEXED_TOKEN token;

        char c = input[cur_char];

        if (c == '\n')
        {
            ++cur_line;
            cur_char_line = 0;
        }
        else if (c == '"' || c == '\'')
        {
            token.token_type = STRING;
            token.char_index = cur_char_line;
            token.line       = cur_line;
            token.value      = lex_string(c);
        }

        ++cur_char;
    }

    return tokens;
}

std::string Lexer::lex_string(char delim)
{
    ++cur_char;
    int start = cur_char;

    char c = input[cur_char];

    while (c != '\n' && c != delim && cur_char < input.length() - 1)
    {
        ++cur_char;
        c = input[cur_char];
    }

    if (cur_char >= input.length())
    {
        throw std::runtime_error("Error lexing string, reached end of input before hitting delimiter");
    }

    if (input[cur_char] == '\n')
    {
        throw std::runtime_error("Error indexing string, hit newline char");
    }

    std::string lexed = input.substr(start, cur_char - start);

    std::cout << lexed << std::endl;

    return input.substr(start, cur_char - start);
}