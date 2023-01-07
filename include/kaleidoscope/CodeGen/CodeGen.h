//
// Created by entropy on 1/6/23.
//

#ifndef KALEIDOSCOPE_CODEGEN_H
#define KALEIDOSCOPE_CODEGEN_H

#include "kaleidoscope/Parser/AST.h"

using namespace llvm;
namespace kaleidoscope{
    class CodeGen {
        std::unique_ptr<LLVMContext> TheContext;
        std::unique_ptr<IRBuilder<>> Builder;
        std::unique_ptr<Module> TheModule;
        std::map<std::string, Value*> NamedValues;
        Value *LogErrorV(const char *Str);
        Value *LogError(const char *Str);

    public:
        CodeGen();
        Value* codegen(ExprAST* expr);
        Value* codegen(NumberExprAST* expr);
        Value* codegen(VariableExprAST* expr);
        Value* codegen(BinaryExprAST* expr);
        Value* codegen(CallExprAST* expr);
        Function* codegen(PrototypeAST* expr);
        Function* codegen(FunctionAST* expr);
    };
}

#endif //KALEIDOSCOPE_CODEGEN_H