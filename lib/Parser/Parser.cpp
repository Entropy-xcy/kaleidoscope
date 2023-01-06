//
// Created by entropy on 1/5/23.
//

#include "kaleidoscope/Parser/Parser.h"

namespace kaleidoscope {
    int Parser::getNextToken() {
        return CurTok = lexer.gettok();
    }

    int Parser::GetTokPrecedence() {
        if (!isascii(CurTok))
            return -1;

        // Make sure it's a declared binop.
        int TokPrec = BinopPrecedence[CurTok];
        if (TokPrec <= 0)
            return -1;
        return TokPrec;
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
        auto Result = std::make_unique<NumberExprAST>(lexer.NumVal);
        getNextToken();
        return std::move(Result);
    }

    std::unique_ptr<ExprAST> Parser::ParseParenExpr() {
        getNextToken(); // eat (.
        auto V = ParseExpression();
        if (!V)
            return nullptr;

        if (CurTok != ')')
            return LogError("expected ')'");
        getNextToken(); // eat ).
        return V;
    }

    std::unique_ptr<ExprAST> Parser::ParseIdentifierExpr() {
        std::string IdName = lexer.IdentifierStr;

        getNextToken(); // eat identifier.

        if (CurTok != '(') // Simple variable ref.
            return std::make_unique<VariableExprAST>(IdName);

        // Call.
        getNextToken(); // eat (
        std::vector<std::unique_ptr<ExprAST>> Args;
        if (CurTok != ')') {
            while (true) {
                if (auto Arg = ParseExpression())
                    Args.push_back(std::move(Arg));
                else
                    return nullptr;

                if (CurTok == ')')
                    break;

                if (CurTok != ',')
                    return LogError("Expected ')' or ',' in argument list");
                getNextToken();
            }
        }

        // Eat the ')'.
        getNextToken();

        return std::make_unique<CallExprAST>(IdName, std::move(Args));
    }

    std::unique_ptr<ExprAST> Parser::ParsePrimary() {
        switch (CurTok) {
            default:
                return LogError("unknown token when expecting an expression");
            case tok_identifier:
                return ParseIdentifierExpr();
            case tok_number:
                return ParseNumberExpr();
            case '(':
                return ParseParenExpr();
        }
    }

    std::unique_ptr<ExprAST> Parser::ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {
        // If this is a binop, find its precedence.
        while (true) {
            int TokPrec = GetTokPrecedence();

            // If this is a binop that binds at least as tightly as the current binop,
            // consume it, otherwise we are done.
            if (TokPrec < ExprPrec)
                return LHS;

            // Okay, we know this is a binop.
            int BinOp = CurTok;
            getNextToken(); // eat binop

            // Parse the primary expression after the binary operator.
            auto RHS = ParsePrimary();
            if (!RHS)
                return nullptr;

            // If BinOp binds less tightly with RHS than the operator after RHS, let
            // the pending operator take RHS as its LHS.
            int NextPrec = GetTokPrecedence();
            if (TokPrec < NextPrec) {
                RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
                if (!RHS)
                    return nullptr;
            }

            // Merge LHS/RHS.
            LHS =
                    std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
        }
    }

    std::unique_ptr<ExprAST> Parser::ParseExpression() {
        auto LHS = ParsePrimary();
        if (!LHS)
            return nullptr;

        return ParseBinOpRHS(0, std::move(LHS));
    }

    std::unique_ptr<PrototypeAST> Parser::ParsePrototype() {
        if (CurTok != tok_identifier)
            return LogErrorP("Expected function name in prototype");

        std::string FnName = lexer.IdentifierStr;
        getNextToken();

        if (CurTok != '(')
            return LogErrorP("Expected '(' in prototype");

        std::vector<std::string> ArgNames;
        while (getNextToken() == tok_identifier)
            ArgNames.push_back(lexer.IdentifierStr);
        if (CurTok != ')')
            return LogErrorP("Expected ')' in prototype");

        // success.
        getNextToken(); // eat ')'.

        return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
    }

    std::unique_ptr<FunctionAST> Parser::ParseDefinition() {
        getNextToken(); // eat def.
        auto Proto = ParsePrototype();
        if (!Proto)
            return nullptr;

        if (auto E = ParseExpression())
            return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
        return nullptr;
    }

    std::unique_ptr<FunctionAST> Parser::ParseTopLevelExpr() {
        if (auto E = ParseExpression()) {
            // Make an anonymous proto.
            auto Proto = std::make_unique<PrototypeAST>("__anon_expr",
                                                        std::vector<std::string>());
            return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
        }
        return nullptr;
    }

    std::unique_ptr<PrototypeAST> Parser::ParseExtern() {
        getNextToken(); // eat extern.
        return ParsePrototype();
    }

    void Parser::HandleDefinition() {
        if (ParseDefinition()) {
            fprintf(stderr, "Parsed a function definition.\n");
        } else {
            // Skip token for error recovery.
            getNextToken();
        }
    }

    void Parser::HandleExtern() {
        if (ParseExtern()) {
            fprintf(stderr, "Parsed an extern\n");
        } else {
            // Skip token for error recovery.
            getNextToken();
        }
    }

    void Parser::HandleTopLevelExpression() {
        if (ParseTopLevelExpr()) {
            fprintf(stderr, "Parsed a top-level expr\n");
        } else {
            // Skip token for error recovery.
            getNextToken();
        }
    }

    void Parser::MainLoop() {
        fprintf(stderr, "ready> ");
        getNextToken();
        while (true) {
            fprintf(stderr, "ready> ");
            switch (CurTok) {
                case tok_eof:
                    return;
                case ';': // ignore top-level semicolons.
                    getNextToken();
                    break;
                case tok_def:
                    HandleDefinition();
                    break;
                case tok_extern:
                    HandleExtern();
                    break;
                default:
                    HandleTopLevelExpression();
                    break;
            }
        }
    }
}
