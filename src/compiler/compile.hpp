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

        struct FlatToken
        {
            const char* str;
            int lineNum;
        };
        enum class HierarchicalTokenType
        {
            Symbol,
            DoubleSymbol,
            Keyword,
            Type,
            Variable,
            Function,
            Uncatagorised
        };
        struct HierarchicalToken
        {
            HierarchicalTokenType type;
            const char* value;
            HierarchicalToken* next;
            HierarchicalToken* prev;
            HierarchicalToken* inside;
            int lineNum;
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
        int createTokenList(common::RAIIBuffer<char>* buff, const size_t& bufferLength, common::RAIIBuffer<FlatToken>* tokenList, const size_t& tokenListSize);
        //step 6: convert the token list into a token tree
        int catagoriseTokens(common::RAIIBuffer<FlatToken>* tokenList, const size_t& numTokens, common::RAIIBuffer<HierarchicalToken>* tokenTree);
        int linkTokens(common::RAIIBuffer<FlatToken>* tokenList, const size_t& numTokens, common::RAIIBuffer<HierarchicalToken>* tokenTree);
        int mergeDoubleSymbols(common::RAIIBuffer<FlatToken>* tokenList, const size_t& numTokens, common::RAIIBuffer<HierarchicalToken>* tokenTree, size_t* tokenTreeLength);
        //step 7: check for invalid syntax
        int syntaxChecker(common::RAIIBuffer<HierarchicalToken>* tokenTree, const size_t& tokenTreeSize);
        //step 8: convert the token tree into a syntax tree

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
                "ref",
                "generic",
                "prop",
                "if",
                "elif",
                "else",
                "for",
                "while",
                "continue",
                "break",
                "switch",
                "case",
                "struct",
                "enum",
                "typedef",
                "and",
                "or",
                "not",
            };
            const size_t posibleKeywordsLength = sizeof(posibleKeywords) / sizeof(const char* const);
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
            bool isDoubleSymbol(const char& a, const char& b);
            const char symbolsThatCombineWithEquals[] = {'!', '#', '$', '%', '&', '*', '+', '-', '/', '<', '=', '>', '@', '^', '~'};
            const size_t symbolsThatCombineWithEqualsLength = sizeof(symbolsThatCombineWithEquals) / sizeof(char);
        }

    }
}

#endif //MDPL_COMPILE_HEADER_GUARD