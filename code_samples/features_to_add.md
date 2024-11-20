## Operators
- v0.1 will interpret + = as +=. v0.2 should not do this.

## Resources
- everything

## Generics
- specifying heirachical relationships
- specifying properties

## FFI
- calling c/c++
- exposing a function to c/c++
- exposing a type to c/c++

## Tokenisint and Passing for v0.2
- detect double symbols during tokenisation and then zero both symbols so that when iterating to the next one nothing is done
- instead of linking tokens add an integer feild to specify the indentation which stores nesting