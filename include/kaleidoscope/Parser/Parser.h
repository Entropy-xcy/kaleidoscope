//
// Created by entropy on 1/5/23.
//

#ifndef KALEIDOSCOPE_PARSER_H
#define KALEIDOSCOPE_PARSER_H

#include <map>
#include "kaleidoscope/Parser/AST.h"
#include "kaleidoscope/Lexer/Lexer.h"

namespace kaleidoscope{
    class Parser {
        // States
        Lexer lexer;
        int CurTok;
        int getNextToken();

        std::map<char, int> BinopPrecedence;
        int GetTokPrecedence();

        // Error Handling
        std::unique_ptr<ExprAST> LogError(const char *Str);
        std::unique_ptr<PrototypeAST> LogErrorP(const char *Str);

        std::unique_ptr<ExprAST> ParseNumberExpr();
        std::unique_ptr<ExprAST> ParseParenExpr();
        std::unique_ptr<ExprAST> ParseIdentifierExpr();
        std::unique_ptr<ExprAST> ParsePrimary();
        std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS);

        std::unique_ptr<ExprAST> ParseExpression();
        std::unique_ptr<PrototypeAST> ParsePrototype();
        std::unique_ptr<FunctionAST> ParseDefinition();
        std::unique_ptr<FunctionAST> ParseTopLevelExpr();
        std::unique_ptr<PrototypeAST> ParseExtern();

    public:
        explicit Parser(Lexer &lexer): lexer(lexer) {
            BinopPrecedence['<'] = 10;
            BinopPrecedence['+'] = 20;
            BinopPrecedence['-'] = 20;
            BinopPrecedence['*'] = 40;
        }

        void HandleDefinition();
        void HandleExtern();
        void HandleTopLevelExpression();
        void MainLoop();

    };

    std::unique_ptr<ExprAST> ParseNumberExpr();
}

#endif //KALEIDOSCOPE_PARSER_H
