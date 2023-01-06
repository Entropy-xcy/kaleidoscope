//
// Created by entropy on 1/5/23.
//
#include <iostream>
#include "kaleidoscope/Lexer/Lexer.h"
#include "kaleidoscope/Parser/Parser.h"

int main() {
    kaleidoscope::Lexer lexer;
    auto parser = kaleidoscope::Parser(lexer);

    parser.MainLoop();
}
