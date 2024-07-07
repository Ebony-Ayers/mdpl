#ifndef MDPL_COMPILE_HEADER_GUARD
#define MDPL_COMPILE_HEADER_GUARD

#include "../pch.hpp"
#include "../common/mdpl_common.hpp"
#include "args.hpp"

namespace mdpl
{
    namespace compiler
    {
        int compile(mdpl::args::CLIOptions* cliOptions);

        struct FlatToken
        {
            const char* str;
        };
        struct HierarchicalToken
        {
            //
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
        int getNumHierarchicalToken(common::RAIIBuffer<FlatToken>* tokenList, const size_t& tokenListSize);
        int createTokenTree(common::RAIIBuffer<FlatToken>* tokenList, const size_t& numTokens, common::RAIIBuffer<HierarchicalToken>* tokenTree, const size_t& tokenTreeSize);
        //step 7: check for invalid syntax
        int syntaxChecker(common::RAIIBuffer<HierarchicalToken>* tokenTree, const size_t& tokenTreeSize);
        //step 8: convert the token tree into a syntax tree

        namespace internal
        {
            //copy the contence inside the literal with out the quotes
            int copyStringLiteral(mdpl::common::RAIIBuffer<char>* dst, const char* src);

            //symbols are : ! @ # $ % ^ & * ( ) - = _ + ~ ` [ ] { } ; : ' " , . < > / ?
            //note : ', @, #, $, %, and ~ are unused but reserved for consistancy and future use
            const char* isSymbol(const char* s);
            const char posibleSymbols[] = {'!', '\"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', ':', ';', '<', '=', '>', '?', '@', '[', ']', '^', '_', '`', '{', '}', '~'};
            const size_t posibleSymbolsLength = sizeof(posibleSymbols) / sizeof(char);
        }

    }
}

#endif //MDPL_COMPILE_HEADER_GUARD