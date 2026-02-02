#include "tokeniser.h"
#include "../common/system_headers.h"

uint32_t stringToSingleSymbol(const char* s)
{
    return (uint32_t)(*(uint8_t*)s);
}
uint32_t stringToDoubleSymbol(const char* s)
{
    return (uint32_t)(*(uint16_t*)s);
}

bool isValidSingleSymbol(uint32_t const symbol)
{
    switch (symbol)
    {
        // Single character symbols
        case MDPL_COMPILER_SYMBOL_exclamation:              /*!*/
        case MDPL_COMPILER_SYMBOL_hash:                     /*#*/
        case MDPL_COMPILER_SYMBOL_dollar:                   /*$*/
        case MDPL_COMPILER_SYMBOL_mod:                      /*%*/
        case MDPL_COMPILER_SYMBOL_and:                      /*&*/
        case MDPL_COMPILER_SYMBOL_openRoundBracket:         /*(*/
        case MDPL_COMPILER_SYMBOL_closeRoundBracket:        /*)*/
        case MDPL_COMPILER_SYMBOL_star:                     /***/
        case MDPL_COMPILER_SYMBOL_plus:                     /*+*/
        case MDPL_COMPILER_SYMBOL_comma:                    /*,*/
        case MDPL_COMPILER_SYMBOL_minus:                    /*-*/
        case MDPL_COMPILER_SYMBOL_dot:                      /*.*/
        case MDPL_COMPILER_SYMBOL_frontSlash:               /*/*/
        case MDPL_COMPILER_SYMBOL_colons:                   /*:*/
        case MDPL_COMPILER_SYMBOL_lessThan:                 /*<*/
        case MDPL_COMPILER_SYMBOL_equals:                   /*=*/
        case MDPL_COMPILER_SYMBOL_greaterThan:              /*>*/
        case MDPL_COMPILER_SYMBOL_question:                 /*?*/
        case MDPL_COMPILER_SYMBOL_at:                       /*@*/
        case MDPL_COMPILER_SYMBOL_openSquareBracket:        /*[*/
        case MDPL_COMPILER_SYMBOL_closeSquareBracket:       /*]*/
        case MDPL_COMPILER_SYMBOL_xor:                      /*^*/
        case MDPL_COMPILER_SYMBOL_grave:                    /*`*/
        case MDPL_COMPILER_SYMBOL_openCurlyBrace:           /*{*/
        case MDPL_COMPILER_SYMBOL_or:                       /*|*/
        case MDPL_COMPILER_SYMBOL_closeCurlyBrace:          /*}*/
        case MDPL_COMPILER_SYMBOL_not:                      /*~*/
            return true;
        default:
            return false;
    }
}
bool isValidDoubleSymbol(uint32_t const symbol)
{
    switch (symbol)
    {
        // Double character symbols
        case MDPL_COMPILER_SYMBOL_doubleAnd:                /*&&*/
        case MDPL_COMPILER_SYMBOL_exponent:                 /****/
        case MDPL_COMPILER_SYMBOL_increment:                /*++*/
        case MDPL_COMPILER_SYMBOL_decrement:                /*--*/
        case MDPL_COMPILER_SYMBOL_doubleDot:                /*..*/
        case MDPL_COMPILER_SYMBOL_doubleColons:             /*::*/
        case MDPL_COMPILER_SYMBOL_shiftLeft:                /*<<*/
        case MDPL_COMPILER_SYMBOL_doesNotEqual:             /*!=*/
        case MDPL_COMPILER_SYMBOL_modEqual:                 /*%=*/
        case MDPL_COMPILER_SYMBOL_andEqual:                 /*&=*/
        case MDPL_COMPILER_SYMBOL_starEqual:                /**=*/
        case MDPL_COMPILER_SYMBOL_plusEqual:                /*+=*/
        case MDPL_COMPILER_SYMBOL_minusEqual:               /*-=*/
        case MDPL_COMPILER_SYMBOL_lessEqual:                /*<=*/
        case MDPL_COMPILER_SYMBOL_doubleEqual:              /*==*/
        case MDPL_COMPILER_SYMBOL_greaterEqual:             /*>=*/
        case MDPL_COMPILER_SYMBOL_notEqual:                 /*~=*/
        case MDPL_COMPILER_SYMBOL_arrow:                    /*->*/
        case MDPL_COMPILER_SYMBOL_shiftRight:               /*>>*/
        case MDPL_COMPILER_SYMBOL_doubleOpenSquareBracket:  /*[[*/
        case MDPL_COMPILER_SYMBOL_doubleCloseSquareBracket: /*]]*/
        case MDPL_COMPILER_SYMBOL_doubleXor:                /*^^*/
        case MDPL_COMPILER_SYMBOL_doubleOpenCurlyBrace:     /*{{*/
        case MDPL_COMPILER_SYMBOL_doubleOR:                 /*||*/
        case MDPL_COMPILER_SYMBOL_doubleCloseCurlyBrace:    /*}}*/
            return true;
        default:
            return false;
    }
}
const char* symbolToString(uint32_t symbol)
{
    switch (symbol)
    {
        // Single character symbols
        case MDPL_COMPILER_SYMBOL_exclamation:              return "!";
        case MDPL_COMPILER_SYMBOL_hash:                     return "#";
        case MDPL_COMPILER_SYMBOL_dollar:                   return "$";
        case MDPL_COMPILER_SYMBOL_mod:                      return "%";
        case MDPL_COMPILER_SYMBOL_and:                      return "&";
        case MDPL_COMPILER_SYMBOL_openRoundBracket:         return "(";
        case MDPL_COMPILER_SYMBOL_closeRoundBracket:        return ")";
        case MDPL_COMPILER_SYMBOL_star:                     return "*";
        case MDPL_COMPILER_SYMBOL_plus:                     return "+";
        case MDPL_COMPILER_SYMBOL_comma:                    return ",";
        case MDPL_COMPILER_SYMBOL_minus:                    return "-";
        case MDPL_COMPILER_SYMBOL_dot:                      return ".";
        case MDPL_COMPILER_SYMBOL_frontSlash:               return "/";
        case MDPL_COMPILER_SYMBOL_colons:                   return ":";
        case MDPL_COMPILER_SYMBOL_lessThan:                 return "<";
        case MDPL_COMPILER_SYMBOL_equals:                   return "=";
        case MDPL_COMPILER_SYMBOL_greaterThan:              return ">";
        case MDPL_COMPILER_SYMBOL_question:                 return "?";
        case MDPL_COMPILER_SYMBOL_at:                       return "@";
        case MDPL_COMPILER_SYMBOL_openSquareBracket:        return "[";
        case MDPL_COMPILER_SYMBOL_closeSquareBracket:       return "]";
        case MDPL_COMPILER_SYMBOL_xor:                      return "^";
        case MDPL_COMPILER_SYMBOL_grave:                    return "`";
        case MDPL_COMPILER_SYMBOL_openCurlyBrace:           return "{";
        case MDPL_COMPILER_SYMBOL_or:                       return "|";
        case MDPL_COMPILER_SYMBOL_closeCurlyBrace:          return "}";
        case MDPL_COMPILER_SYMBOL_not:                      return "~";
        // Double character symbols
        case MDPL_COMPILER_SYMBOL_doubleAnd:                return "&&";
        case MDPL_COMPILER_SYMBOL_exponent:                 return "**";
        case MDPL_COMPILER_SYMBOL_increment:                return "++";
        case MDPL_COMPILER_SYMBOL_decrement:                return "--";
        case MDPL_COMPILER_SYMBOL_doubleDot:                return "..";
        case MDPL_COMPILER_SYMBOL_doubleColons:             return "::";
        case MDPL_COMPILER_SYMBOL_shiftLeft:                return "<<";
        case MDPL_COMPILER_SYMBOL_doesNotEqual:             return "!=";
        case MDPL_COMPILER_SYMBOL_modEqual:                 return "%=";
        case MDPL_COMPILER_SYMBOL_andEqual:                 return "&=";
        case MDPL_COMPILER_SYMBOL_starEqual:                return "*=";
        case MDPL_COMPILER_SYMBOL_plusEqual:                return "+=";
        case MDPL_COMPILER_SYMBOL_minusEqual:               return "-=";
        case MDPL_COMPILER_SYMBOL_lessEqual:                return "<=";
        case MDPL_COMPILER_SYMBOL_doubleEqual:              return "==";
        case MDPL_COMPILER_SYMBOL_greaterEqual:             return ">=";
        case MDPL_COMPILER_SYMBOL_notEqual:                 return "~=";
        case MDPL_COMPILER_SYMBOL_arrow:                    return "->";
        case MDPL_COMPILER_SYMBOL_shiftRight:               return ">>";
        case MDPL_COMPILER_SYMBOL_doubleOpenSquareBracket:  return "[[";
        case MDPL_COMPILER_SYMBOL_doubleCloseSquareBracket: return "]]";
        case MDPL_COMPILER_SYMBOL_doubleXor:                return "^^";
        case MDPL_COMPILER_SYMBOL_doubleOpenCurlyBrace:     return "{{";
        case MDPL_COMPILER_SYMBOL_doubleOR:                 return "||";
        case MDPL_COMPILER_SYMBOL_doubleCloseCurlyBrace:    return "}}";
        default:                                            return "Unrecognised_symbol";
    }
}

uint32_t stringToKeyword(const char* s)
{
    if (strcmp(s, "namespace") == 0) return MDPL_COMPILER_KEYWORD_namespace;
    if (strcmp(s, "public") == 0)    return MDPL_COMPILER_KEYWORD_public;
    if (strcmp(s, "protected") == 0) return MDPL_COMPILER_KEYWORD_protected;
    if (strcmp(s, "code") == 0)      return MDPL_COMPILER_KEYWORD_code;
    if (strcmp(s, "fn") == 0)        return MDPL_COMPILER_KEYWORD_fn;
    if (strcmp(s, "sub") == 0)       return MDPL_COMPILER_KEYWORD_sub;
    if (strcmp(s, "ret") == 0)       return MDPL_COMPILER_KEYWORD_ret;
    if (strcmp(s, "ref") == 0)       return MDPL_COMPILER_KEYWORD_ref;
    if (strcmp(s, "generic") == 0)   return MDPL_COMPILER_KEYWORD_generic;
    if (strcmp(s, "prop") == 0)      return MDPL_COMPILER_KEYWORD_prop;
    if (strcmp(s, "catch") == 0)     return MDPL_COMPILER_KEYWORD_catch;
    if (strcmp(s, "if") == 0)        return MDPL_COMPILER_KEYWORD_if;
    if (strcmp(s, "elif") == 0)      return MDPL_COMPILER_KEYWORD_elif;
    if (strcmp(s, "else") == 0)      return MDPL_COMPILER_KEYWORD_else;
    if (strcmp(s, "for") == 0)       return MDPL_COMPILER_KEYWORD_for;
    if (strcmp(s, "while") == 0)     return MDPL_COMPILER_KEYWORD_while;
    if (strcmp(s, "from") == 0)      return MDPL_COMPILER_KEYWORD_from;
    if (strcmp(s, "to") == 0)        return MDPL_COMPILER_KEYWORD_to;
    if (strcmp(s, "step") == 0)      return MDPL_COMPILER_KEYWORD_step;
    if (strcmp(s, "continue") == 0)  return MDPL_COMPILER_KEYWORD_continue;
    if (strcmp(s, "break") == 0)     return MDPL_COMPILER_KEYWORD_break;
    if (strcmp(s, "switch") == 0)    return MDPL_COMPILER_KEYWORD_switch;
    if (strcmp(s, "case") == 0)      return MDPL_COMPILER_KEYWORD_case;
    if (strcmp(s, "default") == 0)   return MDPL_COMPILER_KEYWORD_default;
    if (strcmp(s, "let") == 0)       return MDPL_COMPILER_KEYWORD_let;
    if (strcmp(s, "mut") == 0)       return MDPL_COMPILER_KEYWORD_mut;
    if (strcmp(s, "auto") == 0)      return MDPL_COMPILER_KEYWORD_auto;
    if (strcmp(s, "struct") == 0)    return MDPL_COMPILER_KEYWORD_struct;
    if (strcmp(s, "enum") == 0)      return MDPL_COMPILER_KEYWORD_enum;
    if (strcmp(s, "bitfield") == 0)  return MDPL_COMPILER_KEYWORD_bitfield;
    if (strcmp(s, "using") == 0)     return MDPL_COMPILER_KEYWORD_using;
    if (strcmp(s, "and") == 0)       return MDPL_COMPILER_KEYWORD_and;
    if (strcmp(s, "or") == 0)        return MDPL_COMPILER_KEYWORD_or;
    if (strcmp(s, "not") == 0)       return MDPL_COMPILER_KEYWORD_not;
    if (strcmp(s, "true") == 0)      return MDPL_COMPILER_KEYWORD_true;
    if (strcmp(s, "false") == 0)     return MDPL_COMPILER_KEYWORD_false;
    if (strcmp(s, "sizeof") == 0)    return MDPL_COMPILER_KEYWORD_sizeof;
    if (strcmp(s, "include") == 0)   return MDPL_COMPILER_KEYWORD_include;
    // C keywords not used in mdpl
    if (strcmp(s, "return") == 0)    return MDPL_COMPILER_KEYWORD_return;
    if (strcmp(s, "static") == 0)    return MDPL_COMPILER_KEYWORD_static;
    if (strcmp(s, "typedef") == 0)   return MDPL_COMPILER_KEYWORD_typedef;
    if (strcmp(s, "const") == 0)     return MDPL_COMPILER_KEYWORD_const;
    if (strcmp(s, "endif") == 0)     return MDPL_COMPILER_KEYWORD_endif;
    if (strcmp(s, "define") == 0)    return MDPL_COMPILER_KEYWORD_define;
    if (strcmp(s, "defined") == 0)   return MDPL_COMPILER_KEYWORD_defined;
    if (strcmp(s, "undef") == 0)     return MDPL_COMPILER_KEYWORD_undef;
    if (strcmp(s, "ifdef") == 0)     return MDPL_COMPILER_KEYWORD_ifdef;
    if (strcmp(s, "ifndef") == 0)    return MDPL_COMPILER_KEYWORD_ifndef;
    return MDPL_COMPILER_KEYWORD_undefined;
}
bool isValidKeyword(const uint32_t keyword)
{
    return keyword != MDPL_COMPILER_KEYWORD_undefined;
}
const char* keywordToString(const uint32_t keyword)
{
    switch (keyword)
    {
        case MDPL_COMPILER_KEYWORD_undefined:  return "Undefined_keyword";
        case MDPL_COMPILER_KEYWORD_namespace:  return "namespace";
        case MDPL_COMPILER_KEYWORD_public:     return "public";
        case MDPL_COMPILER_KEYWORD_protected:  return "protected";
        case MDPL_COMPILER_KEYWORD_code:       return "code";
        case MDPL_COMPILER_KEYWORD_fn:         return "fn";
        case MDPL_COMPILER_KEYWORD_sub:        return "sub";
        case MDPL_COMPILER_KEYWORD_ret:        return "ret";
        case MDPL_COMPILER_KEYWORD_ref:        return "ref";
        case MDPL_COMPILER_KEYWORD_generic:    return "generic";
        case MDPL_COMPILER_KEYWORD_prop:       return "prop";
        case MDPL_COMPILER_KEYWORD_catch:      return "catch";
        case MDPL_COMPILER_KEYWORD_if:         return "if";
        case MDPL_COMPILER_KEYWORD_elif:       return "elif";
        case MDPL_COMPILER_KEYWORD_else:       return "else";
        case MDPL_COMPILER_KEYWORD_for:        return "for";
        case MDPL_COMPILER_KEYWORD_while:      return "while";
        case MDPL_COMPILER_KEYWORD_from:       return "from";
        case MDPL_COMPILER_KEYWORD_to:         return "to";
        case MDPL_COMPILER_KEYWORD_step:       return "step";
        case MDPL_COMPILER_KEYWORD_continue:   return "continue";
        case MDPL_COMPILER_KEYWORD_break:      return "break";
        case MDPL_COMPILER_KEYWORD_switch:     return "switch";
        case MDPL_COMPILER_KEYWORD_case:       return "case";
        case MDPL_COMPILER_KEYWORD_default:    return "default";
        case MDPL_COMPILER_KEYWORD_let:        return "let";
        case MDPL_COMPILER_KEYWORD_mut:        return "mut";
        case MDPL_COMPILER_KEYWORD_auto:       return "auto";
        case MDPL_COMPILER_KEYWORD_struct:     return "struct";
        case MDPL_COMPILER_KEYWORD_enum:       return "enum";
        case MDPL_COMPILER_KEYWORD_bitfield:   return "bitfield";
        case MDPL_COMPILER_KEYWORD_using:      return "using";
        case MDPL_COMPILER_KEYWORD_and:        return "and";
        case MDPL_COMPILER_KEYWORD_or:         return "or";
        case MDPL_COMPILER_KEYWORD_not:        return "not";
        case MDPL_COMPILER_KEYWORD_true:       return "true";
        case MDPL_COMPILER_KEYWORD_false:      return "false";
        case MDPL_COMPILER_KEYWORD_sizeof:     return "sizeof";
        case MDPL_COMPILER_KEYWORD_include:    return "include";
        // C keywords not used in mdpl
        case MDPL_COMPILER_KEYWORD_return:     return "return";
        case MDPL_COMPILER_KEYWORD_static:     return "static";
        case MDPL_COMPILER_KEYWORD_typedef:    return "typedef";
        case MDPL_COMPILER_KEYWORD_const:      return "const";
        case MDPL_COMPILER_KEYWORD_endif:      return "endif";
        case MDPL_COMPILER_KEYWORD_define:     return "define";
        case MDPL_COMPILER_KEYWORD_defined:    return "defined";
        case MDPL_COMPILER_KEYWORD_undef:      return "undef";
        case MDPL_COMPILER_KEYWORD_ifdef:      return "ifdef";
        case MDPL_COMPILER_KEYWORD_ifndef:     return "ifndef";
        default:                               return "Unrecognised_keyword";
    }
}

uint32_t stringToType(const char* s)
{
    if (strcmp(s, "byte") == 0)     return MDPL_COMPILER_TYPE_byte;
    if (strcmp(s, "int8") == 0)     return MDPL_COMPILER_TYPE_int8;
    if (strcmp(s, "uint8") == 0)    return MDPL_COMPILER_TYPE_uint8;
    if (strcmp(s, "int16") == 0)    return MDPL_COMPILER_TYPE_int16;
    if (strcmp(s, "uint16") == 0)   return MDPL_COMPILER_TYPE_uint16;
    if (strcmp(s, "int32") == 0)    return MDPL_COMPILER_TYPE_int32;
    if (strcmp(s, "uint32") == 0)   return MDPL_COMPILER_TYPE_uint32;
    if (strcmp(s, "int64") == 0)    return MDPL_COMPILER_TYPE_int64;
    if (strcmp(s, "uint64") == 0)   return MDPL_COMPILER_TYPE_uint64;
    if (strcmp(s, "float32") == 0)  return MDPL_COMPILER_TYPE_float32;
    if (strcmp(s, "float64") == 0)  return MDPL_COMPILER_TYPE_float64;
    if (strcmp(s, "string") == 0)   return MDPL_COMPILER_TYPE_string;
    if (strcmp(s, "bool") == 0)     return MDPL_COMPILER_TYPE_bool;
    // C types not used in mdpl
    if (strcmp(s, "int") == 0)      return MDPL_COMPILER_TYPE_int;
    if (strcmp(s, "float") == 0)    return MDPL_COMPILER_TYPE_float;
    if (strcmp(s, "double") == 0)   return MDPL_COMPILER_TYPE_double;
    if (strcmp(s, "int8_t") == 0)   return MDPL_COMPILER_TYPE_int8_t;
    if (strcmp(s, "uint8_t") == 0)  return MDPL_COMPILER_TYPE_uint8_t;
    if (strcmp(s, "int16_t") == 0)  return MDPL_COMPILER_TYPE_int16_t;
    if (strcmp(s, "uint16_t") == 0) return MDPL_COMPILER_TYPE_uint16_t;
    if (strcmp(s, "int32_t") == 0)  return MDPL_COMPILER_TYPE_int32_t;
    if (strcmp(s, "uint32_t") == 0) return MDPL_COMPILER_TYPE_uint32_t;
    if (strcmp(s, "int64_t") == 0)  return MDPL_COMPILER_TYPE_int64_t;
    if (strcmp(s, "uint64_t") == 0) return MDPL_COMPILER_TYPE_uint64_t;
    if (strcmp(s, "size_t") == 0)   return MDPL_COMPILER_TYPE_size_t;
    return MDPL_COMPILER_TYPE_undefined;
}
bool isValidType(const uint32_t type)
{
    return type != MDPL_COMPILER_TYPE_undefined;
}
const char* typeToString(const uint32_t type)
{
    switch (type)
    {
        case MDPL_COMPILER_TYPE_undefined: return "Undefined_type";
        case MDPL_COMPILER_TYPE_byte:      return "byte";
        case MDPL_COMPILER_TYPE_int8:      return "int8";
        case MDPL_COMPILER_TYPE_uint8:     return "uint8";
        case MDPL_COMPILER_TYPE_int16:     return "int16";
        case MDPL_COMPILER_TYPE_uint16:    return "uint16";
        case MDPL_COMPILER_TYPE_int32:     return "int32";
        case MDPL_COMPILER_TYPE_uint32:    return "uint32";
        case MDPL_COMPILER_TYPE_int64:     return "int64";
        case MDPL_COMPILER_TYPE_uint64:    return "uint64";
        case MDPL_COMPILER_TYPE_float32:   return "float32";
        case MDPL_COMPILER_TYPE_float64:   return "float64";
        case MDPL_COMPILER_TYPE_string:    return "string";
        case MDPL_COMPILER_TYPE_bool:      return "bool";
        // C types not used in mdpl
        case MDPL_COMPILER_TYPE_int:       return "int";
        case MDPL_COMPILER_TYPE_float:     return "float";
        case MDPL_COMPILER_TYPE_double:    return "double";
        case MDPL_COMPILER_TYPE_int8_t:    return "int8_t";
        case MDPL_COMPILER_TYPE_uint8_t:   return "uint8_t";
        case MDPL_COMPILER_TYPE_int16_t:   return "int16_t";
        case MDPL_COMPILER_TYPE_uint16_t:  return "uint16_t";
        case MDPL_COMPILER_TYPE_int32_t:   return "int32_t";
        case MDPL_COMPILER_TYPE_uint32_t:  return "uint32_t";
        case MDPL_COMPILER_TYPE_int64_t:   return "int64_t";
        case MDPL_COMPILER_TYPE_uint64_t:  return "uint64_t";
        case MDPL_COMPILER_TYPE_size_t:    return "size_t";
        default:                           return "Unrecognised_type";
    }
}

//increase the side of an array list of type Token
MDPL_ERROR_Error* reallocateTokenList(Token** tokens, uint32_t count, uint32_t* capacity)
{
    if (count >= *capacity)
    {
        *capacity *= 2;
        Token* newTokens = (Token*)realloc(*tokens, *capacity * sizeof(Token));
        if (newTokens == nullptr)
        {
            free(*tokens);
            *tokens = nullptr;
            return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_ALLOCATION, "tokenise: failed to reallocate token list");
        }
        *tokens = newTokens;
    }
    return nullptr;
}
//increase the side of an array list of type Statment
MDPL_ERROR_Error* reallocateStatmentList(Statment** statments, uint32_t count, uint32_t* capacity)
{
    if (count >= *capacity)
    {
        *capacity *= 2;
        Statment* newStatments = (Statment*)realloc(*statments, *capacity * sizeof(Statment));
        if (newStatments == nullptr)
        {
            free(*statments);
            *statments = nullptr;
            return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_ALLOCATION, "tokenise: failed to reallocate statment list");
        }
        *statments = newStatments;
    }
    return nullptr;
}
//create a token and add it to the token list, checking if it's a keyword or type
MDPL_ERROR_Error* createStandardToken(Token** tokens, uint32_t* tokenCount, uint32_t* tokenCapacity, uint32_t statmentIndex, const char* str, uint32_t lineNum)
{
    MDPL_RETERR(reallocateTokenList(tokens, *tokenCount, tokenCapacity));
    (*tokens)[*tokenCount].statmentIndex = statmentIndex;
    (*tokens)[*tokenCount].lineNum = lineNum;

    uint32_t keyword = stringToKeyword(str);
    uint32_t type = stringToType(str);

    if (isValidKeyword(keyword))
    {
        (*tokens)[*tokenCount].type = MDPL_COMPILER_TOKEN_TYPE_keyword;
        (*tokens)[*tokenCount].data.keyword = keyword;
    }
    else if (isValidType(type))
    {
        (*tokens)[*tokenCount].type = MDPL_COMPILER_TOKEN_TYPE_type;
        (*tokens)[*tokenCount].data.type = type;
    }
    else
    {
        (*tokens)[*tokenCount].type = MDPL_COMPILER_TOKEN_TYPE_unknown;
        (*tokens)[*tokenCount].data.str = str;
    }

    (*tokenCount)++;
    return nullptr;
}

MDPL_ERROR_Error* readFile(const char* fName, char** str, uint32_t* length)
{
    // Validate input parameters
    if (fName == nullptr || str == nullptr || length == nullptr)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "readFile: null parameter provided");
    }

    // Check file existence
    if (access(fName, F_OK) != 0)
    {
        return MDPL_ERROR_TYPE_MSG_RET(MDPL_ERROR_TYPE_EXTERNAL, "readFile: file does not exist", MDPL_ERROR_EXIT_CANNOT_OPEN_FILE);
    }

    // Check read permission
    if (access(fName, R_OK) != 0)
    {
        return MDPL_ERROR_TYPE_MSG_RET(MDPL_ERROR_TYPE_EXTERNAL, "readFile: permission denied", MDPL_ERROR_EXIT_PERMISSION_DENIED);
    }

    // Open file
    FILE* file = fopen(fName, "rb");
    if (file == nullptr)
    {
        return MDPL_ERROR_TYPE_MSG_RET(MDPL_ERROR_TYPE_EXTERNAL, "readFile: failed to open file", MDPL_ERROR_EXIT_CANNOT_OPEN_FILE);
    }

    // Get file size
    if (fseek(file, 0, SEEK_END) != 0)
    {
        fclose(file);
        return MDPL_ERROR_TYPE_MSG_RET(MDPL_ERROR_TYPE_EXTERNAL, "readFile: failed to seek to end of file", MDPL_ERROR_EXIT_IO_ERROR);
    }

    long fileSize = ftell(file);
    if (fileSize < 0)
    {
        fclose(file);
        return MDPL_ERROR_TYPE_MSG_RET(MDPL_ERROR_TYPE_EXTERNAL, "readFile: failed to get file size", MDPL_ERROR_EXIT_IO_ERROR);
    }

    if (fseek(file, 0, SEEK_SET) != 0)
    {
        fclose(file);
        return MDPL_ERROR_TYPE_MSG_RET(MDPL_ERROR_TYPE_EXTERNAL, "readFile: failed to seek to start of file", MDPL_ERROR_EXIT_IO_ERROR);
    }

    // Check file size fits in uint32_t
    if ((unsigned long)fileSize > UINT32_MAX)
    {
        fclose(file);
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_MAXIMUM_SIZE_EXCEDED, "readFile: file size exceeds uint32_t maximum");
    }

    // Allocate buffer (size + 1 for null terminator)
    char* buffer = (char*)malloc((size_t)fileSize + 1);
    if (buffer == nullptr)
    {
        fclose(file);
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_ALLOCATION, "readFile: failed to allocate buffer");
    }

    // Read file contents
    size_t bytesRead = fread(buffer, 1, (size_t)fileSize, file);
    if (bytesRead != (size_t)fileSize)
    {
        free(buffer);
        fclose(file);
        return MDPL_ERROR_TYPE_MSG_RET(MDPL_ERROR_TYPE_EXTERNAL, "readFile: failed to read entire file", MDPL_ERROR_EXIT_IO_ERROR);
    }

    // Null terminate
    buffer[fileSize] = '\0';

    // Close file
    fclose(file);

    // Set output parameters
    *str = buffer;
    *length = (uint32_t)fileSize;

    return nullptr;
}

MDPL_ERROR_Error* tokenise(char* str, const uint32_t length, Token** tokenList, uint32_t* tokenListLength, Statment** statmentList, uint32_t* statmentListLength)
{
    // Validate input parameters
    if (str == nullptr && length > 0)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "tokenise: str is null but length is non-zero");
    }
    if (tokenList == nullptr)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "tokenise: tokenList is null");
    }
    if (tokenListLength == nullptr)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "tokenise: tokenListLength is null");
    }
    if (statmentList == nullptr)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "tokenise: statmentList is null");
    }
    if (statmentListLength == nullptr)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "tokenise: statmentListLength is null");
    }

    //allocate tokens list
    uint32_t tokenCapacity = 64;
    uint32_t tokenCount = 0;
    Token* tokens = (Token*)malloc(tokenCapacity * sizeof(Token));
    if (tokens == nullptr)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_ALLOCATION, "tokenise: failed to allocate token list");
    }

    //allocate statments list
    uint32_t statmentCapacity = 64;
    uint32_t statmentCount = 0;
    Statment* statments = (Statment*)malloc(statmentCapacity * sizeof(Statment));
    if (statments == nullptr)
    {
        free(tokens);
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_ALLOCATION, "tokenise: failed to allocate statment list");
    }

    uint32_t lineCount = 1;
    uint32_t nextTokenStartIndex = 0;
    uint32_t statmentStartTokenIndex = 0;
    bool lastTokenWasComma = false;

    for (uint32_t i = 0; i < length; i++)
    {
        if (str[i] == '\n')
        {
            str[i] = '\0';

            // Create token for content before the newline if any
            if (nextTokenStartIndex < i)
            {
                MDPL_RETERR(createStandardToken(&tokens, &tokenCount, &tokenCapacity, statmentCount, str + nextTokenStartIndex, lineCount));
                lastTokenWasComma = false;
            }

            lineCount++;

            // End statement on newline if previous token wasn't a comma
            if (!lastTokenWasComma && statmentStartTokenIndex < tokenCount)
            {
                MDPL_RETERR(reallocateStatmentList(&statments, statmentCount, &statmentCapacity));
                statments[statmentCount].startTokenIndex = statmentStartTokenIndex;
                statments[statmentCount].endTokenIndex = tokenCount;
                statments[statmentCount].deepestScopeIndex = 0;
                statmentCount++;
                statmentStartTokenIndex = tokenCount;
            }

            nextTokenStartIndex = i + 1;
        }
        //number literals
        else if((nextTokenStartIndex == i) && (str[i] >= '0') && (str[i] <= '9'))
        {
            bool hasDecimalPoint = false;
            while(true)
            {
                if((i < length) && (str[i] >= '0') && (str[i] <= '9'))
                {
                    i++;
                }
                else if((i < length) && (str[i] == '.') && (!hasDecimalPoint))
                {
                    i++;
                    hasDecimalPoint = true;
                }
                else
                {
                    break;
                }
            }
            MDPL_RETERR(reallocateTokenList(&tokens, tokenCount, &tokenCapacity));
            tokens[tokenCount].type = MDPL_COMPILER_TOKEN_TYPE_number;
            tokens[tokenCount].statmentIndex = statmentCount;
            tokens[tokenCount].data.str = str + nextTokenStartIndex;
            tokens[tokenCount].lineNum = lineCount;
            tokenCount++;
            lastTokenWasComma = false;
            nextTokenStartIndex = i;
            i--; // Reprocess the character that ended the number
        }
        //string literals
        else if((i > 0) && (str[i] == '\"') && (str[i-1] != '\\'))
        {
            // Create token for content before the string if any
            if (nextTokenStartIndex < i)
            {
                MDPL_RETERR(createStandardToken(&tokens, &tokenCount, &tokenCapacity, statmentCount, str + nextTokenStartIndex, lineCount));
            }

            str[i] = '\0';
            nextTokenStartIndex = i + 1;
            while (i < length && str[i] != '\"')
            {
                if(str[i] == '\n')
                {
                    lineCount++;
                }
                i++;
            }
            str[i] = '\0';
            MDPL_RETERR(reallocateTokenList(&tokens, tokenCount, &tokenCapacity));
            tokens[tokenCount].type = MDPL_COMPILER_TOKEN_TYPE_string;
            tokens[tokenCount].statmentIndex = statmentCount;
            tokens[tokenCount].data.str = str + nextTokenStartIndex;
            tokens[tokenCount].lineNum = lineCount;
            tokenCount++;
            lastTokenWasComma = false;
            nextTokenStartIndex = i + 1;
        }
        //character literals
        else if((i > 0) && (str[i] == '\'') && (str[i-1] != '\\'))
        {
            // Create token for content before the string if any
            if (nextTokenStartIndex < i)
            {
                MDPL_RETERR(createStandardToken(&tokens, &tokenCount, &tokenCapacity, statmentCount, str + nextTokenStartIndex, lineCount));
            }

            str[i] = '\0';
            nextTokenStartIndex = i + 1;
            i++; // Move past the opening quote
            while (i < length)
            {
                if(str[i] == '\\' && i + 1 < length)
                {
                    // Skip the escape sequence
                    if(str[i + 1] == '\n')
                    {
                        lineCount++;
                    }
                    i++;
                }
                else if(str[i] == '\'')
                {
                    break;
                }
                else if(str[i] == '\n')
                {
                    lineCount++;
                }
                i++;
            }
            str[i] = '\0';
            MDPL_RETERR(reallocateTokenList(&tokens, tokenCount, &tokenCapacity));
            tokens[tokenCount].type = MDPL_COMPILER_TOKEN_TYPE_character;
            tokens[tokenCount].statmentIndex = statmentCount;
            tokens[tokenCount].data.str = str + nextTokenStartIndex;
            tokens[tokenCount].lineNum = lineCount;
            tokenCount++;
            lastTokenWasComma = false;
            nextTokenStartIndex = i + 1;
        }
        // Check for line comment //
        else if (i + 1 < length && str[i] == '/' && str[i + 1] == '/')
        {
            while (i < length && str[i] != '\n')
            {
                i++;
            }
            if (i < length && str[i] == '\n')
            {
                str[i] = '\0';
                lineCount++;

                // End statement on newline if previous token wasn't a comma
                if (!lastTokenWasComma && statmentStartTokenIndex < tokenCount)
                {
                    MDPL_RETERR(reallocateStatmentList(&statments, statmentCount, &statmentCapacity));
                    statments[statmentCount].startTokenIndex = statmentStartTokenIndex;
                    statments[statmentCount].endTokenIndex = tokenCount;
                    statments[statmentCount].deepestScopeIndex = 0;
                    statmentCount++;
                    statmentStartTokenIndex = tokenCount;
                }

                nextTokenStartIndex = i + 1;
            }
            continue;
        }
        // Check for block comment /*
        else if (i + 1 < length && str[i] == '/' && str[i + 1] == '*')
        {
            i += 2;
            while (i + 1 < length && !(str[i] == '*' && str[i + 1] == '/'))
            {
                if (str[i] == '\n')
                {
                    str[i] = '\0';
                    lineCount++;

                    // End statement on newline if previous token wasn't a comma
                    if (!lastTokenWasComma && statmentStartTokenIndex < tokenCount)
                    {
                        MDPL_RETERR(reallocateStatmentList(&statments, statmentCount, &statmentCapacity));
                        statments[statmentCount].startTokenIndex = statmentStartTokenIndex;
                        statments[statmentCount].endTokenIndex = tokenCount;
                        statments[statmentCount].deepestScopeIndex = 0;
                        statmentCount++;
                        statmentStartTokenIndex = tokenCount;
                    }

                    nextTokenStartIndex = i + 1;
                }
                i++;
            }
            if (i + 1 < length)
            {
                i++; // Skip past the closing */
            }
            nextTokenStartIndex = i + 1;
            continue;
        }
        else if (str[i] == ' ' || str[i] == '\t')
        {
            str[i] = '\0';

            // Only create a token if there's actual content
            if (nextTokenStartIndex < i)
            {
                MDPL_RETERR(createStandardToken(&tokens, &tokenCount, &tokenCapacity, statmentCount, str + nextTokenStartIndex, lineCount));
                lastTokenWasComma = false;
            }

            nextTokenStartIndex = i + 1;
        }
        else if (str[i] == ';')
        {
            str[i] = '\0';

            // Create token for text before the semicolon if any
            if (nextTokenStartIndex < i)
            {
                MDPL_RETERR(createStandardToken(&tokens, &tokenCount, &tokenCapacity, statmentCount, str + nextTokenStartIndex, lineCount));
            }

            // End statement on semicolon
            MDPL_RETERR(reallocateStatmentList(&statments, statmentCount, &statmentCapacity));
            statments[statmentCount].startTokenIndex = statmentStartTokenIndex;
            statments[statmentCount].endTokenIndex = tokenCount;
            statments[statmentCount].deepestScopeIndex = 0;
            statmentCount++;
            statmentStartTokenIndex = tokenCount;
            lastTokenWasComma = false;

            nextTokenStartIndex = i + 1;
        }
        else if (i + 1 < length && isValidDoubleSymbol(stringToDoubleSymbol(&str[i])))
        {
            uint32_t symbol = stringToDoubleSymbol(&str[i]);
            str[i] = '\0';
            str[i + 1] = '\0';

            // Create token for text before the symbol if any
            if (nextTokenStartIndex < i)
            {
                MDPL_RETERR(createStandardToken(&tokens, &tokenCount, &tokenCapacity, statmentCount, str + nextTokenStartIndex, lineCount));
            }

            MDPL_RETERR(reallocateTokenList(&tokens, tokenCount, &tokenCapacity));
            tokens[tokenCount].type = MDPL_COMPILER_TOKEN_TYPE_symbol;
            tokens[tokenCount].statmentIndex = statmentCount;
            tokens[tokenCount].data.symbol = symbol;
            tokens[tokenCount].lineNum = lineCount;
            tokenCount++;
            lastTokenWasComma = false;

            i++;
            nextTokenStartIndex = i + 1;
        }
        else if (isValidSingleSymbol(stringToSingleSymbol(&str[i])))
        {
            uint32_t symbol = stringToSingleSymbol(&str[i]);
            str[i] = '\0';

            // Create token for text before the symbol if any
            if (nextTokenStartIndex < i)
            {
                MDPL_RETERR(createStandardToken(&tokens, &tokenCount, &tokenCapacity, statmentCount, str + nextTokenStartIndex, lineCount));
            }

            MDPL_RETERR(reallocateTokenList(&tokens, tokenCount, &tokenCapacity));
            tokens[tokenCount].type = MDPL_COMPILER_TOKEN_TYPE_symbol;
            tokens[tokenCount].statmentIndex = statmentCount;
            tokens[tokenCount].data.symbol = symbol;
            tokens[tokenCount].lineNum = lineCount;
            tokenCount++;

            lastTokenWasComma = (symbol == MDPL_COMPILER_SYMBOL_comma);

            nextTokenStartIndex = i + 1;
        }
    }

    // Create token for any remaining text after the last delimiter
    if (nextTokenStartIndex < length && str[nextTokenStartIndex] != '\0')
    {
        MDPL_RETERR(createStandardToken(&tokens, &tokenCount, &tokenCapacity, statmentCount, str + nextTokenStartIndex, lineCount));
    }

    // Create final statement if there are remaining tokens
    if (statmentStartTokenIndex < tokenCount)
    {
        MDPL_RETERR(reallocateStatmentList(&statments, statmentCount, &statmentCapacity));
        statments[statmentCount].startTokenIndex = statmentStartTokenIndex;
        statments[statmentCount].endTokenIndex = tokenCount;
        statments[statmentCount].deepestScopeIndex = 0;
        statmentCount++;
    }

    *tokenList = tokens;
    *tokenListLength = tokenCount;
    *statmentList = statments;
    *statmentListLength = statmentCount;
    return nullptr;
}
MDPL_ERROR_Error* groupScopes(char* str, const uint32_t length, Token* tokenList, const uint32_t tokenListLength, Statment* statmentList, const uint32_t statmentListLength, Scope** scopeList)
{
    return nullptr;
}