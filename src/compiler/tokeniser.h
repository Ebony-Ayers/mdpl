#ifndef TOKENISER_HEADER_GUARD
#define TOKENISER_HEADER_GUARD

#include "../pch.h"
#include "../common/mdpl_common.h"
#include "../runtime_lib/mdpl_error.h"

#define MDPL_COMPILER_TOKEN_TYPE_empty        1
#define MDPL_COMPILER_TOKEN_TYPE_symbol       2

//the possible valid symbols are !, #, $, %, &, (, ), *, +, ,, -, ., /, :, <, =, >, ?, &, @, [, \, ], ^, `, {, |, }, ~,
//                               &&, **, ++, --, .., ::, <<, !=, %=, &=, *=,  +=, -=,  <=,  ==,  >=,  ~=,  ->,  >>,  [[,  ]],  ^^,  {{,  ||,  }}
#define MDPL_COMPILER_SYMBOL_exclamation              33    /*!*/
#define MDPL_COMPILER_SYMBOL_hash                     35    /*#*/
#define MDPL_COMPILER_SYMBOL_dollar                   36    /*$*/
#define MDPL_COMPILER_SYMBOL_mod                      37    /*%*/
#define MDPL_COMPILER_SYMBOL_and                      38    /*&*/
#define MDPL_COMPILER_SYMBOL_openRoundBracket         40    /*(*/
#define MDPL_COMPILER_SYMBOL_closeRoundBracket        41    /*)*/
#define MDPL_COMPILER_SYMBOL_star                     42    /***/
#define MDPL_COMPILER_SYMBOL_plus                     43    /*+*/
#define MDPL_COMPILER_SYMBOL_comma                    44    /*,*/
#define MDPL_COMPILER_SYMBOL_minus                    45    /*-*/
#define MDPL_COMPILER_SYMBOL_dot                      46    /*.*/
#define MDPL_COMPILER_SYMBOL_frontSlash               47    /*/*/
#define MDPL_COMPILER_SYMBOL_colons                   58    /*:*/
#define MDPL_COMPILER_SYMBOL_lessThan                 60    /*<*/
#define MDPL_COMPILER_SYMBOL_equals                   61    /*=*/
#define MDPL_COMPILER_SYMBOL_greaterThan              62    /*>*/
#define MDPL_COMPILER_SYMBOL_question                 63    /*?*/
#define MDPL_COMPILER_SYMBOL_at                       64    /*@*/
#define MDPL_COMPILER_SYMBOL_openSquareBracket        91    /*[*/
#define MDPL_COMPILER_SYMBOL_backSlash                92    /*\*/
#define MDPL_COMPILER_SYMBOL_closeSquareBracket       93    /*]*/
#define MDPL_COMPILER_SYMBOL_xor                      94    /*^*/
#define MDPL_COMPILER_SYMBOL_grave                    96    /*`*/
#define MDPL_COMPILER_SYMBOL_openCurlyBrace           123   /*{*/
#define MDPL_COMPILER_SYMBOL_or                       124   /*|*/
#define MDPL_COMPILER_SYMBOL_closeCurlyBrace          125   /*}*/
#define MDPL_COMPILER_SYMBOL_not                      126   /*~*/
#define MDPL_COMPILER_SYMBOL_doubleAnd                9766  /*&&*/
#define MDPL_COMPILER_SYMBOL_exponent                 10794 /****/
#define MDPL_COMPILER_SYMBOL_increment                11051 /*++*/
#define MDPL_COMPILER_SYMBOL_decrement                11565 /*--*/
#define MDPL_COMPILER_SYMBOL_doubleDot                11822 /*..*/
#define MDPL_COMPILER_SYMBOL_doubleColons             14906 /*::*/
#define MDPL_COMPILER_SYMBOL_shiftLeft                15420 /*<<*/
#define MDPL_COMPILER_SYMBOL_doesNotEqual             15649 /*!=*/
#define MDPL_COMPILER_SYMBOL_modEqual                 15653 /*%=*/
#define MDPL_COMPILER_SYMBOL_andEqual                 15654 /*&=*/
#define MDPL_COMPILER_SYMBOL_starEqual                15658 /**=*/
#define MDPL_COMPILER_SYMBOL_plusEqual                15659 /*+=*/
#define MDPL_COMPILER_SYMBOL_minusEqual               15661 /*-=*/
#define MDPL_COMPILER_SYMBOL_lessEqual                15676 /*<=*/
#define MDPL_COMPILER_SYMBOL_doubleEqual              15677 /*==*/
#define MDPL_COMPILER_SYMBOL_greaterEqual             15678 /*>=*/
#define MDPL_COMPILER_SYMBOL_notEqual                 15742 /*~=*/
#define MDPL_COMPILER_SYMBOL_arrow                    15917 /*->*/
#define MDPL_COMPILER_SYMBOL_shiftRight               15934 /*>>*/
#define MDPL_COMPILER_SYMBOL_doubleOpenSquareBracket  23387 /*[[*/
#define MDPL_COMPILER_SYMBOL_doubleCloseSquareBracket 23901 /*]]*/
#define MDPL_COMPILER_SYMBOL_doubleXor                24158 /*^^*/
#define MDPL_COMPILER_SYMBOL_doubleOpenCurlyBrace     31611 /*{{*/
#define MDPL_COMPILER_SYMBOL_doubleOR                 31868 /*||*/
#define MDPL_COMPILER_SYMBOL_doubleCloseCurlyBrace    32125 /*}}*/

//convert a null terminated string to the symbol int
uint32_t stringToSingleSymbol(const char* s);
uint32_t stringToDoubleSymbol(const char* s);
bool isValidSingleSymbol(uint32_t);
bool isValidDoubleSymbol(uint32_t);
//convert a symbol int to a string
const char* symbolToString(uint32_t symbol);

typedef struct /*Token*/ {
    uint32_t type;
    uint32_t statmentIndex;
    union
    {
        const char* str;
        uint32_t symbol;
    } data;
    uint32_t lineNum;
} Token;

typedef struct /*Statment*/
{
    uint32_t startTokenIndex;
    uint32_t endTokenIndex;
    uint32_t deepestScopeIndex;
} Statment;

typedef struct /*Scope*/
{
    uint32_t startTokenIndex;
    uint32_t endTokenIndex;
    uint32_t startStatmentIndex;
    uint32_t endStatmentIndex;
} Scope;

MDPL_ERROR_Error* readFile(const char* fName, char** str, uint32_t* length);
MDPL_ERROR_Error* tokenise(char* str, const uint32_t length, Token** tokenList, uint32_t* tokenListLength, Statment** statmentList, uint32_t* statmentListLength);
MDPL_ERROR_Error* groupScopes(char* str, const uint32_t length, Token* tokenList, const uint32_t tokenListLength, Statment* statmentList, const uint32_t statmentListLength, Scope** scopeList);

#endif //TOKENISER_HEADER_GUARD