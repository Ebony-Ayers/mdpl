#ifndef MDPL_COMPILE_HEADER_GUARD
#define MDPL_COMPILE_HEADER_GUARD

#include "../pch.hpp"
#include "../common/mdpl_common.hpp"
#include "args.hpp"

#include <cstdint>

namespace mdpl
{
    namespace compiler
    {
        int compile(mdpl::args::CLIOptions* cliOptions);

        enum class SourceTokenType
        {
            Empty,
            Symbol,
            Keyword,
            StringLiteral,
            FunctionImplementation,
            FunctionCall,
            Type,
            Variable,
            Uncatagorised
        };
        struct CharTuple
        {
            char c1;
            char c2;
        };
        struct SourceToken
        {
            SourceTokenType type;
            int lineNum;
            int heirachy;
            union
            {
                const char* str;
                CharTuple charTuple;
                size_t keywordIndex;
            } data;
        };
        struct Statment
        {
            size_t startTokenIndex;
            size_t stopTokenIndex;
            size_t attachedScopeIndex;
        };
        struct Scope
        {
            size_t startStatmentIndex;
            size_t stopTokenIndex;
        };
        
        //step 1: clean up bad characters
        int replaceBadChars(common::RAIIBuffer<char>* buff, const size_t& bufferLength);
        //step 2: isolate strings and remove them
        int getNumStrings(common::RAIIBuffer<char>* buff, const size_t& bufferLength, size_t* numStaticStrings, const char* file);
        int isolateStrings(common::RAIIBuffer<char>* buff, const size_t& bufferLength, mdpl::common::RAIIBuffer<mdpl::common::RAIIBuffer<char>>& staticStrings, const size_t& staticStringsLength);
        int removeStrings(common::RAIIBuffer<char>* buff, const size_t& bufferLength);
        //step 3: convert all sapces to null terminating characters
        int spacesToNull(common::RAIIBuffer<char>* buff, const size_t& bufferLength);
        //step 4: remove comments
        int validateComments(common::RAIIBuffer<char>* buff, const size_t& bufferLength, const char* file);
        int removeComments(common::RAIIBuffer<char>* buff, const size_t& bufferLength);
        //step 5: create the token list
        int getNumFlatTokens(common::RAIIBuffer<char>* buff, const size_t& bufferLength, size_t* numTokens);
        int createTokenList(common::RAIIBuffer<char>* buff, const size_t& bufferLength, common::RAIIBuffer<SourceToken>* tokenList, const size_t& tokenListSize);
        //step 6: clasify the tokens
        int validateDoubleSymbols(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens, const char* file);
        int createHeirechy(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens);
        int identityStrings(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens, mdpl::common::RAIIBuffer<mdpl::common::RAIIBuffer<char>>* staticStrings, const size_t& staticStringsLength);
        int identityKeywords(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens);
        int identityFunctions(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens);
        int identityTypes(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens);
        //step 7: group tokens
        int getNumStatments(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens, size_t* numStatments);
        int groupStatments(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens, const size_t& numStatments, common::RAIIBuffer<Statment>* statmentList);
        int getNumScopes(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens, const size_t& numStatments, common::RAIIBuffer<Statment>* statmentList, size_t* numScopes);
        int groupScopes(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens, const size_t& numStatments, common::RAIIBuffer<Statment>* statmentList, const size_t& numScopes, common::RAIIBuffer<Scope>* scopeList);
        int linkScopesToControlFlow(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens, const size_t& numStatments, common::RAIIBuffer<Statment>* statmentList, const size_t& numScopes, common::RAIIBuffer<Scope>* scopeList);

        namespace internal
        {
            //copy the contence inside the literal with out the quotes
            int copyStringLiteral(mdpl::common::RAIIBuffer<char>* dst, const char* src);

            //symbols are : ! @ # $ % ^ & * ( ) - = + ~ ` [ ] { } ; : ' " , . < > / ?
            //note : ', @, #, $, %, and ~ are unused but reserved for consistancy and future use
            bool isSymbol(const char& c);
            const char posibleSymbols[] = {'!', '\"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', ':', ';', '<', '=', '>', '?', '@', '[', ']', '^', '`', '{', '}', '~'};
            const size_t posibleSymbolsLength = sizeof(posibleSymbols) / sizeof(char);

            size_t clasifyKeyword(const char* s);
            //the first keyword is empty to signify that a index of 0 represnets not a keyword
            const char* const posibleKeywords[] = {
                "",
                "namespace",
                "public",
                "protected",
                "code",
                "fn",
                "sub",
                "ret",
                "ref",
                "generic",
                "prop",
                "catch",
                "if",
                "elif",
                "else",
                "for",
                "while",
                "from",
                "to",
                "step",
                "continue",
                "break",
                "switch",
                "case",
                "let",
                "struct",
                "enum",
                "using",
                "and",
                "or",
                "not",
            };
            const size_t posibleKeywordsLength = sizeof(posibleKeywords) / sizeof(const char* const);
            #define MDPL_KEYWORD_ENUM_NAMESPACE 1
            #define MDPL_KEYWORD_ENUM_PUBLIC 2
            #define MDPL_KEYWORD_ENUM_PROTECTED 3
            #define MDPL_KEYWORD_ENUM_CODE 4
            #define MDPL_KEYWORD_ENUM_FN 5
            #define MDPL_KEYWORD_ENUM_SUB 6
            #define MDPL_KEYWORD_ENUM_RET 7
            #define MDPL_KEYWORD_ENUM_REF 8
            #define MDPL_KEYWORD_ENUM_GENERIC 9
            #define MDPL_KEYWORD_ENUM_PROP 10
            #define MDPL_KEYWORD_ENUM_CATCH 11
            #define MDPL_KEYWORD_ENUM_IF 12
            #define MDPL_KEYWORD_ENUM_ELIF 13
            #define MDPL_KEYWORD_ENUM_ELSE 14
            #define MDPL_KEYWORD_ENUM_FOR 15
            #define MDPL_KEYWORD_ENUM_WHILE 16
            #define MDPL_KEYWORD_ENUM_CONTINUE 17
            #define MDPL_KEYWORD_ENUM_BREAK 18
            #define MDPL_KEYWORD_ENUM_SWITCH 19
            #define MDPL_KEYWORD_ENUM_CASE 20
            #define MDPL_KEYWORD_ENUM_LET 21
            #define MDPL_KEYWORD_ENUM_STRUCT 22
            #define MDPL_KEYWORD_ENUM_ENUM 23
            #define MDPL_KEYWORD_ENUM_TYPEDEF 24
            #define MDPL_KEYWORD_ENUM_AND 25
            #define MDPL_KEYWORD_ENUM_OR 26
            #define MDPL_KEYWORD_ENUM_NOT 27
            const char* const buildInTypes[] = {
                "",
                "int8",
                "int16",
                "int32",
                "int64",
                "uint8",
                "uint16",
                "uint32",
                "uint64",
                "float16",
                "float32",
                "float64",
                "string",
                "bool",
                "error"
            };
            const size_t buildInTypesLength = sizeof(buildInTypes) / sizeof(const char* const);

            //symbol pairs operator and =, ->, ::
            bool isDoubleSymbol(const CharTuple& t);
            const char symbolsThatCombineWithEquals[] = {'!', '#', '$', '%', '&', '*', '+', '-', '/', '<', '=', '>', '@', '^', '~'};
            const size_t symbolsThatCombineWithEqualsLength = sizeof(symbolsThatCombineWithEquals) / sizeof(char);
            const CharTuple nonEqualsDoubleSymbols[] = {
                                                        {'-', '>'},
                                                        {':', ':'},
                                                        {'(', ')'},
                                                        {'{', '}'},
                                                        };
            const size_t nonEqualsDoubleSymbolsLength = sizeof(nonEqualsDoubleSymbols) / sizeof(CharTuple);

            namespace terminalColor
            {
                void setColorRed();
                void setColorGreen();
                void setColorYellow();
                void setColorBlue();
                void setColorMagenta();
                void setColorCyan();
                void resetColor();
            }
        }

    }
}

#endif //MDPL_COMPILE_HEADER_GUARD