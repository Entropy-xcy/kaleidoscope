//
// Created by entropy on 1/5/23.
//
#include <iostream>
#include "kaleidoscope/Lexer/Lexer.h"
#include "kaleidoscope/Parser/Parser.h"
#include "kaleidoscope/CodeGen/CodeGen.h"

int main() {
    kaleidoscope::Lexer lexer;
    auto Parser = kaleidoscope::Parser(lexer);

    auto codegen = kaleidoscope::CodeGen(Parser);
    codegen.MainLoop();

}
