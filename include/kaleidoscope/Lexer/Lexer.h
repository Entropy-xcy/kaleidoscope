//
// Created by entropy on 1/5/23.
//

#ifndef KALEIDOSCOPE_LEXER_H
#define KALEIDOSCOPE_LEXER_H

namespace kaleidoscope {
    enum Token {
        tok_eof = -1,
        tok_def = -2,
        tok_extern = -3,

        tok_identifier = -4,
        tok_number = -5,
    };
    class Lexer{
    public:
        std::string IdentifierStr;
        double NumVal;
        int gettok();
    };
}

#endif //KALEIDOSCOPE_LEXER_H
