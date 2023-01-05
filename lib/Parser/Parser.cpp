//
// Created by entropy on 1/5/23.
//

#include "kaleidoscope/Parser/Parser.h"

namespace kaleidoscope{
    int Parser::getNextToken() {
        return CurTok = lexer.gettok();
    }

    std::unique_ptr<ExprAST> Parser::LogError(const char *Str) {
        fprintf(stderr, "LogError: %s\n", Str);
        return nullptr;
    }

    std::unique_ptr<PrototypeAST> Parser::LogErrorP(const char *Str) {
        LogError(Str);
        return nullptr;
    }

    std::unique_ptr<ExprAST> Parser::ParseNumberExpr() {
        // TODO
        return nullptr;
    }

    std::unique_ptr<ExprAST> Parser::ParseParenExpr() {
        // TODO
        return nullptr;
    }

    std::unique_ptr<ExprAST> Parser::ParseIdentifierExpr() {
        // TODO
        return nullptr;
    }

    std::unique_ptr<ExprAST> Parser::ParsePrimary() {
        // TODO
        return nullptr;
    }
}
