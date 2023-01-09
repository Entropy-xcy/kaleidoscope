//
// Created by entropy on 1/6/23.
//

#ifndef KALEIDOSCOPE_CODEGEN_H
#define KALEIDOSCOPE_CODEGEN_H

#include "kaleidoscope/Parser/AST.h"
#include "kaleidoscope/Parser/Parser.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "kaleidoscope/JIT/KaleidoscopeJIT.h"

using namespace llvm;
namespace kaleidoscope{
    class CodeGen {
        Parser parser;
        std::unique_ptr<LLVMContext> TheContext;
        std::unique_ptr<IRBuilder<>> Builder;
        std::unique_ptr<Module> TheModule;
        std::unique_ptr<legacy::FunctionPassManager> TheFPM;
        std::map<std::string, Value*> NamedValues;
        std::unique_ptr<llvm::orc::KaleidoscopeJIT> TheJIT;
        Value *LogErrorV(const char *Str);
        Value *LogError(const char *Str);

        ExitOnError ExitOnErr;

    public:
        explicit CodeGen(Parser parser);
        Value* codegen(ExprAST* expr);
        Value* codegen(NumberExprAST* expr);
        Value* codegen(VariableExprAST* expr);
        Value* codegen(BinaryExprAST* expr);
        Value* codegen(CallExprAST* expr);
        Function* codegen(PrototypeAST* expr);
        Function* codegen(FunctionAST* expr);

        void HandleDefinition();
        void HandleExtern();
        void HandleTopLevelExpression();

        void InitializeModuleAndPassManager();
        void InitializeNativeTarget();
        void InitializeNativeTargetAsmPrinter();
        void InitializeNativeTargetAsmParser();

        void MainLoop();
    };
}

#endif //KALEIDOSCOPE_CODEGEN_H
