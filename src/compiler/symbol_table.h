#ifndef HELPER_HEADER_GUARD
#define HELPER_HEADER_GUARD

#include "../pch.h"
#include "../common/mdpl_common.h"
#include "../runtime_lib/mdpl_error.h"
#include "../../vendor/libdivide/libdivide.h"

//public
//keywords:
//invalid is not a source code keyword but represents a symbol that is not valid 
#define MDPL_COMPILER_SYMBOL_invalid    0
#define MDPL_COMPILER_SYMBOL_namespace  1
#define MDPL_COMPILER_SYMBOL_public     2
#define MDPL_COMPILER_SYMBOL_protected  3
#define MDPL_COMPILER_SYMBOL_code       4
#define MDPL_COMPILER_SYMBOL_fn         5
#define MDPL_COMPILER_SYMBOL_sub        6
#define MDPL_COMPILER_SYMBOL_ret        7
#define MDPL_COMPILER_SYMBOL_ref        8
#define MDPL_COMPILER_SYMBOL_generic    9
#define MDPL_COMPILER_SYMBOL_prop       10
#define MDPL_COMPILER_SYMBOL_catch      11
#define MDPL_COMPILER_SYMBOL_if         12
#define MDPL_COMPILER_SYMBOL_elif       13
#define MDPL_COMPILER_SYMBOL_else       14
#define MDPL_COMPILER_SYMBOL_for        15
#define MDPL_COMPILER_SYMBOL_while      16
#define MDPL_COMPILER_SYMBOL_from       17
#define MDPL_COMPILER_SYMBOL_to         18
#define MDPL_COMPILER_SYMBOL_step       19
#define MDPL_COMPILER_SYMBOL_continue   20
#define MDPL_COMPILER_SYMBOL_break      21
#define MDPL_COMPILER_SYMBOL_switch     22
#define MDPL_COMPILER_SYMBOL_case       23
#define MDPL_COMPILER_SYMBOL_default    24
#define MDPL_COMPILER_SYMBOL_let        25
#define MDPL_COMPILER_SYMBOL_mut        26
#define MDPL_COMPILER_SYMBOL_auto       27
#define MDPL_COMPILER_SYMBOL_struct     28
#define MDPL_COMPILER_SYMBOL_enum       29
#define MDPL_COMPILER_SYMBOL_bitfield   30
#define MDPL_COMPILER_SYMBOL_using      31
#define MDPL_COMPILER_SYMBOL_and        32
#define MDPL_COMPILER_SYMBOL_or         33
#define MDPL_COMPILER_SYMBOL_not        34
#define MDPL_COMPILER_SYMBOL_true       35
#define MDPL_COMPILER_SYMBOL_false      36
#define MDPL_COMPILER_SYMBOL_sizeof     37
#define MDPL_COMPILER_SYMBOL_include    38
//c keywords not used in mdpl
#define MDPL_COMPILER_SYMBOL_return     39
#define MDPL_COMPILER_SYMBOL_static     40
#define MDPL_COMPILER_SYMBOL_typedef    41
#define MDPL_COMPILER_SYMBOL_const      42
#define MDPL_COMPILER_SYMBOL_endif      43
#define MDPL_COMPILER_SYMBOL_define     44
#define MDPL_COMPILER_SYMBOL_defined    45
#define MDPL_COMPILER_SYMBOL_undef      46
#define MDPL_COMPILER_SYMBOL_ifdef      47
#define MDPL_COMPILER_SYMBOL_ifndef     48
//types:
#define MDPL_COMPILER_SYMBOL_undefined  49
#define MDPL_COMPILER_SYMBOL_byte       50
#define MDPL_COMPILER_SYMBOL_int8       51
#define MDPL_COMPILER_SYMBOL_uint8      52
#define MDPL_COMPILER_SYMBOL_int16      53
#define MDPL_COMPILER_SYMBOL_uint16     54
#define MDPL_COMPILER_SYMBOL_int32      55
#define MDPL_COMPILER_SYMBOL_uint32     56
#define MDPL_COMPILER_SYMBOL_int64      57
#define MDPL_COMPILER_SYMBOL_uint64     58
#define MDPL_COMPILER_SYMBOL_float32    59
#define MDPL_COMPILER_SYMBOL_float64    60
#define MDPL_COMPILER_SYMBOL_string     61
#define MDPL_COMPILER_SYMBOL_bool       62
//c types not used in mdpl
#define MDPL_COMPILER_SYMBOL_int        63
#define MDPL_COMPILER_SYMBOL_float      64
#define MDPL_COMPILER_SYMBOL_double     65
#define MDPL_COMPILER_SYMBOL_int8_t     66
#define MDPL_COMPILER_SYMBOL_uint8_t    67
#define MDPL_COMPILER_SYMBOL_int16_t    68
#define MDPL_COMPILER_SYMBOL_uint16_t   69
#define MDPL_COMPILER_SYMBOL_int32_t    70
#define MDPL_COMPILER_SYMBOL_uint32_t   71
#define MDPL_COMPILER_SYMBOL_int64_t    72
#define MDPL_COMPILER_SYMBOL_uint64_t   73
#define MDPL_COMPILER_SYMBOL_size_t     74


//private
#define MDPL_COMPILER_SYMBOL_HASH_INITIAL_CAPACITY 256
#define MDPL_COMPILER_SYMBOL_HASH_INITIAL_SYMBOL 75
#define MDPL_COMPILER_SYMBOL_HASH_NUM_PREDEFINED 74

//public
typedef uint32_t Symbol;

//protected
typedef struct StringSymbolKVPair
{
    const char* string;
    Symbol symbol;
} StringSymbolKVPair;

//public
typedef struct SymbolTable
{
    Symbol nextAvailableSymbol;
    StringSymbolKVPair* stringToSymbolHashTable;
    const char** symbolToStringLUT;
    uint32_t size;
    uint32_t capacity;
    Symbol predefinedSymboltoRealSymbol[MDPL_COMPILER_SYMBOL_HASH_NUM_PREDEFINED];
} SymbolTable;

//public
MDPL_ERROR_Error* stringToSymbol(SymbolTable* table, const char* const str, Symbol* symbol);
MDPL_ERROR_Error* symbolToString(SymbolTable* table, const Symbol symbol, const char** str);
MDPL_ERROR_Error* convertPredefinedSymbolToRealSymbol(SymbolTable* table, const uint32_t predefinedSymbol, Symbol* realSymbol);

//protected
MDPL_ERROR_Error* hashToTableIndex(SymbolTable* table, const char* const str, uint32_t* index);
MDPL_ERROR_Error* computePredefinedSymbols(SymbolTable* table);
MDPL_ERROR_Error* initialiseSymbolTable(SymbolTable* table);
MDPL_ERROR_Error* reallocateSymbolTable(SymbolTable* table);
MDPL_ERROR_Error* destroySymbolTable(SymbolTable* table);

#endif //HELPER_HEADER_GUARD