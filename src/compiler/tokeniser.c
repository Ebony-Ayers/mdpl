#include "tokeniser.h"
#include "../common/system_headers.h"

uint32_t stringToSinglePunctuation(const char* s)
{
    return (uint32_t)(*(uint8_t*)s);
}
uint32_t stringToDoublePunctuation(const char* s)
{
    return (uint32_t)(*(uint16_t*)s);
}

bool isValidSinglePunctuation(uint32_t const punctuation)
{
    switch (punctuation)
    {
        // Single character punctuations
        case MDPL_COMPILER_PUNCTUATION_exclamation:              /*!*/
        case MDPL_COMPILER_PUNCTUATION_hash:                     /*#*/
        case MDPL_COMPILER_PUNCTUATION_dollar:                   /*$*/
        case MDPL_COMPILER_PUNCTUATION_mod:                      /*%*/
        case MDPL_COMPILER_PUNCTUATION_and:                      /*&*/
        case MDPL_COMPILER_PUNCTUATION_openRoundBracket:         /*(*/
        case MDPL_COMPILER_PUNCTUATION_closeRoundBracket:        /*)*/
        case MDPL_COMPILER_PUNCTUATION_star:                     /***/
        case MDPL_COMPILER_PUNCTUATION_plus:                     /*+*/
        case MDPL_COMPILER_PUNCTUATION_comma:                    /*,*/
        case MDPL_COMPILER_PUNCTUATION_minus:                    /*-*/
        case MDPL_COMPILER_PUNCTUATION_dot:                      /*.*/
        case MDPL_COMPILER_PUNCTUATION_frontSlash:               /*/*/
        case MDPL_COMPILER_PUNCTUATION_colons:                   /*:*/
        case MDPL_COMPILER_PUNCTUATION_lessThan:                 /*<*/
        case MDPL_COMPILER_PUNCTUATION_equals:                   /*=*/
        case MDPL_COMPILER_PUNCTUATION_greaterThan:              /*>*/
        case MDPL_COMPILER_PUNCTUATION_question:                 /*?*/
        case MDPL_COMPILER_PUNCTUATION_at:                       /*@*/
        case MDPL_COMPILER_PUNCTUATION_openSquareBracket:        /*[*/
        case MDPL_COMPILER_PUNCTUATION_closeSquareBracket:       /*]*/
        case MDPL_COMPILER_PUNCTUATION_xor:                      /*^*/
        case MDPL_COMPILER_PUNCTUATION_grave:                    /*`*/
        case MDPL_COMPILER_PUNCTUATION_openCurlyBrace:           /*{*/
        case MDPL_COMPILER_PUNCTUATION_or:                       /*|*/
        case MDPL_COMPILER_PUNCTUATION_closeCurlyBrace:          /*}*/
        case MDPL_COMPILER_PUNCTUATION_not:                      /*~*/
            return true;
        default:
            return false;
    }
}
bool isValidDoublePunctuation(uint32_t const punctuation)
{
    switch (punctuation)
    {
        // Double character punctuations
        case MDPL_COMPILER_PUNCTUATION_doubleAnd:                /*&&*/
        case MDPL_COMPILER_PUNCTUATION_exponent:                 /****/
        case MDPL_COMPILER_PUNCTUATION_increment:                /*++*/
        case MDPL_COMPILER_PUNCTUATION_decrement:                /*--*/
        case MDPL_COMPILER_PUNCTUATION_doubleDot:                /*..*/
        case MDPL_COMPILER_PUNCTUATION_doubleColons:             /*::*/
        case MDPL_COMPILER_PUNCTUATION_shiftLeft:                /*<<*/
        case MDPL_COMPILER_PUNCTUATION_doesNotEqual:             /*!=*/
        case MDPL_COMPILER_PUNCTUATION_modEqual:                 /*%=*/
        case MDPL_COMPILER_PUNCTUATION_andEqual:                 /*&=*/
        case MDPL_COMPILER_PUNCTUATION_starEqual:                /**=*/
        case MDPL_COMPILER_PUNCTUATION_plusEqual:                /*+=*/
        case MDPL_COMPILER_PUNCTUATION_minusEqual:               /*-=*/
        case MDPL_COMPILER_PUNCTUATION_lessEqual:                /*<=*/
        case MDPL_COMPILER_PUNCTUATION_doubleEqual:              /*==*/
        case MDPL_COMPILER_PUNCTUATION_greaterEqual:             /*>=*/
        case MDPL_COMPILER_PUNCTUATION_notEqual:                 /*~=*/
        case MDPL_COMPILER_PUNCTUATION_arrow:                    /*->*/
        case MDPL_COMPILER_PUNCTUATION_shiftRight:               /*>>*/
        case MDPL_COMPILER_PUNCTUATION_doubleOpenSquareBracket:  /*[[*/
        case MDPL_COMPILER_PUNCTUATION_doubleCloseSquareBracket: /*]]*/
        case MDPL_COMPILER_PUNCTUATION_doubleXor:                /*^^*/
        case MDPL_COMPILER_PUNCTUATION_doubleOpenCurlyBrace:     /*{{*/
        case MDPL_COMPILER_PUNCTUATION_doubleOR:                 /*||*/
        case MDPL_COMPILER_PUNCTUATION_doubleCloseCurlyBrace:    /*}}*/
            return true;
        default:
            return false;
    }
}
const char* punctuationToString(uint32_t punctuation)
{
    switch (punctuation)
    {
        // Single character punctuations
        case MDPL_COMPILER_PUNCTUATION_exclamation:              return "!";
        case MDPL_COMPILER_PUNCTUATION_hash:                     return "#";
        case MDPL_COMPILER_PUNCTUATION_dollar:                   return "$";
        case MDPL_COMPILER_PUNCTUATION_mod:                      return "%";
        case MDPL_COMPILER_PUNCTUATION_and:                      return "&";
        case MDPL_COMPILER_PUNCTUATION_openRoundBracket:         return "(";
        case MDPL_COMPILER_PUNCTUATION_closeRoundBracket:        return ")";
        case MDPL_COMPILER_PUNCTUATION_star:                     return "*";
        case MDPL_COMPILER_PUNCTUATION_plus:                     return "+";
        case MDPL_COMPILER_PUNCTUATION_comma:                    return ",";
        case MDPL_COMPILER_PUNCTUATION_minus:                    return "-";
        case MDPL_COMPILER_PUNCTUATION_dot:                      return ".";
        case MDPL_COMPILER_PUNCTUATION_frontSlash:               return "/";
        case MDPL_COMPILER_PUNCTUATION_colons:                   return ":";
        case MDPL_COMPILER_PUNCTUATION_lessThan:                 return "<";
        case MDPL_COMPILER_PUNCTUATION_equals:                   return "=";
        case MDPL_COMPILER_PUNCTUATION_greaterThan:              return ">";
        case MDPL_COMPILER_PUNCTUATION_question:                 return "?";
        case MDPL_COMPILER_PUNCTUATION_at:                       return "@";
        case MDPL_COMPILER_PUNCTUATION_openSquareBracket:        return "[";
        case MDPL_COMPILER_PUNCTUATION_closeSquareBracket:       return "]";
        case MDPL_COMPILER_PUNCTUATION_xor:                      return "^";
        case MDPL_COMPILER_PUNCTUATION_grave:                    return "`";
        case MDPL_COMPILER_PUNCTUATION_openCurlyBrace:           return "{";
        case MDPL_COMPILER_PUNCTUATION_or:                       return "|";
        case MDPL_COMPILER_PUNCTUATION_closeCurlyBrace:          return "}";
        case MDPL_COMPILER_PUNCTUATION_not:                      return "~";
        // Double character punctuations
        case MDPL_COMPILER_PUNCTUATION_doubleAnd:                return "&&";
        case MDPL_COMPILER_PUNCTUATION_exponent:                 return "**";
        case MDPL_COMPILER_PUNCTUATION_increment:                return "++";
        case MDPL_COMPILER_PUNCTUATION_decrement:                return "--";
        case MDPL_COMPILER_PUNCTUATION_doubleDot:                return "..";
        case MDPL_COMPILER_PUNCTUATION_doubleColons:             return "::";
        case MDPL_COMPILER_PUNCTUATION_shiftLeft:                return "<<";
        case MDPL_COMPILER_PUNCTUATION_doesNotEqual:             return "!=";
        case MDPL_COMPILER_PUNCTUATION_modEqual:                 return "%=";
        case MDPL_COMPILER_PUNCTUATION_andEqual:                 return "&=";
        case MDPL_COMPILER_PUNCTUATION_starEqual:                return "*=";
        case MDPL_COMPILER_PUNCTUATION_plusEqual:                return "+=";
        case MDPL_COMPILER_PUNCTUATION_minusEqual:               return "-=";
        case MDPL_COMPILER_PUNCTUATION_lessEqual:                return "<=";
        case MDPL_COMPILER_PUNCTUATION_doubleEqual:              return "==";
        case MDPL_COMPILER_PUNCTUATION_greaterEqual:             return ">=";
        case MDPL_COMPILER_PUNCTUATION_notEqual:                 return "~=";
        case MDPL_COMPILER_PUNCTUATION_arrow:                    return "->";
        case MDPL_COMPILER_PUNCTUATION_shiftRight:               return ">>";
        case MDPL_COMPILER_PUNCTUATION_doubleOpenSquareBracket:  return "[[";
        case MDPL_COMPILER_PUNCTUATION_doubleCloseSquareBracket: return "]]";
        case MDPL_COMPILER_PUNCTUATION_doubleXor:                return "^^";
        case MDPL_COMPILER_PUNCTUATION_doubleOpenCurlyBrace:     return "{{";
        case MDPL_COMPILER_PUNCTUATION_doubleOR:                 return "||";
        case MDPL_COMPILER_PUNCTUATION_doubleCloseCurlyBrace:    return "}}";
        default:                                            return "Unrecognised_punctuation";
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
//increase the size of an array list of type Statment
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
//increase the size of an array list of type Scope
MDPL_ERROR_Error* reallocateScopeList(Scope** scopes, uint32_t count, uint32_t* capacity)
{
    if (count >= *capacity)
    {
        *capacity *= 2;
        Scope* newScopes = (Scope*)realloc(*scopes, *capacity * sizeof(Scope));
        if (newScopes == nullptr)
        {
            free(*scopes);
            *scopes = nullptr;
            return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_ALLOCATION, "groupScopes: failed to reallocate scope list");
        }
        *scopes = newScopes;
    }
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

MDPL_ERROR_Error* tokenise(char* str, const uint32_t length, SymbolTable* symbolTable, Token** tokenList, uint32_t* tokenListLength, Statment** statmentList, uint32_t* statmentListLength, const char* const fName)
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
    uint32_t numTokens = 0;
    Token* tokens = (Token*)malloc(tokenCapacity * sizeof(Token));
    if (tokens == nullptr)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_ALLOCATION, "tokenise: failed to allocate token list");
    }

    //allocate statments list
    uint32_t statmentCapacity = 64;
    uint32_t numStatments = 0;
    Statment* statments = (Statment*)malloc(statmentCapacity * sizeof(Statment));
    if (statments == nullptr)
    {
        free(tokens);
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_ALLOCATION, "tokenise: failed to allocate statment list");
    }

    //the tokeniser is an FSM with an initial state, the outer loop, and all other states come from and return to it in a star topology
    //each other state is it's own loop withing the outer loop which lex an entire token
    //it is safe to iterate up to i+1 == length as the string is null terminated
    uint32_t lineNum = 1;
    bool lastTokenWasComma = false;
    uint32_t statmentStartTokenIndex = 0;
    for (uint32_t i = 0; i < length; i++)
    {
        char c = str[i];
        char cn = str[i+1];
        uint32_t singlePunctuation = stringToSinglePunctuation(str + i);
        uint32_t doublePunctuation = stringToDoublePunctuation(str + i);
        //state: line comment
        if((c == '/') && (cn == '/'))
        {
            i += 2;
            for(; i < length; i++)
            {
                c = str[i];
                if(c == '\n')
                {
                    str[i] = '\0';
                    lineNum++;
                    break;
                }
                else
                {
                    str[i] = '\0';
                }
            }
        }
        //state: block comment
        else if((c == '/') && (cn == '*'))
        {
            i += 2;
            for(; i < length; i++)
            {
                c = str[i];
                cn = str[i+1];
                if(c == '\n')
                {
                    lineNum++;
                    str[i] = '\0';
                }
                else if((c == '*') && (cn == '/'))
                {
                    str[i] = '\0';
                    str[i+1] = '\0';
                    break;
                }
                else
                {
                    str[i] = '\0';
                }
            }
            if(!((c == '*') && (cn == '/')))
            {
                printf("Syntax error: block comment is not termianted.\nLine: %u. File: %s\n\n", lineNum, fName);
            }
            i++;
        }
        //state: string literal
        else if(c == '\"')
        {
            str[i] = '\0';
            uint32_t startOfStringLiteral = i + 1;
            for(; i < length; i++)
            {
                c = str[i];
                if(c == '\n')
                {
                    lineNum++;
                    //preserve new lines in string literals
                }
                else if(c == '\\')
                {
                    i++;
                }
                else if(c == '\"')
                {
                    str[i] = '\0';
                    break;
                }
            }
            if(c != '\"')
            {
                printf("Syntax error: string literal is not termianted.\nLine: %u. File: %s\n\n", lineNum, fName);
            }
            MDPL_RETERR(reallocateTokenList(&tokens, numTokens, &tokenCapacity));
            tokens[numTokens].type = MDPL_COMPILER_TOKEN_TYPE_string;
            tokens[numTokens].statmentIndex = numStatments;
            tokens[numTokens].data.str = str + startOfStringLiteral;
            tokens[numTokens].lineNum = lineNum;
            numTokens++;
            lastTokenWasComma = false;
        }
        //state: character literal
        else if(c == '\'')
        {
            str[i] = '\0';
            uint32_t startOfCharacterLiteral = i + 1;
            for(; i < length; i++)
            {
                c = str[i];
                if(c == '\n')
                {
                    lineNum++;
                    printf("Syntax error: character literal contains new line.\nLine: %u. File: %s\n\n", lineNum, fName);
                }
                else if(c == '\\')
                {
                    i++;
                }
                else if(c == '\'')
                {
                    str[i] = '\0';
                    break;
                }

            }
            if(c != '\'')
            {
                printf("Syntax error: character literal is not termianted.\nLine: %u. File: %s\n\n", lineNum, fName);
            }
            MDPL_RETERR(reallocateTokenList(&tokens, numTokens, &tokenCapacity));
            tokens[numTokens].type = MDPL_COMPILER_TOKEN_TYPE_character;
            tokens[numTokens].statmentIndex = numStatments;
            tokens[numTokens].data.str = str + startOfCharacterLiteral;
            tokens[numTokens].lineNum = lineNum;
            numTokens++;
            lastTokenWasComma = false;
        }
        //state: whitespace
        else if((c == ' ') || (c == '\t'))
        {
            str[i] = '\0';
        }
        //state: new line
        else if(c == '\n')
        {
            if(!lastTokenWasComma)
            {
                MDPL_RETERR(reallocateStatmentList(&statments, numStatments, &statmentCapacity));
                statments[numStatments].startTokenIndex = statmentStartTokenIndex;
                statments[numStatments].endTokenIndex = numTokens;
                statments[numStatments].deepestScopeIndex = 0;
                numStatments++;
                statmentStartTokenIndex = numTokens;
            }
            lineNum++;
        }
        //state: semi colon
        else if(c == ';')
        {
            str[i] = '\0';
            MDPL_RETERR(reallocateStatmentList(&statments, numStatments, &statmentCapacity));
            statments[numStatments].startTokenIndex = statmentStartTokenIndex;
            statments[numStatments].endTokenIndex = numTokens;
            statments[numStatments].deepestScopeIndex = 0;
            numStatments++;
            statmentStartTokenIndex = numTokens;
            lastTokenWasComma = false;
        }
        //state: double punctuation
        else if(isValidDoublePunctuation(doublePunctuation))
        {
            str[i] = '\0';
            str[i+1] = '\0';
            MDPL_RETERR(reallocateTokenList(&tokens, numTokens, &tokenCapacity));
            tokens[numTokens].type = MDPL_COMPILER_TOKEN_TYPE_punctuation;
            tokens[numTokens].statmentIndex = numStatments;
            tokens[numTokens].data.punctuation = doublePunctuation;
            tokens[numTokens].lineNum = lineNum;
            numTokens++;
            lastTokenWasComma = false;
            i++;
        }
        else if(isValidSinglePunctuation(singlePunctuation))
        {
            //state: negative number literal
            if((singlePunctuation == MDPL_COMPILER_PUNCTUATION_minus) && ((cn >= '0') && (cn <= '9')))
            {
                uint32_t numDecimalPoint = 0;
                uint32_t startOfNumberLiteral = i;
                for(; i < length; i++)
                {
                    cn = str[i+1];
                    if(cn == '.')
                    {
                        numDecimalPoint++;
                    }
                    else if((cn < '0') || (cn > '9'))
                    {
                        break;
                    }
                }
                if(numDecimalPoint >= 2)
                {
                    printf("Syntax error: numeric literal contains multiple decimal points.\nLine: %u. File: %s\n\n", lineNum, fName);
                }
                else
                {
                    MDPL_RETERR(reallocateTokenList(&tokens, numTokens, &tokenCapacity));
                    tokens[numTokens].type = MDPL_COMPILER_TOKEN_TYPE_number;
                    tokens[numTokens].statmentIndex = numStatments;
                    tokens[numTokens].data.str = str + startOfNumberLiteral;
                    tokens[numTokens].lineNum = lineNum;
                    numTokens++;
                    lastTokenWasComma = false;
                }
            }
            //state: single punctuation
            else
            {
                str[i] = '\0';
                MDPL_RETERR(reallocateTokenList(&tokens, numTokens, &tokenCapacity));
                tokens[numTokens].type = MDPL_COMPILER_TOKEN_TYPE_punctuation;
                tokens[numTokens].statmentIndex = numStatments;
                tokens[numTokens].data.punctuation = singlePunctuation;
                tokens[numTokens].lineNum = lineNum;
                numTokens++;
                lastTokenWasComma = c == ',';
            }
        }
        //state: positive number literal
        else if((c >= '0') && (c <= '9'))
        {
            uint32_t numDecimalPoint = 0;
            uint32_t startOfNumberLiteral = i;
            for(; i < length; i++)
            {
                cn = str[i+1];
                if(cn == '.')
                {
                    numDecimalPoint++;
                }
                else if((cn < '0') || (cn > '9'))
                {
                    break;
                }
            }
            if(numDecimalPoint >= 2)
            {
                printf("Syntax error: numeric literal contains multiple decimal points.\nLine: %u. File: %s\n\n", lineNum, fName);
            }
            else
            {
                MDPL_RETERR(reallocateTokenList(&tokens, numTokens, &tokenCapacity));
                tokens[numTokens].type = MDPL_COMPILER_TOKEN_TYPE_number;
                tokens[numTokens].statmentIndex = numStatments;
                tokens[numTokens].data.str = str + startOfNumberLiteral;
                tokens[numTokens].lineNum = lineNum;
                numTokens++;
                lastTokenWasComma = false;
            }
        }
        //state: symbol
        else
        {
            uint32_t startOfSymbol = i;
            for(; i < length; i++)
            {
                char cn = str[i+1];
                uint32_t spn = stringToSinglePunctuation(str + i + 1);
                if((cn == ' ') || (cn == '\t') || (cn == '\n') || (cn == '/') || (cn == '\"') || (cn == '\''))
                {
                    break;
                }
                else if(isValidSinglePunctuation(spn))
                {
                    break;
                }
            }
            MDPL_RETERR(reallocateTokenList(&tokens, numTokens, &tokenCapacity));
            tokens[numTokens].type = MDPL_COMPILER_TOKEN_TYPE_symbol;
            tokens[numTokens].statmentIndex = numStatments;
            tokens[numTokens].data.str = str + startOfSymbol;
            tokens[numTokens].lineNum = lineNum;
            numTokens++;
            lastTokenWasComma = false;
        }
    }
    //put any remaining tokens into a final statment
    if(statmentStartTokenIndex < numTokens)                                                                                                                                                                                                                                                 
    {                                                                                                                                                                                                                                                                                        
        MDPL_RETERR(reallocateStatmentList(&statments, numStatments, &statmentCapacity));                                                                                                                                                                                                    
        statments[numStatments].startTokenIndex = statmentStartTokenIndex;                                                                                                                                                                                                                   
        statments[numStatments].endTokenIndex = numTokens;                                                                                                                                                                                                                                   
        statments[numStatments].deepestScopeIndex = 0;                                                                                                                                                                                                                                       
        numStatments++;                                                                                                                                                                                                                                                                      
    }

    //convert the symbol, current const char*, to the symbol type
    for(uint32_t i = 0; i < numTokens; i++)
    {
        Token token = tokens[i];
        if(token.type == MDPL_COMPILER_TOKEN_TYPE_symbol)
        {
            const char* const str = token.data.str;
            Symbol symbol;
            MDPL_RETERR(stringToSymbol(symbolTable, str, &symbol));
            tokens[i].data.symbol = symbol;
        }
    }

    *tokenList = tokens;                                                                                                                                                                                                                                                                     
    *tokenListLength = numTokens;                                                                                                                                                                                                                                                            
    *statmentList = statments;                                                                                                                                                                                                                                                               
    *statmentListLength = numStatments;                                                                                                                                                                                                                                                      
    return nullptr;
    /*
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
                MDPL_RETERR(createStandardToken(&tokens, &numTokens, &tokenCapacity, statmentCount, str + nextTokenStartIndex, lineCount));
                lastTokenWasComma = false;
            }

            lineCount++;

            // End statement on newline if previous token wasn't a comma
            if (!lastTokenWasComma && statmentStartTokenIndex < numTokens)
            {
                MDPL_RETERR(reallocateStatmentList(&statments, statmentCount, &statmentCapacity));
                statments[statmentCount].startTokenIndex = statmentStartTokenIndex;
                statments[statmentCount].endTokenIndex = numTokens;
                statments[statmentCount].deepestScopeIndex = 0;
                statmentCount++;
                statmentStartTokenIndex = numTokens;
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
            MDPL_RETERR(reallocateTokenList(&tokens, numTokens, &tokenCapacity));
            tokens[numTokens].type = MDPL_COMPILER_TOKEN_TYPE_number;
            tokens[numTokens].statmentIndex = statmentCount;
            tokens[numTokens].data.str = str + nextTokenStartIndex;
            tokens[numTokens].lineNum = lineCount;
            numTokens++;
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
                MDPL_RETERR(createStandardToken(&tokens, &numTokens, &tokenCapacity, statmentCount, str + nextTokenStartIndex, lineCount));
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
            MDPL_RETERR(reallocateTokenList(&tokens, numTokens, &tokenCapacity));
            tokens[numTokens].type = MDPL_COMPILER_TOKEN_TYPE_string;
            tokens[numTokens].statmentIndex = statmentCount;
            tokens[numTokens].data.str = str + nextTokenStartIndex;
            tokens[numTokens].lineNum = lineCount;
            numTokens++;
            lastTokenWasComma = false;
            nextTokenStartIndex = i + 1;
        }
        //character literals
        else if((i > 0) && (str[i] == '\'') && (str[i-1] != '\\'))
        {
            // Create token for content before the string if any
            if (nextTokenStartIndex < i)
            {
                MDPL_RETERR(createStandardToken(&tokens, &numTokens, &tokenCapacity, statmentCount, str + nextTokenStartIndex, lineCount));
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
            MDPL_RETERR(reallocateTokenList(&tokens, numTokens, &tokenCapacity));
            tokens[numTokens].type = MDPL_COMPILER_TOKEN_TYPE_character;
            tokens[numTokens].statmentIndex = statmentCount;
            tokens[numTokens].data.str = str + nextTokenStartIndex;
            tokens[numTokens].lineNum = lineCount;
            numTokens++;
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
                if (!lastTokenWasComma && statmentStartTokenIndex < numTokens)
                {
                    MDPL_RETERR(reallocateStatmentList(&statments, statmentCount, &statmentCapacity));
                    statments[statmentCount].startTokenIndex = statmentStartTokenIndex;
                    statments[statmentCount].endTokenIndex = numTokens;
                    statments[statmentCount].deepestScopeIndex = 0;
                    statmentCount++;
                    statmentStartTokenIndex = numTokens;
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
                    if (!lastTokenWasComma && statmentStartTokenIndex < numTokens)
                    {
                        MDPL_RETERR(reallocateStatmentList(&statments, statmentCount, &statmentCapacity));
                        statments[statmentCount].startTokenIndex = statmentStartTokenIndex;
                        statments[statmentCount].endTokenIndex = numTokens;
                        statments[statmentCount].deepestScopeIndex = 0;
                        statmentCount++;
                        statmentStartTokenIndex = numTokens;
                    }

                    nextTokenStartIndex = i + 1;
                }
                i++;
            }
            if (i + 1 < length)
            {
                i++; // Skip past the closing
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
                MDPL_RETERR(createStandardToken(&tokens, &numTokens, &tokenCapacity, statmentCount, str + nextTokenStartIndex, lineCount));
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
                MDPL_RETERR(createStandardToken(&tokens, &numTokens, &tokenCapacity, statmentCount, str + nextTokenStartIndex, lineCount));
            }

            // End statement on semicolon
            MDPL_RETERR(reallocateStatmentList(&statments, statmentCount, &statmentCapacity));
            statments[statmentCount].startTokenIndex = statmentStartTokenIndex;
            statments[statmentCount].endTokenIndex = numTokens;
            statments[statmentCount].deepestScopeIndex = 0;
            statmentCount++;
            statmentStartTokenIndex = numTokens;
            lastTokenWasComma = false;

            nextTokenStartIndex = i + 1;
        }
        else if (i + 1 < length && isValidDoublePunctuation(stringToDoublePunctuation(&str[i])))
        {
            uint32_t punctuation = stringToDoublePunctuation(&str[i]);
            str[i] = '\0';
            str[i + 1] = '\0';

            // Create token for text before the punctuation if any
            if (nextTokenStartIndex < i)
            {
                MDPL_RETERR(createStandardToken(&tokens, &numTokens, &tokenCapacity, statmentCount, str + nextTokenStartIndex, lineCount));
            }

            MDPL_RETERR(reallocateTokenList(&tokens, numTokens, &tokenCapacity));
            tokens[numTokens].type = MDPL_COMPILER_TOKEN_TYPE_punctuation;
            tokens[numTokens].statmentIndex = statmentCount;
            tokens[numTokens].data.punctuation = punctuation;
            tokens[numTokens].lineNum = lineCount;
            numTokens++;
            lastTokenWasComma = false;

            i++;
            nextTokenStartIndex = i + 1;
        }
        else if (isValidSinglePunctuation(stringToSinglePunctuation(&str[i])))
        {
            uint32_t punctuation = stringToSinglePunctuation(&str[i]);
            str[i] = '\0';

            // Create token for text before the punctuation if any
            if (nextTokenStartIndex < i)
            {
                MDPL_RETERR(createStandardToken(&tokens, &numTokens, &tokenCapacity, statmentCount, str + nextTokenStartIndex, lineCount));
            }

            MDPL_RETERR(reallocateTokenList(&tokens, numTokens, &tokenCapacity));
            tokens[numTokens].type = MDPL_COMPILER_TOKEN_TYPE_punctuation;
            tokens[numTokens].statmentIndex = statmentCount;
            tokens[numTokens].data.punctuation = punctuation;
            tokens[numTokens].lineNum = lineCount;
            numTokens++;

            lastTokenWasComma = (punctuation == MDPL_COMPILER_PUNCTUATION_comma);

            nextTokenStartIndex = i + 1;
        }
    }

    // Create token for any remaining text after the last delimiter
    if (nextTokenStartIndex < length && str[nextTokenStartIndex] != '\0')
    {
        MDPL_RETERR(createStandardToken(&tokens, &numTokens, &tokenCapacity, statmentCount, str + nextTokenStartIndex, lineCount));
    }

    // Create final statement if there are remaining tokens
    if (statmentStartTokenIndex < numTokens)
    {
        MDPL_RETERR(reallocateStatmentList(&statments, statmentCount, &statmentCapacity));
        statments[statmentCount].startTokenIndex = statmentStartTokenIndex;
        statments[statmentCount].endTokenIndex = numTokens;
        statments[statmentCount].deepestScopeIndex = 0;
        statmentCount++;
    }

    *tokenList = tokens;
    *tokenListLength = numTokens;
    *statmentList = statments;
    *statmentListLength = statmentCount;
    return nullptr;
    */
}
MDPL_ERROR_Error* groupScopes(SymbolTable* symbolTable, Token* tokenList, const uint32_t tokenListLength, Statment* statmentList, const uint32_t statmentListLength, Scope** scopeList, uint32_t* scopeListLength)
{
    // Validate input parameters
    if (tokenList == nullptr && tokenListLength > 0)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "groupScopes: tokenList is null but tokenListLength is non-zero");
    }
    if (statmentList == nullptr && statmentListLength > 0)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "groupScopes: statmentList is null but statmentListLength is non-zero");
    }
    if (scopeList == nullptr)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "groupScopes: scopeList is null");
    }
    if (scopeListLength == nullptr)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "groupScopes: scopeListLength is null");
    }

    // Allocate scope list
    uint32_t scopeCapacity = 64;
    uint32_t scopeCount = 0;
    Scope* scopes = (Scope*)malloc(scopeCapacity * sizeof(Scope));
    if (scopes == nullptr)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_ALLOCATION, "groupScopes: failed to allocate scope list");
    }

    // Stack to track open scope indices (for nested scopes)
    uint32_t stackCapacity = 64;
    uint32_t stackSize = 0;
    uint32_t* scopeStack = (uint32_t*)malloc(stackCapacity * sizeof(uint32_t));
    if (scopeStack == nullptr)
    {
        free(scopes);
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_ALLOCATION, "groupScopes: failed to allocate scope stack");
    }

    // Iterate through tokens looking for scope delimiters
    for (uint32_t i = 0; i < tokenListLength; i++)
    {
        if (tokenList[i].type == MDPL_COMPILER_TOKEN_TYPE_punctuation)
        {
            if (tokenList[i].data.punctuation == MDPL_COMPILER_PUNCTUATION_openCurlyBrace)
            {
                // Reallocate scope list if needed
                MDPL_ERROR_Error* err = reallocateScopeList(&scopes, scopeCount, &scopeCapacity);
                if (err != nullptr)
                {
                    free(scopeStack);
                    return err;
                }

                // Initialize new scope
                scopes[scopeCount].startTokenIndex = i;
                scopes[scopeCount].endTokenIndex = 0; // Will be set when closing brace is found
                scopes[scopeCount].startStatmentIndex = tokenList[i].statmentIndex;
                scopes[scopeCount].endStatmentIndex = 0; // Will be set when closing brace is found

                // Grow stack if needed
                if (stackSize >= stackCapacity)
                {
                    stackCapacity *= 2;
                    uint32_t* newStack = (uint32_t*)realloc(scopeStack, stackCapacity * sizeof(uint32_t));
                    if (newStack == nullptr)
                    {
                        free(scopeStack);
                        free(scopes);
                        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_ALLOCATION, "groupScopes: failed to reallocate scope stack");
                    }
                    scopeStack = newStack;
                }

                // Push scope index onto stack
                scopeStack[stackSize] = scopeCount;
                stackSize++;
                scopeCount++;
            }
            else if (tokenList[i].data.punctuation == MDPL_COMPILER_PUNCTUATION_closeCurlyBrace)
            {
                // Check for unmatched closing brace
                if (stackSize == 0)
                {
                    free(scopeStack);
                    free(scopes);
                    return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "groupScopes: unmatched closing brace");
                }

                // Pop scope from stack and complete it
                stackSize--;
                uint32_t scopeIndex = scopeStack[stackSize];
                scopes[scopeIndex].endTokenIndex = i + 1;
                scopes[scopeIndex].endStatmentIndex = tokenList[i].statmentIndex + 1;

                // Update deepestScopeIndex for statements in this scope
                // Use 1-based indexing: 0 means not in any scope, 1+ is the scope depth
                uint32_t currentDepth = stackSize + 1; // +1 because we already decremented stackSize
                for (uint32_t j = scopes[scopeIndex].startStatmentIndex; j < scopes[scopeIndex].endStatmentIndex && j < statmentListLength; j++)
                {
                    // Only update if this scope is deeper than currently recorded
                    if (currentDepth > statmentList[j].deepestScopeIndex)
                    {
                        statmentList[j].deepestScopeIndex = currentDepth;
                    }
                }
            }
        }
    }

    // Check for unclosed scopes
    if (stackSize > 0)
    {
        free(scopeStack);
        free(scopes);
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "groupScopes: unclosed scope(s)");
    }

    free(scopeStack);
    *scopeList = scopes;
    *scopeListLength = scopeCount;
    return nullptr;
}