#include "symbol_table.h"

MDPL_ERROR_Error* stringToSymbol(SymbolTable* table, const char* const str, Symbol* symbol)
{
    MDPL_RETERR(reallocateSymbolTable(table));
    uint32_t index;
    MDPL_RETERR(hashToTableIndex(table, str, &index));
    //the stirng is new to the table so add it
    if(table->stringToSymbolHashTable[index].string == nullptr)
    {
        table->stringToSymbolHashTable[index].string = str;
        table->stringToSymbolHashTable[index].symbol = table->nextAvailableSymbol;
        table->symbolToStringLUT[table->nextAvailableSymbol - 1] = str;
        *symbol = table->nextAvailableSymbol;
        table->nextAvailableSymbol++;
        table->size++;
    }
    //the string already exists in the table so just get the symbol
    else
    {
        *symbol = table->stringToSymbolHashTable[index].symbol;
    }
    return nullptr;
}
MDPL_ERROR_Error* symbolToString(SymbolTable* table, const Symbol symbol, const char** str)
{
    if(symbol == 0)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "symbol is zero (invalid / undefined)");
    }
    //as 0 is not a valid symbol to symplify things the first list index is symbol 1 not 0 hense symbol - 1
    if(symbol - 1 >= table->size)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "symbol is not valid");
    }
    *str = table->symbolToStringLUT[symbol - 1];
    return nullptr;
}
MDPL_ERROR_Error* convertPredefinedSymbolToRealSymbol(SymbolTable* table, const uint32_t predefinedSymbol, Symbol* realSymbol)
{
    //invalid is not a predefined but is value zero so adjust by 1
    if(predefinedSymbol >= MDPL_COMPILER_SYMBOL_HASH_NUM_PREDEFINED + 1)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_OUT_OF_RANGE, "predefined symbol is too large");
    }
    else if(predefinedSymbol == MDPL_COMPILER_SYMBOL_invalid)
    {
        *realSymbol = 0;
        return nullptr;
    }
    else
    {
        *realSymbol = table->predefinedSymboltoRealSymbol[predefinedSymbol - 1];
        return nullptr;
    };
    return nullptr;
}

MDPL_ERROR_Error* hashToTableIndex(SymbolTable* table, const char* const str, uint32_t* index)
{
    //xor hash of the string mod 8 bytes
    uint32_t hash = 0;
    uint32_t rotator = 0;
    for(int i = 0; i < strlen(str); i++)
    {
        hash ^= str[i] << rotator;
        rotator += 8;
        if(rotator == 32)
        {
            rotator = 0;
        }
    }
    hash %= table->capacity;

    for(uint32_t i = 0; i < table->capacity; i++)
    {
        //string is new to the table
        if(table->stringToSymbolHashTable[hash].string == nullptr)
        {
            *index = hash;
            return nullptr;
        }
        //string is already in the table
        else if(strcmp(str, table->stringToSymbolHashTable[hash].string) == 0)
        {
            *index = hash;
            return nullptr;
        }
        else
        {
            hash++;
            if(hash == table->capacity)
            {
                hash = 0;
            }
        }
    }
    return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_INVALID_ARGUMENT, "failed to convert str to symbol"); 
}
MDPL_ERROR_Error* computePredefinedSymbols(SymbolTable* table)
{
    Symbol symbol;
    MDPL_RETERR(stringToSymbol(table, "namespace", &symbol));
    table->predefinedSymboltoRealSymbol[0] = symbol;
    MDPL_RETERR(stringToSymbol(table, "public", &symbol));
    table->predefinedSymboltoRealSymbol[1] = symbol;
    MDPL_RETERR(stringToSymbol(table, "protected", &symbol));
    table->predefinedSymboltoRealSymbol[2] = symbol;
    MDPL_RETERR(stringToSymbol(table, "code", &symbol));
    table->predefinedSymboltoRealSymbol[3] = symbol;
    MDPL_RETERR(stringToSymbol(table, "fn", &symbol));
    table->predefinedSymboltoRealSymbol[4] = symbol;
    MDPL_RETERR(stringToSymbol(table, "sub", &symbol));
    table->predefinedSymboltoRealSymbol[5] = symbol;
    MDPL_RETERR(stringToSymbol(table, "ret", &symbol));
    table->predefinedSymboltoRealSymbol[6] = symbol;
    MDPL_RETERR(stringToSymbol(table, "ref", &symbol));
    table->predefinedSymboltoRealSymbol[7] = symbol;
    MDPL_RETERR(stringToSymbol(table, "generic", &symbol));
    table->predefinedSymboltoRealSymbol[8] = symbol;
    MDPL_RETERR(stringToSymbol(table, "prop", &symbol));
    table->predefinedSymboltoRealSymbol[9] = symbol;
    MDPL_RETERR(stringToSymbol(table, "catch", &symbol));
    table->predefinedSymboltoRealSymbol[10] = symbol;
    MDPL_RETERR(stringToSymbol(table, "if", &symbol));
    table->predefinedSymboltoRealSymbol[11] = symbol;
    MDPL_RETERR(stringToSymbol(table, "elif", &symbol));
    table->predefinedSymboltoRealSymbol[12] = symbol;
    MDPL_RETERR(stringToSymbol(table, "else", &symbol));
    table->predefinedSymboltoRealSymbol[13] = symbol;
    MDPL_RETERR(stringToSymbol(table, "for", &symbol));
    table->predefinedSymboltoRealSymbol[14] = symbol;
    MDPL_RETERR(stringToSymbol(table, "while", &symbol));
    table->predefinedSymboltoRealSymbol[15] = symbol;
    MDPL_RETERR(stringToSymbol(table, "from", &symbol));
    table->predefinedSymboltoRealSymbol[16] = symbol;
    MDPL_RETERR(stringToSymbol(table, "to", &symbol));
    table->predefinedSymboltoRealSymbol[17] = symbol;
    MDPL_RETERR(stringToSymbol(table, "step", &symbol));
    table->predefinedSymboltoRealSymbol[18] = symbol;
    MDPL_RETERR(stringToSymbol(table, "continue", &symbol));
    table->predefinedSymboltoRealSymbol[19] = symbol;
    MDPL_RETERR(stringToSymbol(table, "break", &symbol));
    table->predefinedSymboltoRealSymbol[20] = symbol;
    MDPL_RETERR(stringToSymbol(table, "switch", &symbol));
    table->predefinedSymboltoRealSymbol[21] = symbol;
    MDPL_RETERR(stringToSymbol(table, "case", &symbol));
    table->predefinedSymboltoRealSymbol[22] = symbol;
    MDPL_RETERR(stringToSymbol(table, "default", &symbol));
    table->predefinedSymboltoRealSymbol[23] = symbol;
    MDPL_RETERR(stringToSymbol(table, "let", &symbol));
    table->predefinedSymboltoRealSymbol[24] = symbol;
    MDPL_RETERR(stringToSymbol(table, "mut", &symbol));
    table->predefinedSymboltoRealSymbol[25] = symbol;
    MDPL_RETERR(stringToSymbol(table, "auto", &symbol));
    table->predefinedSymboltoRealSymbol[26] = symbol;
    MDPL_RETERR(stringToSymbol(table, "struct", &symbol));
    table->predefinedSymboltoRealSymbol[27] = symbol;
    MDPL_RETERR(stringToSymbol(table, "enum", &symbol));
    table->predefinedSymboltoRealSymbol[28] = symbol;
    MDPL_RETERR(stringToSymbol(table, "bitfield", &symbol));
    table->predefinedSymboltoRealSymbol[29] = symbol;
    MDPL_RETERR(stringToSymbol(table, "using", &symbol));
    table->predefinedSymboltoRealSymbol[30] = symbol;
    MDPL_RETERR(stringToSymbol(table, "and", &symbol));
    table->predefinedSymboltoRealSymbol[31] = symbol;
    MDPL_RETERR(stringToSymbol(table, "or", &symbol));
    table->predefinedSymboltoRealSymbol[32] = symbol;
    MDPL_RETERR(stringToSymbol(table, "not", &symbol));
    table->predefinedSymboltoRealSymbol[33] = symbol;
    MDPL_RETERR(stringToSymbol(table, "true", &symbol));
    table->predefinedSymboltoRealSymbol[34] = symbol;
    MDPL_RETERR(stringToSymbol(table, "false", &symbol));
    table->predefinedSymboltoRealSymbol[35] = symbol;
    MDPL_RETERR(stringToSymbol(table, "sizeof", &symbol));
    table->predefinedSymboltoRealSymbol[36] = symbol;
    MDPL_RETERR(stringToSymbol(table, "include", &symbol));
    table->predefinedSymboltoRealSymbol[37] = symbol;
    MDPL_RETERR(stringToSymbol(table, "return", &symbol));
    table->predefinedSymboltoRealSymbol[38] = symbol;
    MDPL_RETERR(stringToSymbol(table, "static", &symbol));
    table->predefinedSymboltoRealSymbol[39] = symbol;
    MDPL_RETERR(stringToSymbol(table, "typedef", &symbol));
    table->predefinedSymboltoRealSymbol[40] = symbol;
    MDPL_RETERR(stringToSymbol(table, "const", &symbol));
    table->predefinedSymboltoRealSymbol[41] = symbol;
    MDPL_RETERR(stringToSymbol(table, "endif", &symbol));
    table->predefinedSymboltoRealSymbol[42] = symbol;
    MDPL_RETERR(stringToSymbol(table, "define", &symbol));
    table->predefinedSymboltoRealSymbol[43] = symbol;
    MDPL_RETERR(stringToSymbol(table, "defined", &symbol));
    table->predefinedSymboltoRealSymbol[44] = symbol;
    MDPL_RETERR(stringToSymbol(table, "undef", &symbol));
    table->predefinedSymboltoRealSymbol[45] = symbol;
    MDPL_RETERR(stringToSymbol(table, "ifdef", &symbol));
    table->predefinedSymboltoRealSymbol[46] = symbol;
    MDPL_RETERR(stringToSymbol(table, "ifndef", &symbol));
    table->predefinedSymboltoRealSymbol[47] = symbol;
    MDPL_RETERR(stringToSymbol(table, "undefined", &symbol));
    table->predefinedSymboltoRealSymbol[48] = symbol;
    MDPL_RETERR(stringToSymbol(table, "byte", &symbol));
    table->predefinedSymboltoRealSymbol[49] = symbol;
    MDPL_RETERR(stringToSymbol(table, "int8", &symbol));
    table->predefinedSymboltoRealSymbol[50] = symbol;
    MDPL_RETERR(stringToSymbol(table, "uint8", &symbol));
    table->predefinedSymboltoRealSymbol[51] = symbol;
    MDPL_RETERR(stringToSymbol(table, "int16", &symbol));
    table->predefinedSymboltoRealSymbol[52] = symbol;
    MDPL_RETERR(stringToSymbol(table, "uint16", &symbol));
    table->predefinedSymboltoRealSymbol[53] = symbol;
    MDPL_RETERR(stringToSymbol(table, "int32", &symbol));
    table->predefinedSymboltoRealSymbol[54] = symbol;
    MDPL_RETERR(stringToSymbol(table, "uint32", &symbol));
    table->predefinedSymboltoRealSymbol[55] = symbol;
    MDPL_RETERR(stringToSymbol(table, "int64", &symbol));
    table->predefinedSymboltoRealSymbol[56] = symbol;
    MDPL_RETERR(stringToSymbol(table, "uint64", &symbol));
    table->predefinedSymboltoRealSymbol[57] = symbol;
    MDPL_RETERR(stringToSymbol(table, "float32", &symbol));
    table->predefinedSymboltoRealSymbol[58] = symbol;
    MDPL_RETERR(stringToSymbol(table, "float64", &symbol));
    table->predefinedSymboltoRealSymbol[59] = symbol;
    MDPL_RETERR(stringToSymbol(table, "string", &symbol));
    table->predefinedSymboltoRealSymbol[60] = symbol;
    MDPL_RETERR(stringToSymbol(table, "bool", &symbol));
    table->predefinedSymboltoRealSymbol[61] = symbol;
    MDPL_RETERR(stringToSymbol(table, "int", &symbol));
    table->predefinedSymboltoRealSymbol[62] = symbol;
    MDPL_RETERR(stringToSymbol(table, "float", &symbol));
    table->predefinedSymboltoRealSymbol[63] = symbol;
    MDPL_RETERR(stringToSymbol(table, "double", &symbol));
    table->predefinedSymboltoRealSymbol[64] = symbol;
    MDPL_RETERR(stringToSymbol(table, "int8_t", &symbol));
    table->predefinedSymboltoRealSymbol[65] = symbol;
    MDPL_RETERR(stringToSymbol(table, "uint8_t", &symbol));
    table->predefinedSymboltoRealSymbol[66] = symbol;
    MDPL_RETERR(stringToSymbol(table, "int16_t", &symbol));
    table->predefinedSymboltoRealSymbol[67] = symbol;
    MDPL_RETERR(stringToSymbol(table, "uint16_t", &symbol));
    table->predefinedSymboltoRealSymbol[68] = symbol;
    MDPL_RETERR(stringToSymbol(table, "int32_t", &symbol));
    table->predefinedSymboltoRealSymbol[69] = symbol;
    MDPL_RETERR(stringToSymbol(table, "uint32_t", &symbol));
    table->predefinedSymboltoRealSymbol[70] = symbol;
    MDPL_RETERR(stringToSymbol(table, "int64_t", &symbol));
    table->predefinedSymboltoRealSymbol[71] = symbol;
    MDPL_RETERR(stringToSymbol(table, "uint64_t", &symbol));
    table->predefinedSymboltoRealSymbol[72] = symbol;
    MDPL_RETERR(stringToSymbol(table, "size_t", &symbol));
    table->predefinedSymboltoRealSymbol[73] = symbol;
    return nullptr;
}
MDPL_ERROR_Error* initialiseSymbolTable(SymbolTable* table)
{
    //zero is not a valid symbol so start at 1
    table->nextAvailableSymbol = 1;
    table->stringToSymbolHashTable = (StringSymbolKVPair*)malloc(sizeof(StringSymbolKVPair) * MDPL_COMPILER_SYMBOL_HASH_INITIAL_CAPACITY);
    if(table->stringToSymbolHashTable == nullptr)
    {
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_ALLOCATION, "Failed to allocate table");
    }
    for(uint32_t i = 0; i < MDPL_COMPILER_SYMBOL_HASH_INITIAL_CAPACITY; i++)
    {
        table->stringToSymbolHashTable[i].string = nullptr;
        table->stringToSymbolHashTable[i].symbol = 0;
    }

    table->symbolToStringLUT = (const char**)malloc(sizeof(const char*) * MDPL_COMPILER_SYMBOL_HASH_INITIAL_CAPACITY);
    if(table->symbolToStringLUT == nullptr)
    {
        free(table->stringToSymbolHashTable);
        return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_ALLOCATION, "Failed to allocate table");
    }

    table->size = 0;
    table->capacity = MDPL_COMPILER_SYMBOL_HASH_INITIAL_CAPACITY;
    
    MDPL_RETERR(computePredefinedSymbols(table));
    
    return nullptr;
}
MDPL_ERROR_Error* reallocateSymbolTable(SymbolTable* table)
{
    if(table->size > ((table->capacity * 3) / 4))
    {
        StringSymbolKVPair* oldStringToSymbolHashTable = table->stringToSymbolHashTable;
        uint32_t oldCapacity = table->capacity;

        table->capacity *= 2;
        
        //hash table
        table->stringToSymbolHashTable = (StringSymbolKVPair*)malloc(sizeof(StringSymbolKVPair) * table->capacity);
        if(table->stringToSymbolHashTable == nullptr)
        {
            free(table->symbolToStringLUT);
            free(oldStringToSymbolHashTable);
            return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_ALLOCATION, "Failed to reallocate table");
        }
        for(uint32_t i = 0; i < table->capacity; i++)
        {
            table->stringToSymbolHashTable[i].string = nullptr;
            table->stringToSymbolHashTable[i].symbol = 0;
        }
        for(uint32_t i = 0; i < oldCapacity; i++)
        {
            if(oldStringToSymbolHashTable[i].string != nullptr)
            {
                uint32_t index;
                MDPL_RETERR(hashToTableIndex(table, oldStringToSymbolHashTable[i].string, &index));
                if(table->stringToSymbolHashTable[index].string == nullptr)
                {
                    table->stringToSymbolHashTable[index].string = oldStringToSymbolHashTable[i].string;
                    table->stringToSymbolHashTable[index].symbol = oldStringToSymbolHashTable[i].symbol;
                }
            }
        }
        free(oldStringToSymbolHashTable);

        //lookup table
        table->symbolToStringLUT = (const char**)realloc(table->symbolToStringLUT, sizeof(const char*) * table->capacity);
        if(table->symbolToStringLUT == nullptr)
        {
            free(table->stringToSymbolHashTable);
            return MDPL_ERROR_TYPE_MSG(MDPL_ERROR_TYPE_ALLOCATION, "Failed to reallocate table");
        }
    };

    return nullptr;
}
MDPL_ERROR_Error* destroySymbolTable(SymbolTable* table)
{
    free(table->stringToSymbolHashTable);
    free(table->symbolToStringLUT);
    return nullptr;
}