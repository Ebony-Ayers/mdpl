#ifndef PARSER_HEADER_GUARD
#define PARSER_HEADER_GUARD

#include "../pch.h"
#include "../common/mdpl_common.h"
#include "../runtime_lib/mdpl_error.h"

#include "tokeniser.h"

typedef struct Expression Expression;

typedef struct FunctionCall
{
    const char* name;
    Expression* parameters;
    uint32_t numParameters;
} FunctionCall;

#define MDPL_COMPILER_RPN_ELEMENT_TYPE_undefined  0
#define MDPL_COMPILER_RPN_ELEMENT_TYPE_operator   1
#define MDPL_COMPILER_RPN_ELEMENT_TYPE_literal    2
#define MDPL_COMPILER_RPN_ELEMENT_TYPE_variable   3
#define MDPL_COMPILER_RPN_ELEMENT_TYPE_function   4

//ordered by precedence
#define MDPL_COMPILER_RNP_ELEMENT_OPERATOR_undefined   0
#define MDPL_COMPILER_RNP_ELEMENT_OPERATOR_assignment  1

typedef struct RPNElement
{
    uint32_t type;
    union
    {
        uint32_t operator;
        const char* literalStr;
        const char* variable;

    } contents;
} RPNElement;

//a collection of unary operators, binary operators and functions calls that can be evaluiated in isolation
typedef struct Expression
{
    RPNElement* elements;
    uint32_t numElements;
} Expression;

//individual cases within a switch statment
typedef struct Case
{
    uint64_t key;
    bool keySign;
    Expression expression;
} Case;

#define MDPL_COMPILER_BLOCK_TYPE_undefined  0
#define MDPL_COMPILER_BLOCK_TYPE_code       1
#define MDPL_COMPILER_BLOCK_TYPE_if         2
#define MDPL_COMPILER_BLOCK_TYPE_for        3
#define MDPL_COMPILER_BLOCK_TYPE_while      4
#define MDPL_COMPILER_BLOCK_TYPE_switch     5

//a block is a section of the code inside a scope
//it can be either control flow of a series of expressions
typedef struct Block
{
    uint32_t type;
    union
    {
        struct
        {
            Expression* expressions;
            uint32_t numExpressions;
        };
        struct Block* block;
        struct
        {
            Case* cases;
            uint32_t numCases;
        };
    } contents;
} Block;

typedef struct Type
{
    const char* baseType;
    uint32_t pointerDepth;
    //TODO:
    //generic template
    //generic specialisation
} Type;

#define MDPL_COMPILER_PARAMETER_QUALIFIERS_undefined  0
#define MDPL_COMPILER_PARAMETER_QUALIFIERS_const      1
#define MDPL_COMPILER_PARAMETER_QUALIFIERS_mut        2
#define MDPL_COMPILER_PARAMETER_QUALIFIERS_ref        4

typedef struct Parameter
{
    Type type;
    uint32_t qualifiers;
    const char* name;
} Parameter;

typedef struct FunctionSignature
{
    const char* name;
    Parameter* inputs;
    uint32_t numInputs;
    Parameter* ouputs;
    uint32_t numOutputs;
} FunctionSignature;

typedef struct FunctionImplementation
{
    FunctionSignature signature;
    Block* blocks;
    uint32_t numBlocks;
} FunctionImplementation;

//equivilent of .h file
typedef struct HeaderSegment
{
    FunctionSignature* signatures;
    uint32_t numSignatures;
} HeaderSegment;

//equivilent of .c file
typedef struct CodeSegment
{
    FunctionImplementation* functions;
    uint32_t numFunctions;
} CodeSegment;

//root of the tree
typedef struct AST {
    HeaderSegment public;
    HeaderSegment protected;
    CodeSegment   code;
} AST;

MDPL_ERROR_Error* generateAST(Token* tokenList, const uint32_t tokenListLength, Statment* statmentList, const uint32_t statmentListLength, bool isHeader, AST* ast);
MDPL_ERROR_Error* generateASTHeader(Token* tokenList, const uint32_t tokenListLength, Statment* statmentList, const uint32_t statmentListLength, AST* ast);
MDPL_ERROR_Error* generateASTCode(Token* tokenList, const uint32_t tokenListLength, Statment* statmentList, const uint32_t statmentListLength, AST* ast);

MDPL_ERROR_Error* freeAST(AST* ast);

MDPL_ERROR_Error* freeHeaderSegment(HeaderSegment* headerSegment);
MDPL_ERROR_Error* freeCodeSegment(CodeSegment* codeSegment);

#endif //PARSER_HEADER_GUARD