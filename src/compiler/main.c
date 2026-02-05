#include "../pch.h"
#include "../common/mdpl_common.h"
#include "../runtime_lib/allocator.h"
#include "../runtime_lib/mdpl_error.h"

#include "tokeniser.h"
#include "parser.h"

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
    Scope* scopeList;
    uint32_t scopeListLength;
    error = groupScopes(tokenList, tokenListLength, statmentList, statmentListLength, &scopeList, &scopeListLength);
    MDPL_PRINT_ERROR(error);
    if(error != nullptr)
    {
        free(str);
        free(tokenList);
        free(statmentList);
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

    printf("\n================================ Scopes: ================================\n\n");
    for(uint32_t i = 0; i < scopeListLength; i++)
    {
        Scope scope = scopeList[i];
        printf("scope:%u [statment:%u, statment:%u]\n", i, scope.startStatmentIndex, scope.endStatmentIndex);
    }

    AST ast;
    error = generateAST(tokenList, tokenListLength, statmentList, statmentListLength, true, &ast);

    printf("\n================================ AST: ================================\n\n");
    printf("AST {\n");

    //print public HeaderSegment
    printf("\tpublic: HeaderSegment {\n");
    printf("\t\tnumSignatures: %u\n", ast.public.numSignatures);
    if(ast.public.signatures == nullptr)
    {
        printf("\t\tsignatures: nullptr\n");
    }
    else
    {
        printf("\t\tsignatures: [\n");
        for(uint32_t i = 0; i < ast.public.numSignatures; i++)
        {
            FunctionSignature* sig = &ast.public.signatures[i];
            printf("\t\t\t[%u] FunctionSignature {\n", i);
            if(sig->name == nullptr)
            {
                printf("\t\t\t\tname: nullptr\n");
            }
            else
            {
                printf("\t\t\t\tname: \"%s\"\n", sig->name);
            }
            printf("\t\t\t\tnumInputs: %u\n", sig->numInputs);
            if(sig->inputs == nullptr)
            {
                printf("\t\t\t\tinputs: nullptr\n");
            }
            else
            {
                printf("\t\t\t\tinputs: [\n");
                for(uint32_t j = 0; j < sig->numInputs; j++)
                {
                    Parameter* param = &sig->inputs[j];
                    printf("\t\t\t\t\t[%u] Parameter {\n", j);
                    printf("\t\t\t\t\t\ttype: Type {\n");
                    if(param->type.baseType == nullptr)
                    {
                        printf("\t\t\t\t\t\t\tbaseType: nullptr\n");
                    }
                    else
                    {
                        printf("\t\t\t\t\t\t\tbaseType: \"%s\"\n", param->type.baseType);
                    }
                    printf("\t\t\t\t\t\t\tpointerDepth: %u\n", param->type.pointerDepth);
                    printf("\t\t\t\t\t\t}\n");
                    printf("\t\t\t\t\t\tqualifiers: %u\n", param->qualifiers);
                    if(param->name == nullptr)
                    {
                        printf("\t\t\t\t\t\tname: nullptr\n");
                    }
                    else
                    {
                        printf("\t\t\t\t\t\tname: \"%s\"\n", param->name);
                    }
                    printf("\t\t\t\t\t}\n");
                }
                printf("\t\t\t\t]\n");
            }
            printf("\t\t\t\tnumOutputs: %u\n", sig->numOutputs);
            if(sig->ouputs == nullptr)
            {
                printf("\t\t\t\touputs: nullptr\n");
            }
            else
            {
                printf("\t\t\t\touputs: [\n");
                for(uint32_t j = 0; j < sig->numOutputs; j++)
                {
                    Parameter* param = &sig->ouputs[j];
                    printf("\t\t\t\t\t[%u] Parameter {\n", j);
                    printf("\t\t\t\t\t\ttype: Type {\n");
                    if(param->type.baseType == nullptr)
                    {
                        printf("\t\t\t\t\t\t\tbaseType: nullptr\n");
                    }
                    else
                    {
                        printf("\t\t\t\t\t\t\tbaseType: \"%s\"\n", param->type.baseType);
                    }
                    printf("\t\t\t\t\t\t\tpointerDepth: %u\n", param->type.pointerDepth);
                    printf("\t\t\t\t\t\t}\n");
                    printf("\t\t\t\t\t\tqualifiers: %u\n", param->qualifiers);
                    if(param->name == nullptr)
                    {
                        printf("\t\t\t\t\t\tname: nullptr\n");
                    }
                    else
                    {
                        printf("\t\t\t\t\t\tname: \"%s\"\n", param->name);
                    }
                    printf("\t\t\t\t\t}\n");
                }
                printf("\t\t\t\t]\n");
            }
            printf("\t\t\t}\n");
        }
        printf("\t\t]\n");
    }
    printf("\t}\n");

    //print protected HeaderSegment
    printf("\tprotected: HeaderSegment {\n");
    printf("\t\tnumSignatures: %u\n", ast.protected.numSignatures);
    if(ast.protected.signatures == nullptr)
    {
        printf("\t\tsignatures: nullptr\n");
    }
    else
    {
        printf("\t\tsignatures: [\n");
        for(uint32_t i = 0; i < ast.protected.numSignatures; i++)
        {
            FunctionSignature* sig = &ast.protected.signatures[i];
            printf("\t\t\t[%u] FunctionSignature {\n", i);
            if(sig->name == nullptr)
            {
                printf("\t\t\t\tname: nullptr\n");
            }
            else
            {
                printf("\t\t\t\tname: \"%s\"\n", sig->name);
            }
            printf("\t\t\t\tnumInputs: %u\n", sig->numInputs);
            if(sig->inputs == nullptr)
            {
                printf("\t\t\t\tinputs: nullptr\n");
            }
            else
            {
                printf("\t\t\t\tinputs: [\n");
                for(uint32_t j = 0; j < sig->numInputs; j++)
                {
                    Parameter* param = &sig->inputs[j];
                    printf("\t\t\t\t\t[%u] Parameter {\n", j);
                    printf("\t\t\t\t\t\ttype: Type {\n");
                    if(param->type.baseType == nullptr)
                    {
                        printf("\t\t\t\t\t\t\tbaseType: nullptr\n");
                    }
                    else
                    {
                        printf("\t\t\t\t\t\t\tbaseType: \"%s\"\n", param->type.baseType);
                    }
                    printf("\t\t\t\t\t\t\tpointerDepth: %u\n", param->type.pointerDepth);
                    printf("\t\t\t\t\t\t}\n");
                    printf("\t\t\t\t\t\tqualifiers: %u\n", param->qualifiers);
                    if(param->name == nullptr)
                    {
                        printf("\t\t\t\t\t\tname: nullptr\n");
                    }
                    else
                    {
                        printf("\t\t\t\t\t\tname: \"%s\"\n", param->name);
                    }
                    printf("\t\t\t\t\t}\n");
                }
                printf("\t\t\t\t]\n");
            }
            printf("\t\t\t\tnumOutputs: %u\n", sig->numOutputs);
            if(sig->ouputs == nullptr)
            {
                printf("\t\t\t\touputs: nullptr\n");
            }
            else
            {
                printf("\t\t\t\touputs: [\n");
                for(uint32_t j = 0; j < sig->numOutputs; j++)
                {
                    Parameter* param = &sig->ouputs[j];
                    printf("\t\t\t\t\t[%u] Parameter {\n", j);
                    printf("\t\t\t\t\t\ttype: Type {\n");
                    if(param->type.baseType == nullptr)
                    {
                        printf("\t\t\t\t\t\t\tbaseType: nullptr\n");
                    }
                    else
                    {
                        printf("\t\t\t\t\t\t\tbaseType: \"%s\"\n", param->type.baseType);
                    }
                    printf("\t\t\t\t\t\t\tpointerDepth: %u\n", param->type.pointerDepth);
                    printf("\t\t\t\t\t\t}\n");
                    printf("\t\t\t\t\t\tqualifiers: %u\n", param->qualifiers);
                    if(param->name == nullptr)
                    {
                        printf("\t\t\t\t\t\tname: nullptr\n");
                    }
                    else
                    {
                        printf("\t\t\t\t\t\tname: \"%s\"\n", param->name);
                    }
                    printf("\t\t\t\t\t}\n");
                }
                printf("\t\t\t\t]\n");
            }
            printf("\t\t\t}\n");
        }
        printf("\t\t]\n");
    }
    printf("\t}\n");

    //print code CodeSegment
    printf("\tcode: CodeSegment {\n");
    printf("\t\tnumFunctions: %u\n", ast.code.numFunctions);
    if(ast.code.functions == nullptr)
    {
        printf("\t\tfunctions: nullptr\n");
    }
    else
    {
        printf("\t\tfunctions: [\n");
        for(uint32_t i = 0; i < ast.code.numFunctions; i++)
        {
            FunctionImplementation* func = &ast.code.functions[i];
            printf("\t\t\t[%u] FunctionImplementation {\n", i);
            printf("\t\t\t\tsignature: FunctionSignature {\n");
            if(func->signature.name == nullptr)
            {
                printf("\t\t\t\t\tname: nullptr\n");
            }
            else
            {
                printf("\t\t\t\t\tname: \"%s\"\n", func->signature.name);
            }
            printf("\t\t\t\t\tnumInputs: %u\n", func->signature.numInputs);
            if(func->signature.inputs == nullptr)
            {
                printf("\t\t\t\t\tinputs: nullptr\n");
            }
            else
            {
                printf("\t\t\t\t\tinputs: [\n");
                for(uint32_t j = 0; j < func->signature.numInputs; j++)
                {
                    Parameter* param = &func->signature.inputs[j];
                    printf("\t\t\t\t\t\t[%u] Parameter {\n", j);
                    printf("\t\t\t\t\t\t\ttype: Type {\n");
                    if(param->type.baseType == nullptr)
                    {
                        printf("\t\t\t\t\t\t\t\tbaseType: nullptr\n");
                    }
                    else
                    {
                        printf("\t\t\t\t\t\t\t\tbaseType: \"%s\"\n", param->type.baseType);
                    }
                    printf("\t\t\t\t\t\t\t\tpointerDepth: %u\n", param->type.pointerDepth);
                    printf("\t\t\t\t\t\t\t}\n");
                    printf("\t\t\t\t\t\t\tqualifiers: %u\n", param->qualifiers);
                    if(param->name == nullptr)
                    {
                        printf("\t\t\t\t\t\t\tname: nullptr\n");
                    }
                    else
                    {
                        printf("\t\t\t\t\t\t\tname: \"%s\"\n", param->name);
                    }
                    printf("\t\t\t\t\t\t}\n");
                }
                printf("\t\t\t\t\t]\n");
            }
            printf("\t\t\t\t\tnumOutputs: %u\n", func->signature.numOutputs);
            if(func->signature.ouputs == nullptr)
            {
                printf("\t\t\t\t\touputs: nullptr\n");
            }
            else
            {
                printf("\t\t\t\t\touputs: [\n");
                for(uint32_t j = 0; j < func->signature.numOutputs; j++)
                {
                    Parameter* param = &func->signature.ouputs[j];
                    printf("\t\t\t\t\t\t[%u] Parameter {\n", j);
                    printf("\t\t\t\t\t\t\ttype: Type {\n");
                    if(param->type.baseType == nullptr)
                    {
                        printf("\t\t\t\t\t\t\t\tbaseType: nullptr\n");
                    }
                    else
                    {
                        printf("\t\t\t\t\t\t\t\tbaseType: \"%s\"\n", param->type.baseType);
                    }
                    printf("\t\t\t\t\t\t\t\tpointerDepth: %u\n", param->type.pointerDepth);
                    printf("\t\t\t\t\t\t\t}\n");
                    printf("\t\t\t\t\t\t\tqualifiers: %u\n", param->qualifiers);
                    if(param->name == nullptr)
                    {
                        printf("\t\t\t\t\t\t\tname: nullptr\n");
                    }
                    else
                    {
                        printf("\t\t\t\t\t\t\tname: \"%s\"\n", param->name);
                    }
                    printf("\t\t\t\t\t\t}\n");
                }
                printf("\t\t\t\t\t]\n");
            }
            printf("\t\t\t\t}\n");
            printf("\t\t\t\tnumBlocks: %u\n", func->numBlocks);
            if(func->blocks == nullptr)
            {
                printf("\t\t\t\tblocks: nullptr\n");
            }
            else
            {
                printf("\t\t\t\tblocks: [\n");
                for(uint32_t j = 0; j < func->numBlocks; j++)
                {
                    Block* block = &func->blocks[j];
                    printf("\t\t\t\t\t[%u] Block {\n", j);
                    printf("\t\t\t\t\t\ttype: %u\n", block->type);
                    printf("\t\t\t\t\t}\n");
                }
                printf("\t\t\t\t]\n");
            }
            printf("\t\t\t}\n");
        }
        printf("\t\t]\n");
    }
    printf("\t}\n");

    printf("}\n");

    freeAST(&ast);

    free(str);
    free(tokenList);
    free(statmentList);
    free(scopeList);

    return 0;
}