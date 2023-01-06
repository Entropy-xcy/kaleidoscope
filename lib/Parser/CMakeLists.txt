add_tinylang_library(kaleidoscopeParser
        Parser.cpp
        )
target_link_libraries(kaleidoscopeParser
        PRIVATE
        kaleidoscopeLexer
        )