#include "compile.hpp"

namespace mdpl
{
    namespace compiler
    {
        int compile(mdpl::args::CLIOptions* cliOptions)
        {
            for(size_t i = 0; i < cliOptions->files.size(); i++)
            {
                const char* fileName = cliOptions->files[i];

                //open the source file
                common::RAIIFile fileHandle(fileName, "r");
                
                //get the length of the sorce file
                fseek(fileHandle, 0, SEEK_END);
                size_t fileSize = static_cast<size_t>(ftell(fileHandle));
                fseek(fileHandle, 0, SEEK_SET);

                //read the source file into memory
                common::RAIIBuffer<char> buff;
                //overallocate memory so that we don't have to deal with bounds checking when scanning multiple characters at a time
                //allocate an extra byte at the start and 8 at the end
                buff.allocate(fileSize + 9);
                //the memory will never be writen to so it should be zeroes to avoid random errors
                buff.getBuff()[0] = '\0';
                memset(buff.getBuff() + fileSize + 1, '\0', 8);
                fread(buff.getBuff() + 1, 1, fileSize, fileHandle);

                //step 1: clean up bad characters
                MDPL_RETERR(replaceBadChars(&buff, fileSize));
                //step 2: isolate strings and remove them
                size_t staticStringsLength;
                MDPL_RETERR(getNumStrings(&buff, fileSize, &staticStringsLength, fileName));
                mdpl::common::RAIIBuffer<mdpl::common::RAIIBuffer<char>> staticStrings;
                MDPL_RETERR(staticStrings.allocate(staticStringsLength));
                MDPL_RETERR(isolateStrings(&buff, fileSize, staticStrings, staticStringsLength));
                MDPL_RETERR(removeStrings(&buff, fileSize));
                //step 3: convert all sapces to null terminating characters
                MDPL_RETERR(spacesToNull(&buff, fileSize));
                //step 4: remove comments
                MDPL_RETERR(validateComments(&buff, fileSize, fileName));
                MDPL_RETERR(removeComments(&buff, fileSize));
                //step 5: create the token list
                size_t numFlatTokens;
                MDPL_RETERR(getNumFlatTokens(&buff, fileSize, &numFlatTokens));
                mdpl::common::RAIIBuffer<SourceToken> flatTokens;
                MDPL_RETERR(createTokenList(&buff, fileSize, &flatTokens, numFlatTokens));
                //step 6: clasify the tokens
                MDPL_RETERR(validateDoubleSymbols(&flatTokens, numFlatTokens, fileName));
                MDPL_RETERR(createHeirechy(&flatTokens, numFlatTokens));
                MDPL_RETERR(identityStrings(&flatTokens, numFlatTokens, &staticStrings, staticStringsLength));
                MDPL_RETERR(identityKeywords(&flatTokens, numFlatTokens));
                MDPL_RETERR(identityFunctions(&flatTokens, numFlatTokens));
                MDPL_RETERR(identityTypes(&flatTokens, numFlatTokens));

                //when debuging dump the clasified symbols for each file
                if(cliOptions->isCompilerDebug)
                {
                    printf("\n======================== Token dump ========================\n");
                    printf("%s\n\n", fileName);
                    
                    printf("0     ");
                    int previousLineNum = 1;
                    for(size_t j = 0; j < numFlatTokens; j++)
                    {
                        SourceToken token = flatTokens.getBuff()[j];
                        if(token.lineNum != previousLineNum)
                        {
                            printf("\n%-4ld  ", j);
                        }
                        if(token.type == SourceTokenType::Symbol)
                        {
                            internal::terminalColor::setColorRed();
                            if(token.data.charTuple.c2 == '\0')
                            {
                                printf("%c ", token.data.charTuple.c1);
                            }
                            else
                            {
                                printf("%c%c ", token.data.charTuple.c1, token.data.charTuple.c2);
                            }
                            internal::terminalColor::resetColor();
                        }
                        else if(token.type == SourceTokenType::StringLiteral)
                        {
                            internal::terminalColor::setColorGreen();
                            printf("\"%s\" ", token.data.str);
                            internal::terminalColor::resetColor();
                        }
                        else if(token.type == SourceTokenType::Keyword)
                        {
                            internal::terminalColor::setColorYellow();
                            printf("%s ", internal::posibleKeywords[token.data.keywordIndex]);
                            internal::terminalColor::resetColor();
                        }
                        else if(token.type != SourceTokenType::Empty)
                        {
                            if(token.type == SourceTokenType::FunctionImplementation)
                            {
                                internal::terminalColor::setColorBlue();
                            }
                            else if(token.type == SourceTokenType::FunctionCall)
                            {
                                internal::terminalColor::setColorMagenta();
                            }
                            else if(token.type == SourceTokenType::Type)
                            {
                                internal::terminalColor::setColorCyan();
                            }
                            printf("%s ", token.data.str);
                            internal::terminalColor::resetColor();
                        }
                        previousLineNum = token.lineNum;
                    }
                    printf("\n");
                }
            }
            return 0;
        }

        int replaceBadChars(common::RAIIBuffer<char>* buff, const size_t& bufferLength)
        {
            char* str = buff->getBuff();
            for(size_t i = 0; i < bufferLength; i++)
            {
                switch (str[i])
                {
                case '\r':
                    str[i] = '\0';
                    break;
                default:
                    break;
                }
            }
            return 0;
        }

        int getNumStrings(common::RAIIBuffer<char>* buff, const size_t& bufferLength, size_t* numStaticStrings, const char* file)
        {
            char* str = buff->getBuff();

            int numDoubleQuotes = 0;
            //in the event that the very first character is a dounble quote
            if(str[0] == '\"')
            {
                numDoubleQuotes = 1;
            }
            int lineNum = 0;
            for(size_t i = 1; i < bufferLength; i++)
            {
                if(str[i] == '\n')
                {
                    lineNum++;
                }
                if((str[i] == '\"') && (str[i-1] != '\\'))
                {
                    numDoubleQuotes++;
                }
            }
            if(numDoubleQuotes % 2 == 1)
            {
                printf("%d\n", numDoubleQuotes);
                printf("Syntax error: \" is is missing corresponding \".\nFile: %s, line: %d\n\n", file, lineNum);
                return 1;
            }
            else
            {
                *numStaticStrings = static_cast<size_t>(numDoubleQuotes) / 2;
                return 0;
            }
            return 0;
        }
        int isolateStrings(common::RAIIBuffer<char>* buff, const size_t& bufferLength, mdpl::common::RAIIBuffer<mdpl::common::RAIIBuffer<char>>& staticStrings, const size_t& staticStringsLength)
        {
            char* str = buff->getBuff();

            bool isOpeningQuote = true;
            size_t staticStringsIndex = 0;
            //in the event that the very first character is a dounble quote
            if(str[0] == '\"')
            {
                MDPL_RETERR(internal::copyStringLiteral(&(staticStrings[0]), &(str[0])));
                staticStringsIndex++;
                isOpeningQuote = false;
            }
            for(size_t i = 1; i < bufferLength; i++)
            {
                if((str[i] == '\"') && (str[i-1] != '\\'))
                {
                    if(isOpeningQuote)
                    {
                        if(staticStringsIndex >= staticStringsLength)
                        {
                            printf("Error: isolateStrings exceded length of staticStrings.\n");
                            return 1;
                        }
                        MDPL_RETERR(internal::copyStringLiteral(&(staticStrings[staticStringsIndex]), &(str[i])));
                        staticStringsIndex++;
                        isOpeningQuote = false;
                    }
                    else
                    {
                        isOpeningQuote = true;
                    }
                }
            }

            return 0;
        }
        int removeStrings(common::RAIIBuffer<char>* buff, const size_t& bufferLength)
        {
            char* str = buff->getBuff();

            bool insideString = false;
            //in the event that the very first character is a dounble quote
            if(str[0] == '\"')
            {
                insideString = true;
            }
            for(size_t i = 1; i < bufferLength; i++)
            {
                if((str[i] == '\"') && (str[i-1] != '\\'))
                {
                    insideString = !insideString;
                }
                else
                {
                    if(insideString)
                    {
                        if(str[i-1] == '\\')
                        {
                            str[i - 1] = '\0';
                            str[i] = '\0';
                            i++;
                        }
                        else
                        {
                            if(str[i] != '\n')
                            {
                                str[i] = '\0';
                            }
                        }
                    }
                }
            }

            return 0;
        }

        int spacesToNull(common::RAIIBuffer<char>* buff, const size_t& bufferLength)
        {
            char* str = buff->getBuff();
            for(size_t i = 0; i < bufferLength; i++)
            {
                if((str[i] == ' ') || (str[i] == '\t'))
                {
                    str[i] = '\0';
                }
            }
            return 0;
        }

        int validateComments(common::RAIIBuffer<char>* buff, const size_t& bufferLength, const char* file)
        {
            char* str = buff->getBuff();
            //0 - none
            //1 - block comment
            //2 - line comment
            int commentType = 0;
            int lineNum = 0;
            for(size_t i = 0; i < bufferLength; i++)
            {
                if(str[i] == '\n')
                {
                    lineNum++;
                }
                switch(commentType)
                {
                    case 0:
                        if((str[i] == '/') && (str[i+1] == '*'))
                        {
                            commentType = 1;
                            i++;
                        }
                        else if((str[i] == '/') && (str[i+1] == '/'))
                        {
                            commentType = 2;
                            i++;
                        }
                        else
                        {
                            continue;
                        }
                        break;
                    case 1:
                        if((str[i] == '*') && (str[i+1] == '/'))
                        {
                            commentType = 0;
                            i++;
                        }
                        break;
                    case 2:
                        if(str[i] == '\n')
                        {
                            commentType = 0;
                        }
                        break;
                    default:
                        break;
                }
            }
            if(commentType == 1)
            {
                printf("Syntax error: /* is missing */.\nFile: %s, line: %d\n\n", file, lineNum);
                return 1;
            }
            else
            {
                return 0;
            }
        }
        int removeComments(common::RAIIBuffer<char>* buff, const size_t& bufferLength)
        {
            char* str = buff->getBuff();

            //0 - none
            //1 - block comment
            //2 - line comment
            int commentType = 0;
            for(size_t i = 0; i < bufferLength; i++)
            {
                switch(commentType)
                {
                    case 0:
                        if((str[i] == '/') && (str[i+1] == '*'))
                        {
                            str[i] = '\0';
                            str[i+1] = '\0';
                            commentType = 1;
                            i++;
                        }
                        else if((str[i] == '/') && (str[i+1] == '/'))
                        {
                            str[i] = '\0';
                            str[i+1] = '\0';
                            commentType = 2;
                            i++;
                        }
                        else
                        {
                            continue;
                        }
                        break;
                    case 1:
                        if((str[i] == '*') && (str[i+1] == '/'))
                        {
                            str[i] = '\0';
                            str[i+1] = '\0';
                            commentType = 0;
                            i++;
                        }
                        else
                        {
                            str[i] = '\0';
                        }
                        break;
                    case 2:
                        if(str[i] == '\n')
                        {
                            commentType = 0;
                        }
                        else
                        {
                            str[i] = '\0';
                        }
                        break;
                    default:
                        break;
                }
            }
            if(commentType == 1)
            {
                printf("Error: at end of removeComments() not all block comments have been closed.\n");
                return 1;
            }
            return 0;
        }

        int getNumFlatTokens(common::RAIIBuffer<char>* buff, const size_t& bufferLength, size_t* numTokens)
        {
            char* str = buff->getBuff();
            *numTokens = 0;

            bool previousIsNullTerminating = (str[0] == '\0');
            //the buffer starts at index 1 so we need to loop to length plus one
            for(size_t i = 1; i < bufferLength + 1; i++)
            {
                if(internal::isSymbol(str[i]))
                {
                    (*numTokens)++;
                    previousIsNullTerminating = true;
                    if(internal::isSymbol(str[i+1]) && (str[i] != '\"') && (str[i+1] != '\"'))
                    {
                        i += 1;
                    }
                }
                else if(str[i] == '\n')
                {
                    previousIsNullTerminating = true;
                }
                else if(str[i] == '\0')
                {
                    previousIsNullTerminating = true;
                }
                else
                {
                    if(previousIsNullTerminating)
                    {
                        (*numTokens)++;
                        previousIsNullTerminating = false;
                    }
                }
            }

            return 0;
        }
        int createTokenList(common::RAIIBuffer<char>* buff, const size_t& bufferLength, common::RAIIBuffer<SourceToken>* tokenList, const size_t& tokenListSize)
        {
            MDPL_RETERR(tokenList->allocate(tokenListSize));
            char* str = buff->getBuff();

            size_t tokenIndex = 0;
            int lineNum = 1;
            //the buffer starts at index 1 so we need to loop to length plus one
            for(size_t i = 1; i < bufferLength + 1; i++)
            {
                if(tokenIndex >= tokenListSize)
                {
                    printf("Error: in createTokenList(), token index exceded token list size. Skipping rest of file.\n");
                    return 1;
                }
                if(internal::isSymbol(str[i]))
                {
                    tokenList->getBuff()[tokenIndex].type = SourceTokenType::Symbol;
                    tokenList->getBuff()[tokenIndex].lineNum = lineNum;
                    tokenList->getBuff()[tokenIndex].heirachy = 0;
                    tokenList->getBuff()[tokenIndex].data.charTuple.c1 = str[i];
                    if(internal::isSymbol(str[i+1]) && (str[i] != '\"') && (str[i+1] != '\"'))
                    {
                        tokenList->getBuff()[tokenIndex].data.charTuple.c2 = str[i+1];
                        str[i+1] = '\0';
                    }
                    str[i] = '\0';
                    tokenIndex++;
                }
                else if(str[i] == '\n')
                {
                    str[i] = '\0';
                    lineNum++;
                }
                else if((str[i] != '\0') && (str[i-1] == '\0'))
                {
                    tokenList->getBuff()[tokenIndex].type = SourceTokenType::Uncatagorised;
                    tokenList->getBuff()[tokenIndex].lineNum = lineNum;
                    tokenList->getBuff()[tokenIndex].heirachy = 0;
                    tokenList->getBuff()[tokenIndex].data.str = str + i;
                    tokenIndex++;
                }
            }

            return 0;
        }

        int validateDoubleSymbols(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens, const char* file)
        {
            for(size_t i = 0; i < numTokens; i++)
            {
                SourceToken token = tokenList->getBuff()[i];
                if(token.type == SourceTokenType::Symbol)
                {
                    if(token.data.charTuple.c2 != '\0')
                    {
                        if(!internal::isDoubleSymbol(token.data.charTuple))
                        {
                            printf("Syntax error: unknown symbol \"%c%c\".\nFile: %s, line: %d\n\n", token.data.charTuple.c1, token.data.charTuple.c2, file, token.lineNum);
                            return 1;
                        }
                    }
                }
            }
            return 0;
        }
        int createHeirechy(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens)
        {
            int currentHeirachyLevel = 0;
            for(size_t i = 0; i < numTokens; i++)
            {
                SourceToken token = tokenList->getBuff()[i];
                if(token.type == SourceTokenType::Symbol)
                {
                    if(token.data.charTuple.c2 == '\0')
                    {
                        if((token.data.charTuple.c1 == '(') || (token.data.charTuple.c1 == '[') || (token.data.charTuple.c1 == '{'))
                        {
                            currentHeirachyLevel++;
                        }
                        else if((token.data.charTuple.c1 == ')') || (token.data.charTuple.c1 == ']') || (token.data.charTuple.c1 == '}'))
                        {
                            currentHeirachyLevel--;
                        }
                    }
                }
                tokenList->getBuff()[i].heirachy = currentHeirachyLevel;
            }
            if(currentHeirachyLevel != 0)
            {
                printf("Something went wrong when creating heirachy. Final heirachy is %d.\n", currentHeirachyLevel);
            }
            return 0;
        }
        int identityStrings(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens, mdpl::common::RAIIBuffer<mdpl::common::RAIIBuffer<char>>* staticStrings, const size_t& staticStringsLength)
        {
            for(size_t i = 0; i < numTokens - 1; i++)
            {
                SourceToken curentToken = tokenList->getBuff()[i];
                SourceToken nextToken = tokenList->getBuff()[i+1];
                size_t literalIndex = 0;
                if(curentToken.type == SourceTokenType::Symbol)
                {
                    if((curentToken.data.charTuple.c1 == '\"') && (curentToken.data.charTuple.c2 == '\0'))
                    {
                        if((nextToken.type != SourceTokenType::Symbol) || !((nextToken.data.charTuple.c1 == '\"') && (nextToken.data.charTuple.c2 == '\0')))
                        {
                            printf("Error: in identityStrings(). Symbol token \" was not followed by a seocnd symbol token \".\n");
                            return 1;
                        }
                        else
                        {   
                            if(literalIndex >= staticStringsLength)
                            {
                                printf("Error: in identityStrings() literalIndex >= staticStringsLength.\n");
                                return 1;
                            }
                            tokenList->getBuff()[i].type = SourceTokenType::StringLiteral;
                            tokenList->getBuff()[i].data.str = staticStrings->getBuff()[literalIndex];
                            tokenList->getBuff()[i+1].type = SourceTokenType::Empty;
                            i++;
                        }
                    }
                }
            }
            return 0;
        }
        int identityKeywords(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens)
        {
            for(size_t i = 0; i < numTokens; i++)
            {
                SourceToken curentToken = tokenList->getBuff()[i];
                if(curentToken.type == SourceTokenType::Uncatagorised)
                {
                    size_t keywordIndex = internal::clasifyKeyword(curentToken.data.str);
                    if(keywordIndex != 0)
                    {
                        tokenList->getBuff()[i].type = SourceTokenType::Keyword;
                        tokenList->getBuff()[i].data.keywordIndex = keywordIndex;
                    }
                }
            }
            return 0;
        }
        int identityFunctions(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens)
        {
            for(size_t i = 1; i < numTokens - 1; i++)
            {
                SourceToken previousToken = tokenList->getBuff()[i-1];
                SourceToken curentToken = tokenList->getBuff()[i];
                SourceToken nextToken = tokenList->getBuff()[i+1];
                if(curentToken.type == SourceTokenType::Uncatagorised)
                {
                    if(nextToken.type == SourceTokenType::Symbol)
                    {
                        if((nextToken.data.charTuple.c1 == '(') && (nextToken.data.charTuple.c2 == '\0'))
                        {
                            if(previousToken.type == SourceTokenType::Keyword)
                            {
                                if((previousToken.data.keywordIndex == MDPL_KEYWORD_ENUM_FN) || (previousToken.data.keywordIndex == MDPL_KEYWORD_ENUM_SUB))
                                {
                                    tokenList->getBuff()[i].type = SourceTokenType::FunctionImplementation;
                                }
                                else
                                {
                                    tokenList->getBuff()[i].type = SourceTokenType::FunctionCall;
                                }
                            }
                            else
                            {
                                tokenList->getBuff()[i].type = SourceTokenType::FunctionCall;
                            }
                        }
                    }
                }
            }
            return 0;
        }
        int identityTypes(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens)
        {
            int numOpenFunctions = 0;
            for(size_t i = 1; i < numTokens; i++)
            {
                SourceToken previousToken = tokenList->getBuff()[i-1];
                SourceToken curentToken = tokenList->getBuff()[i];
                if(numOpenFunctions != 0)
                {
                    if(curentToken.type == SourceTokenType::Uncatagorised)
                    {
                        if(previousToken.type == SourceTokenType::Symbol)
                        {
                            if(((previousToken.data.charTuple.c1 == '(') && (previousToken.data.charTuple.c2 == '\0')) || ((previousToken.data.charTuple.c1 == ',') && (previousToken.data.charTuple.c2 == '\0')))
                            {
                                tokenList->getBuff()[i].type = SourceTokenType::Type;
                            }
                        }
                    }
                    else if(curentToken.type == SourceTokenType::Symbol)
                    {
                        if((curentToken.data.charTuple.c1 == '(') && (curentToken.data.charTuple.c2 == '\0'))
                        {
                            numOpenFunctions++;
                        }
                        else if((curentToken.data.charTuple.c1 == ')') && (curentToken.data.charTuple.c2 == '\0'))
                        {
                            numOpenFunctions--;
                        }
                    }
                }
                else
                {
                    if(curentToken.type == SourceTokenType::Uncatagorised)
                    {
                        if(previousToken.type == SourceTokenType::Keyword)
                        {
                            if(previousToken.data.keywordIndex == MDPL_KEYWORD_ENUM_LET)
                            {
                                tokenList->getBuff()[i].type = SourceTokenType::Type;
                            }
                        }
                    }
                    else if(curentToken.type == SourceTokenType::FunctionImplementation)
                    {
                        numOpenFunctions = 1;
                        i++;
                    }
                    else if(curentToken.type == SourceTokenType::Symbol)
                    {
                        if((curentToken.data.charTuple.c1 == '-') && (curentToken.data.charTuple.c1 == '>'))
                        {
                            numOpenFunctions = 1;
                            i++;
                        }
                    }
                }
            }
            return 0;
        }

        namespace internal
        {
            int copyStringLiteral(mdpl::common::RAIIBuffer<char>* dst, const char* src)
            {
                if(src[0] != '\"')
                {
                    printf("Error: copyStringLiteral called with string not starting with a \".\n");
                    return 1;
                }
                const char* endQuote = strchr(&(src[1]), '\"');
                if(endQuote == nullptr)
                {
                    printf("Error: copyStringLiteral could not find closing double quote.\n");
                    return 1;
                }
                //does not include the null terminating character
                size_t stringLength = static_cast<size_t>(endQuote - src) - 1;
                dst->allocate(stringLength + 1);
                //offset by one to skip the opening quote
                memcpy(dst->getBuff(), src + 1, stringLength * sizeof(char));
                dst->getBuff()[stringLength] = '\0';
                return 0;
            }

            bool isSymbol(const char& c)
            {
                for(size_t i = 0; i < posibleSymbolsLength; i++)
                {
                    if(c == posibleSymbols[i])
                    {
                        return true;
                    }
                }
                return false;
            }

            size_t clasifyKeyword(const char* s)
            {
                for(size_t i = 1; i < posibleKeywordsLength; i++)
                {
                    if(strcmp(s, posibleKeywords[i]) == 0)
                    {
                        return i;
                    }
                }
                return 0;
            }

            bool isDoubleSymbol(const CharTuple& t)
            {
                if((t.c1 == '\"') || (t.c2 == '\"'))
                {
                    return false;
                }
                else if(t.c2 == '=')
                {
                    for(size_t i = 0; i < symbolsThatCombineWithEqualsLength; i++)
                    {
                        if(t.c1 == symbolsThatCombineWithEquals[i])
                        {
                            return true;
                        }
                    }
                }
                else
                {
                    for(size_t i = 0; i < nonEqualsDoubleSymbolsLength; i++)
                    {
                        CharTuple other = nonEqualsDoubleSymbols[i];
                        if((t.c1 == other.c1) && (t.c2 == other.c2))
                        {
                            return true;
                        }
                    }
                }
                return false;
            }

            namespace terminalColor
            {
                void setColorRed()
                {
                    printf("\x1b[31m");
                }
                void setColorGreen()
                {
                    printf("\x1b[32m");
                }
                void setColorYellow()
                {
                    printf("\x1b[33m");
                }
                void setColorBlue()
                {
                    printf("\x1b[34m");
                }
                void setColorMagenta()
                {
                    printf("\x1b[35m");
                }
                void setColorCyan()
                {
                    printf("\x1b[36m");
                }
                void resetColor()
                {
                    printf("\x1b[0m");
                }
            }
        }
    }
}