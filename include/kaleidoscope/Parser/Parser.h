//
// Created by entropy on 1/5/23.
//

#ifndef KALEIDOSCOPE_PARSER_H
#define KALEIDOSCOPE_PARSER_H

#include "kaleidoscope/Parser/AST.h"
#include "kaleidoscope/Lexer/Lexer.h"

namespace kaleidoscope{
    class Parser {
        // States
        Lexer lexer;
        int CurTok;
        int getNextToken();

        // Error Handling
        std::unique_ptr<ExprAST> LogError(const char *Str);
        std::unique_ptr<PrototypeAST> LogErrorP(const char *Str);

        std::unique_ptr<ExprAST> ParseNumberExpr();
        std::unique_ptr<ExprAST> ParseParenExpr();
        std::unique_ptr<ExprAST> ParseIdentifierExpr();
        std::unique_ptr<ExprAST> ParsePrimary();

    public:
        explicit Parser(Lexer &lexer) : lexer(lexer) {}
    };

    std::unique_ptr<ExprAST> ParseNumberExpr();
}

#endif //KALEIDOSCOPE_PARSER_H
