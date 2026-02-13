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

MDPL_ERROR_Error* generateASTHeader(Token* tokenList, const uint32_t tokenListLength, Statment* statmentList, const uint32_t statmentListLength, AST* ast)
{
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
    free(headerSegment->compileTimeGlobals);
    free(headerSegment->runtimeGlobals);
    for(uint32_t i = 0; i < headerSegment->numStructs; i++)
    {
        free(headerSegment->structs[i].types);
        free(headerSegment->structs[i].names);
    }
    free(headerSegment->structs);
    return nullptr;
}
MDPL_ERROR_Error* freeCodeSegment(CodeSegment* codeSegment)
{
    for(uint32_t i = 0; i < codeSegment->numFunctions; i++)
    {
        FunctionImplementation function = codeSegment->functions[i];
        for(uint32_t j = 0; j < function.numExpressions; j++)
        {
            Expression expression = function.expressions[j];
            free(expression.elements);
        }
        free(function.expressions);
    }
    free(codeSegment->functions);
    return nullptr;
}