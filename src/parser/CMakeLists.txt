include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(BISON 3.0)
find_package(FLEX 2.6)

BISON_TARGET(
                cQasmParser 
                grammar.y ${CMAKE_CURRENT_BINARY_DIR}/grammar.tab.c
                COMPILE_FLAGS "-Wall -d -t -g -r all"
            )
FLEX_TARGET(cQasmScanner lex.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)
ADD_FLEX_BISON_DEPENDENCY(cQasmScanner cQasmParser)

set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/grammar.tab.c PROPERTIES LANGUAGE CXX )
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c PROPERTIES LANGUAGE CXX )
ADD_LIBRARY( cqasmlexgram STATIC
             ${CMAKE_CURRENT_BINARY_DIR}/grammar.tab.c
             ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)

SET_PROPERTY(TARGET cqasmlexgram PROPERTY CXX_STANDARD 11)
