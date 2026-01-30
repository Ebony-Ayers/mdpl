#include "../pch.h"
#include "../common/mdpl_common.h"
#include "../runtime_lib/allocator.h"
#include "../runtime_lib/mdpl_error.h"

#include "tokeniser.h"

#define FILE_NAME "tokeniser.c"

static void printToken(Token token)
{
    if(token.type == MDPL_COMPILER_TOKEN_TYPE_empty)
    {
        printf("%s ", token.data.str);
    }
    else if(token.type == MDPL_COMPILER_TOKEN_TYPE_symbol)
    {
        printf("%s ", symbolToString(token.data.symbol));
    }
}

int main(int argc, char* argv[])
{
    MDPL_ERROR_Error* error;

    if(argc == 1)
    {
        printf("Error: did not provide a file.\n");
        return 1;
    }
    
    char* str;
    uint32_t length;
    error = readFile(argv[1], &str, &length);
    MDPL_PRINT_ERROR(error);

    Token* tokenList;
    uint32_t tokenListLength;
    Statment* statmentList;
    uint32_t statmentListLength;
    error = tokenise(str, length, &tokenList, &tokenListLength, &statmentList, &statmentListLength);
    MDPL_PRINT_ERROR(error);
    if(error != nullptr)
    {
        free(str);
        return error->exitCode;
    }

    printf("\n================================ Tokens: ================================\n\n");
    uint32_t previousLineNum = 0;
    for(uint32_t i = 0; i < tokenListLength; i++)
    {
        Token token = tokenList[i];
        if(token.lineNum != previousLineNum)
        {
            printf("\n");
            previousLineNum = token.lineNum;
        }
        printToken(token);
    }
    printf("\n");

    printf("\n================================ Statments: ================================\n\n");
    for(uint32_t i = 0; i < statmentListLength; i++)
    {
        Statment statment = statmentList[i];
        printf("%u [%u, %u]: ", i, statment.startTokenIndex, statment.endTokenIndex);
        for(uint32_t j = statment.startTokenIndex; j < statment.endTokenIndex; j++)
        {
            printToken(tokenList[j]);
        }
        printf("\n");
    }

    free(str);
    free(tokenList);
    free(statmentList);

    return 0;
}