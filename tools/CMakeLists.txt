create_subdirectory_options(TINYLANG TOOL)

add_tinylang_executable(kaleidoscope main.cpp)
target_link_libraries(kaleidoscope
        PRIVATE
        kaleidoscopeLexer
        )
target_link_libraries(kaleidoscope
        PRIVATE
        kaleidoscopeCodeGen
        )
target_link_libraries(kaleidoscope
        PRIVATE
        kaleidoscopeParser
        )