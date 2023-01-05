//
// Created by entropy on 1/5/23.
//

#ifndef KALEIDOSCOPE_AST_H
#define KALEIDOSCOPE_AST_H

#include <string>
#include <memory>
#include <utility>
#include <vector>

namespace kaleidoscope {
    class ExprAST {
        public:
            virtual ~ExprAST() = default;
    };

    class NumberExprAST : public ExprAST {
        double Val;
    public:
        explicit NumberExprAST(double Val): Val(Val) {}
    };

    class VariableExprAST : public ExprAST {
        std::string Name;

    public:
        explicit VariableExprAST(std::string Name) : Name(std::move(Name)) {}
    };

    class BinaryExprAST : public ExprAST {
        char Op;
        std::unique_ptr<ExprAST> LHS, RHS;

    public:
        BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS,
                      std::unique_ptr<ExprAST> RHS)
                : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
    };

    class CallExprAST : public ExprAST {
        std::string Callee;
        std::vector<std::unique_ptr<ExprAST>> Args;

    public:
        CallExprAST(std::string Callee,
                    std::vector<std::unique_ptr<ExprAST>> Args)
                : Callee(std::move(Callee)), Args(std::move(Args)) {}
    };

    class PrototypeAST {
        std::string Name;
        std::vector<std::string> Args;

    public:
        PrototypeAST(std::string name, std::vector<std::string> Args)
                : Name(std::move(name)), Args(std::move(Args)) {}

        [[nodiscard]] std::string getName() const { return Name; }
    };

    class FunctionAST {
        std::unique_ptr<PrototypeAST> Proto;
        std::unique_ptr<ExprAST> Body;

    public:
        FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                    std::unique_ptr<ExprAST> Body)
                : Proto(std::move(Proto)), Body(std::move(Body)) {}
    };
}

#endif //KALEIDOSCOPE_AST_H
