#include "parser.h"

MDPL_ERROR_Error* generateAST(Token* tokenList, const uint32_t tokenListLength, Statment* statmentList, const uint32_t statmentListLength, bool isHeader, AST* ast)
{
    if(isHeader)
    {
        MDPL_RETERR(generateASTHeader(tokenList, tokenListLength, statmentList, statmentListLength, ast));
        ast->protected.signatures = nullptr;
        ast->protected.numSignatures = 0;
        ast->code.functions = nullptr;
        ast->code.numFunctions = 0;
    }
    else
    {
        ast->public.signatures = nullptr;
        ast->public.numSignatures = 0;
        ast->protected.signatures = nullptr;
        ast->protected.numSignatures = 0;
        MDPL_RETERR(generateASTCode(tokenList, tokenListLength, statmentList, statmentListLength, ast));
    }
    return nullptr;
}
//check if a statement is a function declaration by looking for pattern: ... name ( ... ) ;
static bool isCFunctionDeclaration(Token* tokenList, Statment* stmt)
{
    uint32_t start = stmt->startTokenIndex;
    uint32_t end = stmt->endTokenIndex;

    //find opening parenthesis
    bool foundOpenParen = false;
    bool foundCloseParen = false;
    uint32_t openParenIndex = 0;

    for(uint32_t i = start; i <= end; i++)
    {
        if(tokenList[i].type == MDPL_COMPILER_TOKEN_TYPE_symbol)
        {
            if(tokenList[i].data.symbol == MDPL_COMPILER_SYMBOL_openRoundBracket && !foundOpenParen)
            {
                foundOpenParen = true;
                openParenIndex = i;
            }
            else if(tokenList[i].data.symbol == MDPL_COMPILER_SYMBOL_closeRoundBracket && foundOpenParen)
            {
                foundCloseParen = true;
            }
        }
    }

    //must have parentheses and an identifier before the open paren
    if(!foundOpenParen || !foundCloseParen)
    {
        return false;
    }

    //check that there's an identifier (function name) before the opening paren
    if(openParenIndex > start && tokenList[openParenIndex - 1].type == MDPL_COMPILER_TOKEN_TYPE_unknown)
    {
        return true;
    }

    return false;
}

//parse a C type (handles const, pointers, etc.)
static MDPL_ERROR_Error* parseCType(Token* tokenList, const uint32_t tokenListLength, uint32_t* index, Type* type, uint32_t* qualifiers)
{
    uint32_t i = *index;
    type->baseType = nullptr;
    type->pointerDepth = 0;
    *qualifiers = MDPL_COMPILER_PARAMETER_QUALIFIERS_undefined;

    //check for const qualifier before type
    while(i < tokenListLength && tokenList[i].type == MDPL_COMPILER_TOKEN_TYPE_keyword)
    {
        if(tokenList[i].data.keyword == MDPL_COMPILER_KEYWORD_const)
        {
            *qualifiers |= MDPL_COMPILER_PARAMETER_QUALIFIERS_const;
            i++;
        }
        else if(tokenList[i].data.keyword == MDPL_COMPILER_KEYWORD_static ||
                tokenList[i].data.keyword == MDPL_COMPILER_KEYWORD_typedef)
        {
            //skip static/typedef for function declarations
            i++;
        }
        else
        {
            break;
        }
    }

    //parse base type
    if(i >= tokenListLength)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "Unexpected end of tokens while parsing type");
    }

    if(tokenList[i].type == MDPL_COMPILER_TOKEN_TYPE_type)
    {
        type->baseType = typeToString(tokenList[i].data.type);
        i++;
    }
    else if(tokenList[i].type == MDPL_COMPILER_TOKEN_TYPE_unknown)
    {
        type->baseType = tokenList[i].data.str;
        i++;
    }
    else if(tokenList[i].type == MDPL_COMPILER_TOKEN_TYPE_keyword &&
            tokenList[i].data.keyword == MDPL_COMPILER_KEYWORD_const)
    {
        //const after type specifier (e.g., in "char const*")
        *qualifiers |= MDPL_COMPILER_PARAMETER_QUALIFIERS_const;
        i++;
    }
    else
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "Expected type name");
    }

    //check for const after type name (e.g., "char const")
    while(i < tokenListLength &&
          tokenList[i].type == MDPL_COMPILER_TOKEN_TYPE_keyword &&
          tokenList[i].data.keyword == MDPL_COMPILER_KEYWORD_const)
    {
        *qualifiers |= MDPL_COMPILER_PARAMETER_QUALIFIERS_const;
        i++;
    }

    //count pointer depth
    while(i < tokenListLength && tokenList[i].type == MDPL_COMPILER_TOKEN_TYPE_symbol)
    {
        if(tokenList[i].data.symbol == MDPL_COMPILER_SYMBOL_star)
        {
            type->pointerDepth++;
            i++;
        }
        else if(tokenList[i].data.symbol == MDPL_COMPILER_SYMBOL_exponent)
        {
            //tokeniser produces ** as exponent symbol instead of two stars
            type->pointerDepth += 2;
            i++;
        }
        else
        {
            break;
        }

        //check for const after pointer (e.g., "char * const")
        while(i < tokenListLength &&
              tokenList[i].type == MDPL_COMPILER_TOKEN_TYPE_keyword &&
              tokenList[i].data.keyword == MDPL_COMPILER_KEYWORD_const)
        {
            *qualifiers |= MDPL_COMPILER_PARAMETER_QUALIFIERS_const;
            i++;
        }
    }

    *index = i;
    return nullptr;
}

//parse a single C parameter
static MDPL_ERROR_Error* parseCParameter(Token* tokenList, const uint32_t tokenListLength, uint32_t* index, Parameter* param)
{
    uint32_t i = *index;

    //parse type and qualifiers
    MDPL_RETERR(parseCType(tokenList, tokenListLength, &i, &param->type, &param->qualifiers));

    //parse parameter name (optional in C declarations)
    if(i < tokenListLength && tokenList[i].type == MDPL_COMPILER_TOKEN_TYPE_unknown)
    {
        param->name = tokenList[i].data.str;
        i++;
    }
    else
    {
        param->name = nullptr;
    }

    *index = i;
    return nullptr;
}

//parse a C function declaration from a statement
static MDPL_ERROR_Error* parseCFunctionSignature(Token* tokenList, const uint32_t tokenListLength, Statment* stmt, FunctionSignature* sig)
{
    uint32_t i = stmt->startTokenIndex;
    uint32_t end = stmt->endTokenIndex;

    //find the opening parenthesis to locate the function name
    uint32_t openParenIndex = 0;
    for(uint32_t j = i; j <= end; j++)
    {
        if(tokenList[j].type == MDPL_COMPILER_TOKEN_TYPE_symbol &&
           tokenList[j].data.symbol == MDPL_COMPILER_SYMBOL_openRoundBracket)
        {
            openParenIndex = j;
            break;
        }
    }

    //the token before open paren should be the function name
    if(openParenIndex == 0 || openParenIndex <= i)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "Could not find function name");
    }

    uint32_t funcNameIndex = openParenIndex - 1;
    if(tokenList[funcNameIndex].type != MDPL_COMPILER_TOKEN_TYPE_unknown)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "Expected function name before '('");
    }
    sig->name = tokenList[funcNameIndex].data.str;

    //parse return type (everything from start to function name)
    //allocate single output for return type
    sig->ouputs = malloc(sizeof(Parameter));
    if(sig->ouputs == nullptr)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_ALLOCATION, "Failed to allocate return type");
    }
    sig->numOutputs = 1;
    sig->ouputs[0].name = nullptr;

    uint32_t typeIndex = i;
    MDPL_RETERR(parseCType(tokenList, funcNameIndex, &typeIndex, &sig->ouputs[0].type, &sig->ouputs[0].qualifiers));

    //check if return type is void with no pointers (means no return value)
    if(sig->ouputs[0].type.baseType != nullptr &&
       strcmp(sig->ouputs[0].type.baseType, "void") == 0 &&
       sig->ouputs[0].type.pointerDepth == 0)
    {
        free(sig->ouputs);
        sig->ouputs = nullptr;
        sig->numOutputs = 0;
    }

    //parse parameters
    i = openParenIndex + 1; //skip '('

    //find closing parenthesis
    uint32_t closeParenIndex = 0;
    uint32_t parenDepth = 1;
    for(uint32_t j = i; j <= end; j++)
    {
        if(tokenList[j].type == MDPL_COMPILER_TOKEN_TYPE_symbol)
        {
            if(tokenList[j].data.symbol == MDPL_COMPILER_SYMBOL_openRoundBracket)
            {
                parenDepth++;
            }
            else if(tokenList[j].data.symbol == MDPL_COMPILER_SYMBOL_closeRoundBracket)
            {
                parenDepth--;
                if(parenDepth == 0)
                {
                    closeParenIndex = j;
                    break;
                }
            }
        }
    }

    //check for empty parameter list or (void)
    bool emptyParams = false;
    if(i >= closeParenIndex)
    {
        emptyParams = true;
    }
    else if(tokenList[i].type == MDPL_COMPILER_TOKEN_TYPE_type &&
            tokenList[i].data.type == MDPL_COMPILER_TYPE_int) //check for void - need to use the actual void type
    {
        //check if it's just "void" with no parameter name
        if(i + 1 == closeParenIndex)
        {
            emptyParams = true;
        }
    }

    //check specifically for void keyword
    if(!emptyParams && i < closeParenIndex)
    {
        //check if single token is a void type
        const char* typeStr = nullptr;
        if(tokenList[i].type == MDPL_COMPILER_TOKEN_TYPE_type)
        {
            typeStr = typeToString(tokenList[i].data.type);
        }
        if(typeStr != nullptr && strcmp(typeStr, "void") == 0 && i + 1 == closeParenIndex)
        {
            emptyParams = true;
        }
    }

    if(emptyParams)
    {
        sig->inputs = nullptr;
        sig->numInputs = 0;
        return nullptr;
    }

    //count parameters by counting commas + 1
    uint32_t paramCount = 1;
    parenDepth = 0;
    for(uint32_t j = i; j < closeParenIndex; j++)
    {
        if(tokenList[j].type == MDPL_COMPILER_TOKEN_TYPE_symbol)
        {
            if(tokenList[j].data.symbol == MDPL_COMPILER_SYMBOL_openRoundBracket)
            {
                parenDepth++;
            }
            else if(tokenList[j].data.symbol == MDPL_COMPILER_SYMBOL_closeRoundBracket)
            {
                parenDepth--;
            }
            else if(tokenList[j].data.symbol == MDPL_COMPILER_SYMBOL_comma && parenDepth == 0)
            {
                paramCount++;
            }
        }
    }

    //allocate parameters
    sig->inputs = malloc(paramCount * sizeof(Parameter));
    if(sig->inputs == nullptr)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_ALLOCATION, "Failed to allocate parameters");
    }
    sig->numInputs = paramCount;

    //initialize parameters
    for(uint32_t p = 0; p < paramCount; p++)
    {
        sig->inputs[p].type.baseType = nullptr;
        sig->inputs[p].type.pointerDepth = 0;
        sig->inputs[p].qualifiers = MDPL_COMPILER_PARAMETER_QUALIFIERS_undefined;
        sig->inputs[p].name = nullptr;
    }

    //parse each parameter
    for(uint32_t p = 0; p < paramCount; p++)
    {
        MDPL_RETERR(parseCParameter(tokenList, closeParenIndex, &i, &sig->inputs[p]));

        //skip comma if not last parameter
        if(p < paramCount - 1)
        {
            if(i < closeParenIndex &&
               tokenList[i].type == MDPL_COMPILER_TOKEN_TYPE_symbol &&
               tokenList[i].data.symbol == MDPL_COMPILER_SYMBOL_comma)
            {
                i++;
            }
        }
    }

    return nullptr;
}

MDPL_ERROR_Error* generateASTHeader(Token* tokenList, const uint32_t tokenListLength, Statment* statmentList, const uint32_t statmentListLength, AST* ast)
{
    //count function declarations
    uint32_t functionCount = 0;
    for(uint32_t i = 0; i < statmentListLength; i++)
    {
        if(isCFunctionDeclaration(tokenList, &statmentList[i]))
        {
            functionCount++;
        }
    }

    //allocate signatures array
    if(functionCount == 0)
    {
        ast->public.signatures = nullptr;
        ast->public.numSignatures = 0;
        return nullptr;
    }

    ast->public.signatures = malloc(functionCount * sizeof(FunctionSignature));
    if(ast->public.signatures == nullptr)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_ALLOCATION, "Failed to allocate function signatures array");
    }
    ast->public.numSignatures = functionCount;

    //initialize all signatures
    for(uint32_t i = 0; i < functionCount; i++)
    {
        ast->public.signatures[i].name = nullptr;
        ast->public.signatures[i].inputs = nullptr;
        ast->public.signatures[i].numInputs = 0;
        ast->public.signatures[i].ouputs = nullptr;
        ast->public.signatures[i].numOutputs = 0;
    }

    //parse each function declaration
    uint32_t sigIndex = 0;
    for(uint32_t i = 0; i < statmentListLength && sigIndex < functionCount; i++)
    {
        if(isCFunctionDeclaration(tokenList, &statmentList[i]))
        {
            MDPL_RETERR(parseCFunctionSignature(tokenList, tokenListLength, &statmentList[i], &ast->public.signatures[sigIndex]));
            sigIndex++;
        }
    }

    return nullptr;
}
MDPL_ERROR_Error* generateASTCode(Token* tokenList, const uint32_t tokenListLength, Statment* statmentList, const uint32_t statmentListLength, AST* ast)
{
    return nullptr;
}

MDPL_ERROR_Error* freeAST(AST* ast)
{
    MDPL_RETERR(freeHeaderSegment(&(ast->public)));
    MDPL_RETERR(freeHeaderSegment(&(ast->protected)));
    MDPL_RETERR(freeCodeSegment(&(ast->code)));
    return nullptr;
}
MDPL_ERROR_Error* freeHeaderSegment(HeaderSegment* headerSegment)
{
    for(uint32_t i = 0; i < headerSegment->numSignatures; i++)
    {
        free(headerSegment->signatures[i].inputs);
        free(headerSegment->signatures[i].ouputs);
    }
    free(headerSegment->signatures);
    return nullptr;
}
MDPL_ERROR_Error* freeCodeSegment(CodeSegment* codeSegment)
{
    for(uint32_t i = 0; i < codeSegment->numFunctions; i++)
    {
        for(uint32_t j = 0; j < codeSegment->functions[i].numBlocks; j++)
        {
            Block* block = &(codeSegment->functions[i].blocks[j]);
            if(block->type == MDPL_COMPILER_BLOCK_TYPE_code)
            {
                for(uint32_t k = 0; k < block->contents.expressions->numElements; k++)
                {
                    free(block->contents.expressions[k].elements);
                }
                free(block->contents.expressions);
            }
            else if(block->type == MDPL_COMPILER_BLOCK_TYPE_switch)
            {
                free(block->contents.cases);
            }
        }
        free(codeSegment->functions[i].blocks);
    }
    free(codeSegment->functions);
    return nullptr;
}