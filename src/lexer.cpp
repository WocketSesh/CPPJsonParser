#include "lexer.h"
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <vector>

// TODO: change all throw statements to fprintf(stderr)

Lexer::Lexer(std::string input)
{
    this->input = input;
}

std::vector<LEXED_TOKEN> Lexer::lex(void)
{
    std::vector<LEXED_TOKEN> tokens;

    cur_line = 1;

    for (char c = input[cur_char]; cur_char < input.length(); cur_char++, cur_char_line++, c = input[cur_char])
    {
        LEXED_TOKEN token;
        token.token_type = TOKENS_ENUM;

        if (c == ':' || c == ' ' || c == ',')
            continue;

        if (c == '\n')
        {
            ++cur_line;
            cur_char_line = 0;
        }
        // JSON doesnt support single quote?
        // Also doesnt support unquoted keys?!?
        else if (c == '"' || c == '\'')
        {
            token.token_type = STRING;
            token.char_index = cur_char_line;
            token.line       = cur_line;
            token.value      = lex_string(c);
        }
        else if (isdigit(c) || c == '-' || c == '.')
        {
            token.token_type = NUMBER;
            token.char_index = cur_char_line;
            token.line       = cur_line;
            token.value      = lex_number();
        }
        else if (c == '[')
        {
            token.token_type = ARRAY_START;
            token.char_index = cur_char_line;
            token.line       = cur_line;
            token.value      = NULL;
        }
        else if (c == ']')
        {
            token.token_type = ARRAY_END;
            token.char_index = cur_char_line;
            token.line       = cur_line;
            token.value      = NULL;
        }
        else if (c == '{')
        {
            token.token_type = OBJECT_START;
            token.char_index = cur_char_line;
            token.line       = cur_line;
            token.value      = NULL;
        }
        else if (c == '}')
        {
            token.token_type = OBJECT_END;
            token.char_index = cur_char_line;
            token.line       = cur_line;
            token.value      = NULL;
        }
        // can only be bool value?
        else if (c == 'f' || c == 't')
        {
            token.token_type = BOOLEAN;
            token.char_index = cur_char_line;
            token.line       = cur_line;
            token.value      = lex_bool();
        }
        else
        {
            fprintf(stderr, "Unexpected token at %d:%d single: '%c' full: '%s'\n", cur_line, cur_char_line, c,
                    lex_string(' ').c_str());
            exit(EXIT_FAILURE);
        }

        if (token.token_type != TOKENS_ENUM)
            tokens.push_back(token);
    }

    return tokens;
}

bool Lexer::lex_bool()
{
    int start = cur_char;

    for (char c = input[cur_char];
         c != '\n' && c != ' ' && c != ',' && c != ']' && c != '}' && cur_char < input.length();
         ++cur_char, ++cur_char_line, c = input[cur_char])
        ;

    std::string b = input.substr(start, cur_char - start);

    if (b != "true" && b != "false")
    {
        throw std::runtime_error("Error lexing boolean");
    }

    return b == "true" ? true : false;
}

std::string Lexer::lex_string(char delim)
{
    ++cur_char;
    ++cur_char_line;
    int start = cur_char;

    for (char c = input[cur_char]; c != '\n' && c != delim && cur_char < input.length() - 1;
         ++cur_char, ++cur_char_line, c = input[cur_char])
        ;

    if (cur_char >= input.length())
    {
        throw std::runtime_error("Error lexing string, reached end of input before hitting delimiter");
    }

    if (input[cur_char] == '\n')
    {
        throw std::runtime_error("Error indexing string, hit newline char");
    }

    return input.substr(start, cur_char - start);
}

double Lexer::lex_number()
{
    int  start = cur_char;
    char c     = input[cur_char];

    for (c = input[cur_char]; (isdigit(c) || c == '.' || c == '-') && cur_char < input.length();
         ++cur_char, c = input[cur_char])
        ;

    if (!isdigit(c) && c != '.' && c != ' ' && c != ',' && cur_char < input.length())
    {
        std::cout << c << std::endl;
        throw std::runtime_error("Error lexing string, got nondigit char that is also not '.'");
    }

    return std::stod(input.substr(start, cur_char - start));
}

void Lexer::print_tokens(std::vector<LEXED_TOKEN> tokens)
{
    for (auto i = 0; i < tokens.size(); i++)
    {
        switch (tokens[i].token_type)
        {
        case STRING:
            fprintf(stdout, "Token Type: %s\nToken Line: %d\nToken Char: %d\nToken Value: %s\n", "string",
                    tokens[i].line, tokens[i].char_index, std::any_cast<std::string>(tokens[i].value).c_str());
            break;

        case NUMBER:
            fprintf(stdout, "Token Type: %s\nToken Line: %d\nToken Char: %d\nToken Value: %.14g\n", "number",
                    tokens[i].line, tokens[i].char_index, std::any_cast<double>(tokens[i].value));
            break;

        case BOOLEAN:
            fprintf(stdout, "Token Type: %s\nToken Line: %d\nToken Char: %d\nToken Value: %s\n", "boolean",
                    tokens[i].line, tokens[i].char_index, std::any_cast<bool>(tokens[i].value) ? "true" : "false");
            break;

        case ARRAY_START:
            fprintf(stdout, "Token Type: %s\nToken Line: %d\nToken Char: %d\nToken Value: %s\n", "ARRAY_START",
                    tokens[i].line, tokens[i].char_index, "[");
            break;

        case ARRAY_END:
            fprintf(stdout, "Token Type: %s\nToken Line: %d\nToken Char: %d\nToken Value: %s\n", "ARRAY_END",
                    tokens[i].line, tokens[i].char_index, "]");
            break;

        case OBJECT_START:
            fprintf(stdout, "Token Type: %s\nToken Line: %d\nToken Char: %d\nToken Value: %s\n", "OBJECT_START",
                    tokens[i].line, tokens[i].char_index, "{");
            break;

        case OBJECT_END:
            fprintf(stdout, "Token Type: %s\nToken Line: %d\nToken Char: %d\nToken Value: %s\n", "OBJECT_END",
                    tokens[i].line, tokens[i].char_index, "}");
            break;

        default:
            break;
        }

        fprintf(stdout, "-------\n");
    }
}