#if !defined(PARSER_H)
#define PARSER_H

#include "lexer.h"
#include <any>
#include <vector>

enum LevelType
{
    NONE_LEVEL = 0,
    ARRAY_LEVEL,
    OBJECT_LEVEL
};

struct LevelTracker
{
    LevelTracker *prev;
    LevelType     level_type;
};

// NUMBER  = double
// STRING = std::string
// BOOL = bool
// OBJECT = std::map
// ARRAY = std::vector
struct JSONNode
{
    TOKENS   type;
    std::any value;
};

class Parser
{
  public:
    std::vector<LEXED_TOKEN> tokens;

    JSONNode parse(void);
    Parser(std::vector<LEXED_TOKEN> tokens);
    ~Parser();
};

#endif