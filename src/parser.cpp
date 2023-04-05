#include "parser.h"
#include "lexer.h"
#include <any>
#include <cstdlib>
#include <map>
#include <stdio.h>
#include <vector>

Parser::Parser(std::vector<LEXED_TOKEN> tokens)
{
    this->tokens = tokens;
}

JSONNode Parser::parse()
{

    std::vector<JSONNode>           *current_arr;
    std::map<std::string, JSONNode> *current_obj;
    bool                             expecting_obj_value = false;

    LevelTracker lt;
    lt.level_type = NONE_LEVEL;
    lt.prev       = NULL;

    LEXED_TOKEN *prev_token = nullptr;

    if (tokens.empty())
    {
        JSONNode node;
        node.type  = TOKENS_ENUM;
        node.value = nullptr;
        return node;
    }

    if (tokens.at(0).token_type != ARRAY_START && tokens.at(0).token_type != OBJECT_START)
    {
        fprintf(stderr, "Tokens array expected to start with ARRAY_START or OBJECT_START.\n");
        exit(EXIT_FAILURE);
    }

    for (auto i = 0; i < tokens.size(); i++)
    {
        LEXED_TOKEN cur_token = tokens.at(i);
        JSONNode    node;

        switch (cur_token.token_type)
        {
        case STRING:
            node.type  = STRING;
            node.value = cur_token.value;

            if (lt.level_type == ARRAY_LEVEL)
            {
                current_arr->push_back(node);
            }
            else if (lt.level_type == OBJECT_LEVEL)
            {
                if (prev_token->token_type != STRING && prev_token->token_type != OBJECT_START)
                {
                    fprintf(stderr, "Error while parsing tokens, got string but prev token was not STRING(KEY) and not "
                                    "OBJECT_START\n");
                    exit(EXIT_FAILURE);
                }
                // If expecting value push to map, otherwise just set prev_token so it can be used as a key for the next
                // token.
                if (expecting_obj_value)
                {
                    current_obj->insert(std::pair(std::any_cast<std::string>(prev_token->value), node));
                    expecting_obj_value = false;
                }
                else
                    expecting_obj_value = true;
            }

            prev_token = &cur_token;
            break;
        case NUMBER:
            node.type  = STRING;
            node.value = cur_token.value;

            if (lt.level_type == ARRAY_LEVEL)
            {
                current_arr->push_back(node);
            }
            else
            {
                if (prev_token->token_type != STRING)
                {
                    fprintf(stderr, "Error while parsing tokens, got number but prev token was not STRING(KEY)\n");
                    exit(EXIT_FAILURE);
                }

                if (expecting_obj_value)
                {
                    current_obj->insert(std::pair(std::any_cast<std::string>(prev_token->value), node));
                    expecting_obj_value = false;
                }
                else
                {
                    fprintf(stderr, "Error while parsing tokens, got number but was expecting key.\n");
                    exit(EXIT_FAILURE);
                }
            }

            prev_token = &cur_token;
            break;

        case BOOLEAN:
            node.type  = BOOLEAN;
            node.value = cur_token.value;

            if (lt.level_type == ARRAY_LEVEL)
            {
                current_arr->push_back(node);
            }
            else
            {
                if (prev_token->token_type != STRING)
                {
                    fprintf(stderr, "Error while parsing tokens, got BOOLEAN but was expecting STRING(KEY)\n");
                    exit(EXIT_FAILURE);
                }

                if (expecting_obj_value)
                {
                    current_obj->insert(std::pair(std::any_cast<std::string>(prev_token->value), node));
                    expecting_obj_value = false;
                }
                else
                {
                    fprintf(stderr, "Error while parsing tokens, got BOOLEAN but was expecting key\n");
                    exit(EXIT_FAILURE);
                }
            }

            prev_token = &cur_token;
            break;

        default:
            break;
        }
    }
}
