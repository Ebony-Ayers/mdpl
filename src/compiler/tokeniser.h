#ifndef TOKENISER_HEADER_GUARD
#define TOKENISER_HEADER_GUARD

#include "../pch.h"
#include "../common/mdpl_common.h"
#include "../runtime_lib/mdpl_error.h"

#define MDPL_COMPILER_TOKEN_TYPE_undefined    0
#define MDPL_COMPILER_TOKEN_TYPE_unknown      1
#define MDPL_COMPILER_TOKEN_TYPE_symbol       2
#define MDPL_COMPILER_TOKEN_TYPE_keyword      3
#define MDPL_COMPILER_TOKEN_TYPE_type         4
#define MDPL_COMPILER_TOKEN_TYPE_number       5
#define MDPL_COMPILER_TOKEN_TYPE_string       6
#define MDPL_COMPILER_TOKEN_TYPE_character    7

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
bool isValidSingleSymbol(const uint32_t);
bool isValidDoubleSymbol(const uint32_t);
//convert a symbol int to a string
const char* symbolToString(uint32_t symbol);

#define MDPL_COMPILER_KEYWORD_undefined  0
#define MDPL_COMPILER_KEYWORD_namespace  1
#define MDPL_COMPILER_KEYWORD_public     2
#define MDPL_COMPILER_KEYWORD_protected  3
#define MDPL_COMPILER_KEYWORD_code       4
#define MDPL_COMPILER_KEYWORD_fn         5
#define MDPL_COMPILER_KEYWORD_sub        6
#define MDPL_COMPILER_KEYWORD_ret        7
#define MDPL_COMPILER_KEYWORD_ref        8
#define MDPL_COMPILER_KEYWORD_generic    9
#define MDPL_COMPILER_KEYWORD_prop       10
#define MDPL_COMPILER_KEYWORD_catch      11
#define MDPL_COMPILER_KEYWORD_if         12
#define MDPL_COMPILER_KEYWORD_elif       13
#define MDPL_COMPILER_KEYWORD_else       14
#define MDPL_COMPILER_KEYWORD_for        15
#define MDPL_COMPILER_KEYWORD_while      16
#define MDPL_COMPILER_KEYWORD_from       17
#define MDPL_COMPILER_KEYWORD_to         18
#define MDPL_COMPILER_KEYWORD_step       19
#define MDPL_COMPILER_KEYWORD_continue   20
#define MDPL_COMPILER_KEYWORD_break      21
#define MDPL_COMPILER_KEYWORD_switch     22
#define MDPL_COMPILER_KEYWORD_case       23
#define MDPL_COMPILER_KEYWORD_default    24
#define MDPL_COMPILER_KEYWORD_let        25
#define MDPL_COMPILER_KEYWORD_mut        26
#define MDPL_COMPILER_KEYWORD_auto       27
#define MDPL_COMPILER_KEYWORD_struct     28
#define MDPL_COMPILER_KEYWORD_enum       29
#define MDPL_COMPILER_KEYWORD_bitfield   30
#define MDPL_COMPILER_KEYWORD_using      31
#define MDPL_COMPILER_KEYWORD_and        32
#define MDPL_COMPILER_KEYWORD_or         33
#define MDPL_COMPILER_KEYWORD_not        34
#define MDPL_COMPILER_KEYWORD_true       35
#define MDPL_COMPILER_KEYWORD_false      36
#define MDPL_COMPILER_KEYWORD_sizeof     37
#define MDPL_COMPILER_KEYWORD_include    38
//c keywords not used in mdpl
#define MDPL_COMPILER_KEYWORD_return     39
#define MDPL_COMPILER_KEYWORD_static     40
#define MDPL_COMPILER_KEYWORD_typedef    41
#define MDPL_COMPILER_KEYWORD_const      42
#define MDPL_COMPILER_KEYWORD_endif      43
#define MDPL_COMPILER_KEYWORD_define     44
#define MDPL_COMPILER_KEYWORD_defined    45
#define MDPL_COMPILER_KEYWORD_undef      46
#define MDPL_COMPILER_KEYWORD_ifdef      47
#define MDPL_COMPILER_KEYWORD_ifndef     48

uint32_t stringToKeyword(const char* s);
bool isValidKeyword(const uint32_t keyword);
const char* keywordToString(const uint32_t keyword);

#define MDPL_COMPILER_TYPE_undefined  0
#define MDPL_COMPILER_TYPE_byte       1
#define MDPL_COMPILER_TYPE_int8       2
#define MDPL_COMPILER_TYPE_uint8      3
#define MDPL_COMPILER_TYPE_int16      4
#define MDPL_COMPILER_TYPE_uint16     5
#define MDPL_COMPILER_TYPE_int32      6
#define MDPL_COMPILER_TYPE_uint32     7
#define MDPL_COMPILER_TYPE_int64      8
#define MDPL_COMPILER_TYPE_uint64     9
#define MDPL_COMPILER_TYPE_float32    10
#define MDPL_COMPILER_TYPE_float64    11
#define MDPL_COMPILER_TYPE_string     12
#define MDPL_COMPILER_TYPE_bool       13
//c types not used in mdpl
#define MDPL_COMPILER_TYPE_int        14
#define MDPL_COMPILER_TYPE_float      15
#define MDPL_COMPILER_TYPE_double     16
#define MDPL_COMPILER_TYPE_int8_t     17
#define MDPL_COMPILER_TYPE_uint8_t    18
#define MDPL_COMPILER_TYPE_int16_t    19
#define MDPL_COMPILER_TYPE_uint16_t   20
#define MDPL_COMPILER_TYPE_int32_t    21
#define MDPL_COMPILER_TYPE_uint32_t   22
#define MDPL_COMPILER_TYPE_int64_t    23
#define MDPL_COMPILER_TYPE_uint64_t   24
#define MDPL_COMPILER_TYPE_size_t     25

uint32_t stringToType(const char* s);
bool isValidType(const uint32_t type);
const char* typeToString(const uint32_t type);

typedef struct /*Token*/ {
    uint32_t type;
    uint32_t statmentIndex;
    union
    {
        const char* str;
        uint32_t symbol;
        uint32_t keyword;
        uint32_t type;
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

MDPL_ERROR_Error* reallocateTokenList(Token** tokens, uint32_t count, uint32_t* capacity);
MDPL_ERROR_Error* reallocateStatmentList(Statment** statments, uint32_t count, uint32_t* capacity);
MDPL_ERROR_Error* createStandardToken(Token** tokens, uint32_t* tokenCount, uint32_t* tokenCapacity, uint32_t statmentIndex, const char* str, uint32_t lineNum);

MDPL_ERROR_Error* readFile(const char* fName, char** str, uint32_t* length);
MDPL_ERROR_Error* tokenise(char* str, const uint32_t length, Token** tokenList, uint32_t* tokenListLength, Statment** statmentList, uint32_t* statmentListLength);
MDPL_ERROR_Error* groupScopes(char* str, const uint32_t length, Token* tokenList, const uint32_t tokenListLength, Statment* statmentList, const uint32_t statmentListLength, Scope** scopeList);

#endif //TOKENISER_HEADER_GUARD