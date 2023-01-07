//
// Created by entropy on 1/6/23.
//

#include "kaleidoscope/CodeGen/CodeGen.h"


using namespace llvm;



kaleidoscope::CodeGen::CodeGen() = default;

Value *kaleidoscope::CodeGen::codegen(kaleidoscope::ExprAST *expr) {
    return nullptr;
}

Value *kaleidoscope::CodeGen::codegen(kaleidoscope::NumberExprAST *expr) {
    // APFloat: Arbitary precision float
    return ConstantFP::get(*TheContext, APFloat(expr -> Val));
}

Value *kaleidoscope::CodeGen::codegen(kaleidoscope::VariableExprAST *expr) {
    Value *V = NamedValues[expr->Name];
    if (!V)
        LogErrorV("Unknown variable name");
    return V;
}

Value *kaleidoscope::CodeGen::codegen(kaleidoscope::BinaryExprAST *expr) {
    return nullptr;
}

Value *kaleidoscope::CodeGen::codegen(kaleidoscope::CallExprAST *expr) {
    return nullptr;
}

Function *kaleidoscope::CodeGen::codegen(kaleidoscope::PrototypeAST *expr) {
    return nullptr;
}

Function *kaleidoscope::CodeGen::codegen(kaleidoscope::FunctionAST *expr) {
    return nullptr;
}

Value *kaleidoscope::CodeGen::LogErrorV(const char *Str) {
    return nullptr;
}

Value *kaleidoscope::CodeGen::LogError(const char *Str) {
    return nullptr;
}
