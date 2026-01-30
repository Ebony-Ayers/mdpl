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

bool isValidSingleSymbol(uint32_t symbol)
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
        case MDPL_COMPILER_SYMBOL_backSlash:                /*\*/
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
bool isValidDoubleSymbol(uint32_t symbol)
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
        case MDPL_COMPILER_SYMBOL_backSlash:                return "\\";
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
        default:                                            return "Unrecognised symbol";
    }
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

static MDPL_ERROR_Error* reallocateTokenList(Token** tokens, uint32_t count, uint32_t* capacity)
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

static MDPL_ERROR_Error* reallocateStatmentList(Statment** statments, uint32_t count, uint32_t* capacity)
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

    uint32_t tokenCapacity = 64;
    uint32_t tokenCount = 0;
    Token* tokens = (Token*)malloc(tokenCapacity * sizeof(Token));
    if (tokens == nullptr)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_ALLOCATION, "tokenise: failed to allocate token list");
    }

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
                MDPL_RETERR(reallocateTokenList(&tokens, tokenCount, &tokenCapacity));
                tokens[tokenCount].type = MDPL_COMPILER_TOKEN_TYPE_empty;
                tokens[tokenCount].statmentIndex = statmentCount;
                tokens[tokenCount].data.str = str + nextTokenStartIndex;
                tokens[tokenCount].lineNum = lineCount;
                tokenCount++;
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
            continue;
        }

        // Check for line comment //
        if (i + 1 < length && str[i] == '/' && str[i + 1] == '/')
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
                MDPL_RETERR(reallocateTokenList(&tokens, tokenCount, &tokenCapacity));
                tokens[tokenCount].type = MDPL_COMPILER_TOKEN_TYPE_empty;
                tokens[tokenCount].statmentIndex = statmentCount;
                tokens[tokenCount].data.str = str + nextTokenStartIndex;
                tokens[tokenCount].lineNum = lineCount;
                tokenCount++;
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
                MDPL_RETERR(reallocateTokenList(&tokens, tokenCount, &tokenCapacity));
                tokens[tokenCount].type = MDPL_COMPILER_TOKEN_TYPE_empty;
                tokens[tokenCount].statmentIndex = statmentCount;
                tokens[tokenCount].data.str = str + nextTokenStartIndex;
                tokens[tokenCount].lineNum = lineCount;
                tokenCount++;
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
                MDPL_RETERR(reallocateTokenList(&tokens, tokenCount, &tokenCapacity));
                tokens[tokenCount].type = MDPL_COMPILER_TOKEN_TYPE_empty;
                tokens[tokenCount].statmentIndex = statmentCount;
                tokens[tokenCount].data.str = str + nextTokenStartIndex;
                tokens[tokenCount].lineNum = lineCount;
                tokenCount++;
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
                MDPL_RETERR(reallocateTokenList(&tokens, tokenCount, &tokenCapacity));
                tokens[tokenCount].type = MDPL_COMPILER_TOKEN_TYPE_empty;
                tokens[tokenCount].statmentIndex = statmentCount;
                tokens[tokenCount].data.str = str + nextTokenStartIndex;
                tokens[tokenCount].lineNum = lineCount;
                tokenCount++;
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
        MDPL_RETERR(reallocateTokenList(&tokens, tokenCount, &tokenCapacity));
        tokens[tokenCount].type = MDPL_COMPILER_TOKEN_TYPE_empty;
        tokens[tokenCount].statmentIndex = statmentCount;
        tokens[tokenCount].data.str = str + nextTokenStartIndex;
        tokens[tokenCount].lineNum = lineCount;
        tokenCount++;
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