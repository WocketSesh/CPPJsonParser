#if !defined(LEXER_H)
#define LEXER_H

#include <any>
#include <string>
#include <vector>

enum TOKENS
{
    TOKENS_ENUM = 0,
    STRING,
    NUMBER,
    BOOLEAN,
    ARRAY,
    ARRAY_START,
    ARRAY_END,
    OBJECT,
    OBJECT_START,
    OBJECT_END
};

struct LEXED_TOKEN
{
    TOKENS   token_type;
    std::any value;
    int      line;
    int      char_index;
};

class Lexer
{
  private:
    int         cur_line;
    int         cur_char;
    int         cur_char_line;
    std::string input;

  public:
    Lexer(std::string input);
    std::vector<LEXED_TOKEN> lex(void);
    std::string              lex_string(char delim);
    bool                     lex_bool();
    double                   lex_number();
    void                     print_tokens(std::vector<LEXED_TOKEN> tokens);
};

#endif // LEXER_H
