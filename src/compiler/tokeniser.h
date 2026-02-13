#ifndef TOKENISER_HEADER_GUARD
#define TOKENISER_HEADER_GUARD

#include "../pch.h"
#include "../common/mdpl_common.h"
#include "../runtime_lib/mdpl_error.h"

#include "symbol_table.h"

#define MDPL_COMPILER_TOKEN_TYPE_undefined    0
#define MDPL_COMPILER_TOKEN_TYPE_unknown      1
#define MDPL_COMPILER_TOKEN_TYPE_punctuation  2
#define MDPL_COMPILER_TOKEN_TYPE_symbol       3
#define MDPL_COMPILER_TOKEN_TYPE_number       4
#define MDPL_COMPILER_TOKEN_TYPE_string       5
#define MDPL_COMPILER_TOKEN_TYPE_character    6

//the possible valid punctuations are !, #, $, %, &, (, ), *, +, ,, -, ., /, :, <, =, >, ?, &, @, [, \, ], ^, `, {, |, }, ~,
//                                    &&, **, ++, --, .., ::, <<, !=, %=, &=, *=,  +=, -=,  <=,  ==,  >=,  ~=,  ->,  >>,  [[,  ]],  ^^,  {{,  ||,  }}
#define MDPL_COMPILER_PUNCTUATION_exclamation              33    /*!*/
#define MDPL_COMPILER_PUNCTUATION_hash                     35    /*#*/
#define MDPL_COMPILER_PUNCTUATION_dollar                   36    /*$*/
#define MDPL_COMPILER_PUNCTUATION_mod                      37    /*%*/
#define MDPL_COMPILER_PUNCTUATION_and                      38    /*&*/
#define MDPL_COMPILER_PUNCTUATION_openRoundBracket         40    /*(*/
#define MDPL_COMPILER_PUNCTUATION_closeRoundBracket        41    /*)*/
#define MDPL_COMPILER_PUNCTUATION_star                     42    /***/
#define MDPL_COMPILER_PUNCTUATION_plus                     43    /*+*/
#define MDPL_COMPILER_PUNCTUATION_comma                    44    /*,*/
#define MDPL_COMPILER_PUNCTUATION_minus                    45    /*-*/
#define MDPL_COMPILER_PUNCTUATION_dot                      46    /*.*/
#define MDPL_COMPILER_PUNCTUATION_frontSlash               47    /*/*/
#define MDPL_COMPILER_PUNCTUATION_colons                   58    /*:*/
#define MDPL_COMPILER_PUNCTUATION_lessThan                 60    /*<*/
#define MDPL_COMPILER_PUNCTUATION_equals                   61    /*=*/
#define MDPL_COMPILER_PUNCTUATION_greaterThan              62    /*>*/
#define MDPL_COMPILER_PUNCTUATION_question                 63    /*?*/
#define MDPL_COMPILER_PUNCTUATION_at                       64    /*@*/
#define MDPL_COMPILER_PUNCTUATION_openSquareBracket        91    /*[*/
#define MDPL_COMPILER_PUNCTUATION_closeSquareBracket       93    /*]*/
#define MDPL_COMPILER_PUNCTUATION_xor                      94    /*^*/
#define MDPL_COMPILER_PUNCTUATION_grave                    96    /*`*/
#define MDPL_COMPILER_PUNCTUATION_openCurlyBrace           123   /*{*/
#define MDPL_COMPILER_PUNCTUATION_or                       124   /*|*/
#define MDPL_COMPILER_PUNCTUATION_closeCurlyBrace          125   /*}*/
#define MDPL_COMPILER_PUNCTUATION_not                      126   /*~*/
#define MDPL_COMPILER_PUNCTUATION_doubleAnd                9766  /*&&*/
#define MDPL_COMPILER_PUNCTUATION_exponent                 10794 /****/
#define MDPL_COMPILER_PUNCTUATION_increment                11051 /*++*/
#define MDPL_COMPILER_PUNCTUATION_decrement                11565 /*--*/
#define MDPL_COMPILER_PUNCTUATION_doubleDot                11822 /*..*/
#define MDPL_COMPILER_PUNCTUATION_doubleColons             14906 /*::*/
#define MDPL_COMPILER_PUNCTUATION_shiftLeft                15420 /*<<*/
#define MDPL_COMPILER_PUNCTUATION_doesNotEqual             15649 /*!=*/
#define MDPL_COMPILER_PUNCTUATION_modEqual                 15653 /*%=*/
#define MDPL_COMPILER_PUNCTUATION_andEqual                 15654 /*&=*/
#define MDPL_COMPILER_PUNCTUATION_starEqual                15658 /**=*/
#define MDPL_COMPILER_PUNCTUATION_plusEqual                15659 /*+=*/
#define MDPL_COMPILER_PUNCTUATION_minusEqual               15661 /*-=*/
#define MDPL_COMPILER_PUNCTUATION_lessEqual                15676 /*<=*/
#define MDPL_COMPILER_PUNCTUATION_doubleEqual              15677 /*==*/
#define MDPL_COMPILER_PUNCTUATION_greaterEqual             15678 /*>=*/
#define MDPL_COMPILER_PUNCTUATION_notEqual                 15742 /*~=*/
#define MDPL_COMPILER_PUNCTUATION_arrow                    15917 /*->*/
#define MDPL_COMPILER_PUNCTUATION_shiftRight               15934 /*>>*/
#define MDPL_COMPILER_PUNCTUATION_doubleOpenSquareBracket  23387 /*[[*/
#define MDPL_COMPILER_PUNCTUATION_doubleCloseSquareBracket 23901 /*]]*/
#define MDPL_COMPILER_PUNCTUATION_doubleXor                24158 /*^^*/
#define MDPL_COMPILER_PUNCTUATION_doubleOpenCurlyBrace     31611 /*{{*/
#define MDPL_COMPILER_PUNCTUATION_doubleOR                 31868 /*||*/
#define MDPL_COMPILER_PUNCTUATION_doubleCloseCurlyBrace    32125 /*}}*/

//convert a null terminated string to the punctuation int
uint32_t stringToSinglePunctuation(const char* s);
uint32_t stringToDoublePunctuation(const char* s);
bool isValidSinglePunctuation(const uint32_t);
bool isValidDoublePunctuation(const uint32_t);
//convert a punctuation int to a string
const char* punctuationToString(uint32_t punctuation);

typedef struct Token
{
    uint32_t type;
    uint32_t statmentIndex;
    union
    {
        const char* str;
        uint32_t punctuation;
        Symbol symbol;
    } data;
    uint32_t lineNum;
} Token;

typedef struct Statment
{
    uint32_t startTokenIndex;
    uint32_t endTokenIndex;
    uint32_t deepestScopeIndex;
} Statment;

typedef struct Scope
{
    uint32_t startTokenIndex;
    uint32_t endTokenIndex;
    uint32_t startStatmentIndex;
    uint32_t endStatmentIndex;
} Scope;

MDPL_ERROR_Error* reallocateTokenList(Token** tokens, uint32_t count, uint32_t* capacity);
MDPL_ERROR_Error* reallocateStatmentList(Statment** statments, uint32_t count, uint32_t* capacity);
MDPL_ERROR_Error* reallocateScopeList(Scope** scopes, uint32_t count, uint32_t* capacity);

MDPL_ERROR_Error* readFile(const char* fName, char** str, uint32_t* length);
MDPL_ERROR_Error* tokenise(char* str, const uint32_t length, SymbolTable* symbolTable, Token** tokenList, uint32_t* tokenListLength, Statment** statmentList, uint32_t* statmentListLength, const char* const fName);
MDPL_ERROR_Error* groupScopes(SymbolTable* symbolTable, Token* tokenList, const uint32_t tokenListLength, Statment* statmentList, const uint32_t statmentListLength, Scope** scopeList, uint32_t* scopeListLength);

#endif //TOKENISER_HEADER_GUARD