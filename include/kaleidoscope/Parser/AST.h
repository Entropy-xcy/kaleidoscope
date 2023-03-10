//
// Created by entropy on 1/5/23.
//

#ifndef KALEIDOSCOPE_AST_H
#define KALEIDOSCOPE_AST_H

#include <string>
#include <memory>
#include <utility>
#include <vector>
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

using namespace llvm;

namespace kaleidoscope {
    class ExprAST {
        public:
            virtual ~ExprAST() = default;
    };

    class NumberExprAST : public ExprAST {
    public:
        explicit NumberExprAST(double Val): Val(Val) {}

        double Val;
    };


    class VariableExprAST : public ExprAST {

    public:
        explicit VariableExprAST(std::string Name) : Name(std::move(Name)) {}

        std::string Name;
    };

    class BinaryExprAST : public ExprAST {

    public:
        BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS,
                      std::unique_ptr<ExprAST> RHS)
                : LHS(std::move(LHS)), RHS(std::move(RHS)), Op(op) {}

        std::unique_ptr<ExprAST> LHS;
        std::unique_ptr<ExprAST> RHS;
        char Op;
    };

    class CallExprAST : public ExprAST {

    public:
        CallExprAST(std::string Callee,
                    std::vector<std::unique_ptr<ExprAST>> Args)
                : Callee(std::move(Callee)), Args(std::move(Args)) {}

        std::string Callee;
        std::vector<std::unique_ptr<ExprAST>> Args;
    };

    class PrototypeAST {

    public:
        PrototypeAST(std::string name, std::vector<std::string> Args)
                : Args(std::move(Args)), Name(std::move(name)) {}

        [[nodiscard]] std::string getName() const { return Name; }

        std::vector<std::string> Args;
        std::string Name;
    };

    class FunctionAST {

    public:
        FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                    std::unique_ptr<ExprAST> Body)
                : Proto(std::move(Proto)), Body(std::move(Body)) {}

        std::unique_ptr<PrototypeAST> Proto;
        std::unique_ptr<ExprAST> Body;
    };
}

#endif //KALEIDOSCOPE_AST_H
