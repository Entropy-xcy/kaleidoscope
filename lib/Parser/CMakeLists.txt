#add_tinylang_library(kaleidoscopeAST
#        AST.cpp
#        )

add_tinylang_library(kaleidoscopeParser Parser.cpp AST.cpp)

target_link_libraries(kaleidoscopeParser
        PRIVATE
        kaleidoscopeLexer
        )




