#include "../pch.h"
#include "../common/mdpl_common.h"
#include "../runtime_lib/allocator.h"
#include "../runtime_lib/mdpl_error.h"

#include "tokeniser.h"

#define FILE_NAME "tokeniser.c"

static void setTerminalColorRed()     { printf("\x1b[31m"); }
static void setTerminalColorGreen()   { printf("\x1b[32m"); }
static void setTerminalColorYellow()  { printf("\x1b[33m"); }
static void setTerminalColorBlue()    { printf("\x1b[34m"); }
static void setTerminalColorMagenta() { printf("\x1b[35m"); }
static void setTerminalColorCyan()    { printf("\x1b[36m"); }
static void resetTerminalColor()      { printf("\x1b[39m"); }
static void terminalUnderlineOn()     { printf("\x1b[4m");  }
static void terminalUnderlineOff()    { printf("\x1b[24m"); }

static void printToken(Token token)
{
    if(token.type == MDPL_COMPILER_TOKEN_TYPE_undefined)
    {
        printf("Undefined token ");
    }
    else if(token.type == MDPL_COMPILER_TOKEN_TYPE_unknown)
    {
        setTerminalColorRed();
        printf("%s ", token.data.str);
        resetTerminalColor();
    }
    else if(token.type == MDPL_COMPILER_TOKEN_TYPE_symbol)
    {
        setTerminalColorGreen();
        printf("%s ", symbolToString(token.data.symbol));
        resetTerminalColor();
    }
    else if(token.type == MDPL_COMPILER_TOKEN_TYPE_keyword)
    {
        setTerminalColorYellow();
        printf("%s ", keywordToString(token.data.keyword));
        resetTerminalColor();
    }
    else if(token.type == MDPL_COMPILER_TOKEN_TYPE_type)
    {
        setTerminalColorBlue();
        printf("%s ", typeToString(token.data.type));
        resetTerminalColor();
    }
    else if(token.type == MDPL_COMPILER_TOKEN_TYPE_number)
    {
        setTerminalColorMagenta();
        printf("%s ", token.data.str);
        resetTerminalColor();
    }
    else if(token.type == MDPL_COMPILER_TOKEN_TYPE_string)
    {
        setTerminalColorCyan();
        printf("%s ", token.data.str);
        resetTerminalColor();
    }
    else if(token.type == MDPL_COMPILER_TOKEN_TYPE_character)
    {
        setTerminalColorCyan();
        printf("%s ", token.data.str);
        resetTerminalColor();
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

    printf("\n================================ Statments: ================================\n\n");
    for(uint32_t i = 0; i < statmentListLength; i++)
    {
        Statment statment = statmentList[i];
        printf("l:%u s:%u [t:%u, t:%u]: ", tokenList[statment.startTokenIndex].lineNum, i, statment.startTokenIndex, statment.endTokenIndex);
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