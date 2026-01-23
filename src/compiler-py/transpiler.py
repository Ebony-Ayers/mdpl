from passer import *
import enums

def convertType(token):
    if (token.str in enums.defaultTypes) and ("int" in token.str):
        return token.str + "_t"
    elif token.str == "float32":
        return "float"
    elif token.str == "float64":
        return "double"
    elif token.str == "string":
        return "MDPL_STDLIB_STRING_StringRef"
    elif token.str.startswith("ShortList<"):
        return "MDPL_STDLIB_SHORT_LIST_" + convertType(token.str[10:-1])
    else:
        return token.str

def transpileHeaders(fileName, tokens, statments, scopes, namespace, targetScopeIndex, fileSuffix):
    #transpile the headers
    fileNameWithOutPathAndExtension = fileName[fileName.rfind("/")+1:fileName.rfind(".")]
    publicHeaderFileName = fileNameWithOutPathAndExtension + "_" + fileSuffix
    with open(publicHeaderFileName + ".h", "w") as f:
        f.write("//generated from: " + fileNameWithOutPathAndExtension + ".mdpl\n\n")
        f.write("#ifndef " + publicHeaderFileName.upper() + "_HEADER_GUARD\n")
        f.write("#define " + publicHeaderFileName.upper() + "_HEADER_GUARD\n")
        f.write("\n")
        f.write("#include<stdint.h>\n")
        f.write("#include<stdbool.h>\n")
        f.write("#include \"mdpl_error.h\"\n")
        f.write("#include \"mdpl_string.h\"\n")
        f.write("\n")

        for i in range(scopes[targetScopeIndex].startStatmentIndex + 1, scopes[targetScopeIndex].endStatmentIndex):
            statment = statments[i]

            #function
            if tokens[statment.startTokenIndex].str in ("fn", "sub"):
                #function return type
                f.write("MDPL_ERROR_Error* ")
                #function name
                f.write(namespace + "_" + tokens[statment.startTokenIndex+1].str)
                #opening bracket
                f.write("(")
                #arguments
                isFirstArgument = True
                ingoreNextConst = False
                isOutputs = False
                for j in range(statment.startTokenIndex + 3, statment.endTokenIndex + 1):
                    token = tokens[j]
                    if token.str == "mut":
                        ingoreNextConst = True
                    elif token.category == enums.categories["Type"]:
                        if isFirstArgument:
                            isFirstArgument = False
                        else:
                            f.write(", ")
                        if ingoreNextConst:
                            ingoreNextConst = False
                        else:
                            f.write("const ")
                        f.write(convertType(token))
                        if isOutputs:
                            f.write("*")    
                    elif token.str == "ref":
                        f.write("*")
                    elif token.category == enums.categories["Variable"]:
                        f.write(" " + token.str)
                    elif token.str == "->":
                        isOutputs = True
                #closing bracket
                f.write(");\n")
            
            elif tokens[statment.startTokenIndex].category == enums.categories["Type"]:
                f.write("const ")
                f.write(convertType(tokens[statment.startTokenIndex]) + " ")
                f.write(tokens[statment.startTokenIndex + 1].str)
                f.write(" = ")
                for j in range(statment.startTokenIndex + 3, statment.endTokenIndex + 1):
                    f.write(tokens[j].str)
                f.write(";\n")

        f.write("\n")
        f.write("#endif //" + publicHeaderFileName.upper() + "_HEADER_GUARD\n")

def transpile(fileName, tokens, statments, scopes):
    #headers:
    
    namespace = tokens[1].str
    #find where the data for the headers is
    publicTokenIndex = None
    protectedTokenIndex = None
    for i,token in enumerate(tokens):
        if token.str == "public":
            publicTokenIndex = i
        elif token.str == "protected":
            protectedTokenIndex = i
            break
    publicScopeIndex = None
    protectedScopeIndex = None
    for i,scope in enumerate(scopes):
        if scope.startTokenIndex == publicTokenIndex + 1:
            publicScopeIndex = i
        elif scope.startTokenIndex == protectedTokenIndex + 1:
            protectedScopeIndex = i

    transpileHeaders(fileName, tokens, statments, scopes, namespace, publicScopeIndex, "public")
    transpileHeaders(fileName, tokens, statments, scopes, namespace, protectedScopeIndex, "protected")

    #c: