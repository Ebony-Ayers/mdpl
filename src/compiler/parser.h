#ifndef PARSER_HEADER_GUARD
#define PARSER_HEADER_GUARD

#include "../pch.h"
#include "../common/mdpl_common.h"
#include "../runtime_lib/mdpl_error.h"

#include "tokeniser.h"
#include "symbol_table.h"

#define MDPL_COMPILER_PARAMETER_QUALIFIERS_undefined   0
#define MDPL_COMPILER_PARAMETER_QUALIFIERS_const       1
#define MDPL_COMPILER_PARAMETER_QUALIFIERS_mut         2
#define MDPL_COMPILER_PARAMETER_QUALIFIERS_ref         4
#define MDPL_COMPILER_PARAMETER_QUALIFIERS_returnable  8

#if MDPL_COMPILER == MDPL_COMPILER_CLANG
	__attribute__((packed))
#elif MDPL_COMPILER == MDPL_COMPILER_GCC
	__attribute__((packed))
#elif MDPL_COMPILER == MDPL_COMPILER_MSVC
	#pragma pack(push, 1)
#elif MDPL_COMPILER == MDPL_COMPILER_INTEL
	#pragma pack(1)
#endif
typedef struct Type
{
    Symbol name;
    uint16_t pointerDepth;
    uint16_t qualifiers;
} Type;
#if MDPL_COMPILER == MDPL_COMPILER_MSVC
	#pragma pack(pop)
#elif MDPL_COMPILER == MDPL_COMPILER_INTEL
	#pragma pack()
#endif

typedef struct Literal
{
    Type type;
    uint64_t value; //should reinterpret cast to appriorite type
} Literal;

#define MDPL_COMPILER_RPN_ELEMENT_TYPE_undefined  0
#define MDPL_COMPILER_RPN_ELEMENT_TYPE_literal    1
#define MDPL_COMPILER_RPN_ELEMENT_TYPE_variable   2
#define MDPL_COMPILER_RPN_ELEMENT_TYPE_function   3
#define MDPL_COMPILER_RPN_ELEMENT_TYPE_operator   4

typedef struct RPNElement
{
    uint32_t type;
    union
    {
        Literal literal;
        Symbol variable;
        int function; //TODO: implement functions calls
        uint32_t symbol;
    };
} RPNElement;

typedef struct Expression
{
    bool isControlFlow;
    RPNElement* elements;
    uint32_t numElements;
    //the following are unused if isControlFlow is false
    uint32_t nextExpressionAtOriginalScope;
    uint32_t expressionStartIndex;
    uint32_t expressionEndIndex;
} Expression;

typedef struct Structure
{
    Type* types;
    Symbol* names;
    uint32_t numFields;
} Structure;

typedef struct GlobalVariable
{
    Symbol name;
    Type type;
} GlobalVariable;

typedef struct Parameter
{
    Type type;
    Symbol name;
} Parameter;

typedef struct FunctionSignature
{
    Symbol name;
    Parameter* inputs;
    uint32_t numInputs;
    Parameter* ouputs;
    uint32_t numOutputs;
} FunctionSignature;

typedef struct FunctionImplementation
{
    FunctionSignature signature;
    Expression* expressions;
    uint32_t numExpressions;
} FunctionImplementation;

//equivilent of .h file
typedef struct HeaderSegment
{
    FunctionSignature* signatures;
    uint32_t numSignatures;
    //equivilent of #define
    GlobalVariable* compileTimeGlobals;
    uint32_t numCompileTimeGlobals;
    //equivilent of globals
    GlobalVariable* runtimeGlobals;
    uint32_t numRuntimeGlobals;
    Structure* structs;
    uint32_t numStructs;
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

//public:
MDPL_ERROR_Error* generateAST(Token* tokenList, const uint32_t tokenListLength, Statment* statmentList, const uint32_t statmentListLength, bool isHeader, AST* ast);
MDPL_ERROR_Error* generateASTHeader(Token* tokenList, const uint32_t tokenListLength, Statment* statmentList, const uint32_t statmentListLength, AST* ast);
MDPL_ERROR_Error* generateASTCode(Token* tokenList, const uint32_t tokenListLength, Statment* statmentList, const uint32_t statmentListLength, AST* ast);

MDPL_ERROR_Error* freeAST(AST* ast);

MDPL_ERROR_Error* freeHeaderSegment(HeaderSegment* headerSegment);
MDPL_ERROR_Error* freeCodeSegment(CodeSegment* codeSegment);

//protected:

#endif //PARSER_HEADER_GUARD