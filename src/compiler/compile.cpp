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
                MDPL_RETERR(identityTypes(&flatTokens, numFlatTokens, fileName));
                //step 7: group tokens
                size_t numScopes;
                MDPL_RETERR(getNumScopes(&flatTokens, numFlatTokens, &numScopes));
                mdpl::common::RAIIBuffer<Scope> scopeList;
                MDPL_RETERR(groupScopes(&flatTokens, numFlatTokens, numScopes, &scopeList));
                size_t numStatments;
                MDPL_RETERR(getNumStatments(&flatTokens, numFlatTokens, &numStatments));
                mdpl::common::RAIIBuffer<Statment> statmentList;
                MDPL_RETERR(groupStatments(&flatTokens, numFlatTokens, numStatments, &statmentList));
                MDPL_RETERR(linkScopesToStatments(numFlatTokens, numStatments, &statmentList, numScopes, &scopeList));

                //when debuging dump the clasified symbols for each file
                if(cliOptions->isCompilerDebug)
                {
                    printf("\n======================== Token dump ========================\n");
                    printf("%s\n\n", fileName);
                    
                    //we print the number at the end of the loop as we need to manually print the zero
                    printf("0     ");
                    int previousLineNum = 1;
                    int numIndents = 0;
                    int statmentIndex = 0;
                    for(size_t j = 0; j < numFlatTokens; j++)
                    {
                        SourceToken token = flatTokens.getBuff()[j];
                        Statment statment = statmentList.getBuff()[statmentIndex];
                        //detech change of statments
                        bool shouldStartUnderline = false;
                        //to allow a topen to be the end and start of different statments the end must be checked seperate and before the start
                        if(j == statment.stopTokenIndex)
                        {
                            internal::terminalColor::underlineOff();
                            statmentIndex++;
                            //a token can be the end of one statment and the start of another as statments are up to but not including
                            statment = statmentList.getBuff()[statmentIndex];
                        }
                        if(j == statment.startTokenIndex)
                        {
                            shouldStartUnderline = true;
                        }
                        //detect a new line
                        if(token.lineNum != previousLineNum)
                        {
                            //print a line followed by an int padded 4 to the left
                            printf("\n%-4ld  ", j);
                            for(int k = 0; k < numIndents; k++)
                            {
                                printf("    ");
                            }
                        }
                        if(token.type == SourceTokenType::Symbol)
                        {
                            internal::terminalColor::setColorRed();
                            if(shouldStartUnderline)
                            {
                                internal::terminalColor::underlineOn();
                            }
                            //check if the token is one or two chars long
                            if(token.data.charTuple.c2 == '\0')
                            {
                                if(token.data.charTuple.c1 == '{')
                                {
                                    numIndents++;
                                }
                                else if(token.data.charTuple.c1 == '}')
                                {
                                    numIndents--;
                                    //to be able to reduce the number of intents we need to reprint the current line
                                    internal::terminalColor::resetColor();
                                    printf("\r%-4ld  ", j);
                                    for(int k = 0; k < numIndents; k++)
                                    {
                                        printf("    ");
                                    }
                                    internal::terminalColor::setColorRed();
                                }

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
                            if(shouldStartUnderline)
                            {
                                internal::terminalColor::underlineOn();
                            }
                            printf("\"%s\" ", token.data.str);
                            internal::terminalColor::resetColor();
                        }
                        else if(token.type == SourceTokenType::Keyword)
                        {
                            internal::terminalColor::setColorYellow();
                            if(shouldStartUnderline)
                            {
                                internal::terminalColor::underlineOn();
                            }
                            printf("%s ", internal::posibleKeywords[token.data.keywordIndex]);
                            internal::terminalColor::resetColor();
                        }
                        else if(token.type != SourceTokenType::Empty)
                        {
                            if(shouldStartUnderline)
                            {
                                internal::terminalColor::underlineOn();
                            }
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

                    printf("\n======================== Scope dump ========================\nstartTokenIndex stopTokenIndex | startStatmentIndex stopStatmentIndex\n");
                    for(size_t k = 0; k < numScopes; k++)
                    {
                        Scope scope = scopeList.getBuff()[k];
                        printf("%lu %lu | %lu %lu\n", scope.startTokenIndex, scope.stopTokenIndex, scope.startStatmentIndex, scope.stopStatmentIndex);
                    }

                    //the statment dump is irrelevant when the underlining works correctly so this has been left here for debugging if underlining breaks
                    #if 0
                    printf("\n======================== Statment dump ========================\n");
                    for(size_t k = 0; k < numStatments; k++)
                    {
                        Statment statment = statmentList.getBuff()[k];
                        printf("%lu %lu\n", statment.startTokenIndex, statment.stopTokenIndex);
                    }
                    #endif
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
        int identityTypes(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens, const char* file)
        {
            int numOpenBrakets = 0;
            int numCloseBrakets = 0;
            bool isFunctionImplementation = false;
            int activeLineNum = 0;
            for(size_t i = 1; i < numTokens; i++)
            {
                SourceToken previousToken = tokenList->getBuff()[i-1];
                SourceToken curentToken = tokenList->getBuff()[i];
                //as you cannot have two function declarations of definitions on the same line a change of line indicates a change of statment
                if(curentToken.lineNum != activeLineNum)
                {
                    //if the previous line was a function definition check if it correctly formatted
                    if(isFunctionImplementation)
                    {
                        if((numOpenBrakets == 2) && (numCloseBrakets == 2))
                        {
                            numOpenBrakets = 0;
                            numCloseBrakets = 0;
                            isFunctionImplementation = false;
                            activeLineNum = curentToken.lineNum;
                        }
                        else
                        {
                            printf("Syntax error: Incorrect number of brackets in function signature.\nFile: %s, line: %d\n\n", file, curentToken.lineNum);
                            return 0;
                        }
                    }
                    //if the previous line was not a function just update the active line
                    else
                    {
                        activeLineNum = curentToken.lineNum;
                    }
                }
                if(isFunctionImplementation)
                {
                    //check for type requirements and mark as nessescary
                    if(curentToken.type == SourceTokenType::Uncatagorised)
                    {
                        if(previousToken.type == SourceTokenType::Symbol)
                        {
                            if(((previousToken.data.charTuple.c1 == '(') && (previousToken.data.charTuple.c2 == '\0')) || ((previousToken.data.charTuple.c1 == ',') && (previousToken.data.charTuple.c2 == '\0')))
                            {
                                tokenList->getBuff()[i].type = SourceTokenType::Type;
                            }
                        }
                        else if(previousToken.type == SourceTokenType::Keyword)
                        {
                            if(previousToken.data.keywordIndex == MDPL_KEYWORD_ENUM_MUT)
                            {
                                tokenList->getBuff()[i].type = SourceTokenType::Type;
                            }
                        }
                        
                    }
                    //count open and close brackets
                    else if(curentToken.type == SourceTokenType::Symbol)
                    {
                        if((curentToken.data.charTuple.c1 == '(') && (curentToken.data.charTuple.c2 == '\0'))
                        {
                            numOpenBrakets++;
                        }
                        else if((curentToken.data.charTuple.c1 == ')') && (curentToken.data.charTuple.c2 == '\0'))
                        {
                            numCloseBrakets++;
                        }
                        else if((curentToken.data.charTuple.c1 == '(') && (curentToken.data.charTuple.c2 == ')'))
                        {
                            numOpenBrakets++;
                            numCloseBrakets++;
                        }
                    }
                }
                //non functions lines
                else
                {
                    if(curentToken.type == SourceTokenType::FunctionImplementation)
                    {
                        isFunctionImplementation = true;
                    }
                    else if(previousToken.type == SourceTokenType::Keyword)
                    {
                        if(previousToken.data.keywordIndex == MDPL_KEYWORD_ENUM_LET)
                        {
                            tokenList->getBuff()[i].type = SourceTokenType::Type;
                        }
                        else if(previousToken.data.keywordIndex == MDPL_KEYWORD_ENUM_MUT)
                        {
                            tokenList->getBuff()[i].type = SourceTokenType::Type;
                        }
                        else if(previousToken.data.keywordIndex == MDPL_KEYWORD_ENUM_FOR)
                        {
                            tokenList->getBuff()[i].type = SourceTokenType::Type;
                        }
                    }
                }
            }
            return 0;
        }

        int getNumScopes(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens, size_t* numScopes)
        {
            *numScopes = 0;
            size_t numClosedScopes = 0;
            for(size_t i = 0; i < numTokens; i++)
            {
                SourceToken token = tokenList->getBuff()[i];

                if(token.type == SourceTokenType::Symbol)
                {
                    if((token.data.charTuple.c1 == '{') && (token.data.charTuple.c2 == '\0'))
                    {
                        *numScopes += 1;
                    }
                    else if((token.data.charTuple.c1 == '}') && (token.data.charTuple.c2 == '\0'))
                    {
                        numClosedScopes++;
                    }
                }
            }
            if(*numScopes != numClosedScopes)
            {
                printf("Syntax error: at %ld scope(s) were not closed.\n", *numScopes - numClosedScopes);
                return 1;
            }
            return 0;
        }
        int groupScopes(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens, const size_t& numScopes, common::RAIIBuffer<Scope>* scopeList)
        {
            scopeList->allocate(numScopes);

            size_t scopeListIndex = 0;
            for(size_t i = 0; i < numTokens; i++)
            {
                SourceToken token = tokenList->getBuff()[i];

                if(token.type == SourceTokenType::Symbol)
                {
                    if((token.data.charTuple.c1 == '{') && (token.data.charTuple.c2 == '\0'))
                    {
                        Scope scope;
                        scope.startTokenIndex = i;
                        MDPL_RETERR(internal::findCorespondingScopeClose(tokenList, numTokens, i, &scope.stopTokenIndex));
                        scope.startStatmentIndex = 0;
                        scope.stopStatmentIndex = 0;
                        if(scopeListIndex >= numScopes)
                        {
                            printf("Error: index exceded number of scopes.\n");
                            return 1;
                        }
                        scopeList->getBuff()[scopeListIndex] = scope;
                        scopeListIndex++;
                    }
                }
            }
            return 0;
        }
        int getNumStatments(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens, size_t* numStatments)
        {
            *numStatments = 0;
            size_t startTokenIndex = 0;
            size_t stopTokenIndex = 1;
            for(size_t i = 1; i < numTokens; i++)
            {
                SourceToken currentToken = tokenList->getBuff()[i-1];
                SourceToken previousToken = tokenList->getBuff()[i];

                if(currentToken.lineNum != previousToken.lineNum)
                {
                    stopTokenIndex = i;
                    if(startTokenIndex != stopTokenIndex)
                    {
                        if(startTokenIndex + 1 == stopTokenIndex)
                        {
                            SourceToken token = tokenList->getBuff()[startTokenIndex];
                            if(token.type == SourceTokenType::Symbol)
                            {
                                if((token.data.charTuple.c1 == '{' || token.data.charTuple.c1 == '}') and token.data.charTuple.c2 == '\0')
                                {
                                    //do nothing
                                }
                                else
                                {
                                    (*numStatments)++;
                                }
                            }
                            else
                            {
                                (*numStatments)++;
                            }
                        }
                        else
                        {
                            (*numStatments)++;
                        }
                    }
                    startTokenIndex = i;
                }
                else if(currentToken.type == SourceTokenType::Symbol)
                {
                    if(currentToken.data.charTuple.c1 == ';')
                    {
                        stopTokenIndex = i;
                        if(startTokenIndex != stopTokenIndex)
                        {
                            //because we have not checked if a statment terminated by a semi colons is a open or closing brace in this version you can't have { ; or } ;
                            (*numStatments)++;
                        }
                        startTokenIndex = i + 1;
                    }
                }
            }
            return 0;
        }

        int groupStatments(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens, const size_t& numStatments, common::RAIIBuffer<Statment>* statmentList)
        {
            statmentList->allocate(numStatments);

            size_t startTokenIndex = 0;
            size_t stopTokenIndex = 1;
            size_t statmentsIndex = 0;
            for(size_t i = 1; i < numTokens; i++)
            {
                SourceToken currentToken = tokenList->getBuff()[i-1];
                SourceToken nextToken = tokenList->getBuff()[i];

                //statment ends at EOL
                if(currentToken.lineNum != nextToken.lineNum)
                {
                    stopTokenIndex = i;
                    //if we have progressed at least one token
                    if(startTokenIndex != stopTokenIndex)
                    {
                        //if the statment is one token long
                        if(startTokenIndex + 1 == stopTokenIndex)
                        {
                            //create a statment so long as  the starting token is not a curly brace
                            SourceToken token = tokenList->getBuff()[startTokenIndex];
                            if(token.type == SourceTokenType::Symbol)
                            {
                                if((token.data.charTuple.c1 == '{' || token.data.charTuple.c1 == '}') and token.data.charTuple.c2 == '\0')
                                {
                                    //do nothing
                                }
                                else
                                {
                                    if(statmentsIndex >= numStatments)
                                    {
                                        printf("Error: statments index excedes the number of statments.\n");
                                        return 1;
                                    }
                                    Statment statment;
                                    statment.startTokenIndex = startTokenIndex;
                                    statment.stopTokenIndex = stopTokenIndex;
                                    statment.attachedScopeIndex = 0;
                                    statmentList->getBuff()[statmentsIndex] = statment;
                                    statmentsIndex++;
                                }
                            }
                            else
                            {
                                if(statmentsIndex >= numStatments)
                                {
                                    printf("Error: statments index excedes the number of statments.\n");
                                    return 1;
                                }
                                Statment statment;
                                statment.startTokenIndex = startTokenIndex;
                                statment.stopTokenIndex = stopTokenIndex;
                                statment.attachedScopeIndex = 0;
                                statmentList->getBuff()[statmentsIndex] = statment;
                                statmentsIndex++;
                            }
                        }
                        //if the statment is more than one token long
                        else
                        {
                            if(statmentsIndex >= numStatments)
                            {
                                printf("Error: statments index excedes the number of statments.\n");
                                return 1;
                            }
                            Statment statment;
                            statment.startTokenIndex = startTokenIndex;
                            statment.stopTokenIndex = stopTokenIndex;
                            statment.attachedScopeIndex = 0;
                            statmentList->getBuff()[statmentsIndex] = statment;
                            statmentsIndex++;
                        }
                    }
                    startTokenIndex = i;
                }
                //statments ends at semi colon
                else if(currentToken.type == SourceTokenType::Symbol)
                {
                    if(currentToken.data.charTuple.c1 == ';')
                    {
                        stopTokenIndex = i;
                        if(startTokenIndex != stopTokenIndex)
                        {
                            //because we have not checked if a statment terminated by a semi colons is a open or closing brace in this version you can't have { ; or } ;
                            if(statmentsIndex >= numStatments)
                            {
                                printf("Error: statments index excedes the number of statments.\n");
                                return 1;
                            }
                            Statment statment;
                            statment.startTokenIndex = startTokenIndex;
                            //as ther semi colon has no real meaning do not include it in the statment. Remembering that current token is index i-1
                            statment.stopTokenIndex = stopTokenIndex - 1;
                            statment.attachedScopeIndex = 0;
                            statmentList->getBuff()[statmentsIndex] = statment;
                            statmentsIndex++;
                        }
                        startTokenIndex = i;
                    }
                }
            }

            //statmentsIndex will always be incremented after writing to hense not numStatments-1
            if(statmentsIndex != numStatments)
            {
                printf("Error: Failed to group all statments.\n");
                return 1;
            }

            return 0;
        }
        int linkScopesToStatments(const size_t& numTokens, const size_t& numStatments, common::RAIIBuffer<Statment>* statmentList, const size_t& numScopes, common::RAIIBuffer<Scope>* scopeList)
        {
            size_t statmentIndex = 0;
            for(size_t i = 0; i < numScopes; i++)
            {
                Scope scope = scopeList->getBuff()[i];
                //printf(">> i=%d statmentIndex=%lu scope.startTokenIndex=%lu scope.stopTokenIndex=%lu statment.stopTokenIndex=%lu statment.stopTokenIndex=%lu\n", i, statmentIndex, scope.startTokenIndex, scope.stopTokenIndex, statmentList->getBuff()[statmentIndex].startTokenIndex, statmentList->getBuff()[statmentIndex].stopTokenIndex);

                //skip empty scopes
                if(scope.startTokenIndex + 1 == scope.stopTokenIndex)
                {
                    printf("continuing.\n");
                    continue;
                }

                //getting the starting statment of each scope is easy as they are in order
                while(scope.startTokenIndex + 1 > statmentList->getBuff()[statmentIndex].startTokenIndex)
                {
                    statmentIndex++;
                    if(statmentIndex >= numStatments)
                    {
                        printf("Error: exceded bounds of statment list when searching for start statment.\n");
                        return 0;
                    }
                }
                if(scope.startTokenIndex + 1 != statmentList->getBuff()[statmentIndex].startTokenIndex)
                {
                    printf("Error: could not match start of scope to a statment.\n");
                    return 0;
                }
                else
                {
                    scopeList->getBuff()[i].startStatmentIndex = statmentIndex;
                }

                //the file with likely end with multiuple closes of scope so to avoid edge cases we will mangually handle that here
                if(scope.stopTokenIndex == numTokens - 1)
                {
                    //adjust for up to but not including
                    scopeList->getBuff()[i].stopStatmentIndex = numStatments;
                }
                else
                {
                    //finding the stoping statment is a lot harder. I am sure there is an algorithm better then n^2 however this is just a prototype
                    size_t j = statmentIndex;
                    while(scope.stopTokenIndex > statmentList->getBuff()[j].stopTokenIndex)
                    {
                        j++;
                        if(j >= numStatments)
                        {
                            printf("Error: exceded bounds of statment list when searching for stop statment.\n");
                            return 0;
                        }
                    }
                    if(scope.stopTokenIndex != statmentList->getBuff()[j].stopTokenIndex)
                    {
                        //in the event of two or more scopes closing imediatly after each other we will not be able to find the statment we are looking for so we need to handle this case manually
                        //the only thing that isn't a statment are curly brackets so the next statment will always be the correct one as nothing can be inbetween as if it was it would be the next statment
                        //to make sure something disasterous hasn't happened we will check that the next statment is indeed after the scope even though it should be imposible not happen
                        if(scope.stopTokenIndex < statmentList->getBuff()[j].startTokenIndex)
                        {
                            //adjust for up to but not including
                            scopeList->getBuff()[i].stopStatmentIndex = j;
                        }
                        else
                        {
                            printf("STOP i=%lu statmentIndex=%lu scope.startTokenIndex=%lu scope.stopTokenIndex=%lu statment.startTokenIndex=%lu statment.stopTokenIndex=%lu\n", i, j, scope.startTokenIndex, scope.stopTokenIndex, statmentList->getBuff()[j].startTokenIndex, statmentList->getBuff()[j].stopTokenIndex);
                            printf("Error: could not match end of scope to a statment.\n");
                            return 0;
                        }
                    }
                    else
                    {
                        //adjust for up to but not including
                        scopeList->getBuff()[i].stopStatmentIndex = j + 1;
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
                    printf("\x1b[39m");
                }

                void underlineOn()
                {
                    printf("\x1b[4m");
                }
                void underlineOff()
                {
                    printf("\x1b[24m");
                }
            }

            int findCorespondingScopeClose(common::RAIIBuffer<SourceToken>* tokenList, const size_t& numTokens, const size_t startToken, size_t* endToken)
            {
                int numOpenScopes = 1;
                for(size_t i = startToken + 1; i < numTokens; i++)
                {
                    SourceToken token = tokenList->getBuff()[i];

                    if(token.type == SourceTokenType::Symbol)
                    {
                        if((token.data.charTuple.c1 == '{') && (token.data.charTuple.c2 == '\0'))
                        {
                            numOpenScopes++;
                        }
                        else if((token.data.charTuple.c1 == '}') && (token.data.charTuple.c2 == '\0'))
                        {
                            numOpenScopes--;
                            if(numOpenScopes <= 0)
                            {
                                *endToken = i;
                                return 0;
                            }
                        }
                    }
                }

                printf("Error: failed to find closing scope bracket.\n");
                return 1;
            }
        }
    }
}