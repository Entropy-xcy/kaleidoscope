// Created by entropy on 1/6/23.
//

#include "kaleidoscope/CodeGen/CodeGen.h"

#include <utility>


using namespace llvm;


kaleidoscope::CodeGen::CodeGen(Parser parser) : parser(std::move(parser)) {
    TheContext = std::make_unique<LLVMContext>();
    TheModule = std::make_unique<Module>("my cool jit", *TheContext);
    Builder = std::make_unique<IRBuilder<>>(*TheContext);
}

Value *kaleidoscope::CodeGen::codegen(kaleidoscope::NumberExprAST *expr) {
    // APFloat: Arbitary precision float
    return ConstantFP::get(*TheContext, APFloat(expr->Val));
}

Value *kaleidoscope::CodeGen::codegen(kaleidoscope::VariableExprAST *expr) {
    Value *V = NamedValues[expr->Name];
    if (!V)
        LogErrorV("Unknown variable name");
    return V;
}

Value *kaleidoscope::CodeGen::codegen(kaleidoscope::BinaryExprAST *expr) {
    Value *L = codegen(expr->LHS.get());
    Value *R = codegen(expr->RHS.get());
    if (!L || !R)
        return nullptr;

    switch (expr->Op) {
        case '+':
            return Builder->CreateFAdd(L, R, "addtmp");
        case '-':
            return Builder->CreateFSub(L, R, "subtmp");
        case '*':
            return Builder->CreateFMul(L, R, "multmp");
        case '<':
            L = Builder->CreateFCmpULT(L, R, "cmptmp");
            // Convert bool 0/1 to double 0.0 or 1.0
            return Builder->CreateUIToFP(L, Type::getDoubleTy(*TheContext),
                                         "booltmp");
        default:
            return LogErrorV("invalid binary operator");
    }
}

Value *kaleidoscope::CodeGen::codegen(kaleidoscope::CallExprAST *expr) {
    Function *CalleeF = TheModule->getFunction(expr->Callee);
    if (!CalleeF)
        return LogErrorV("Unknown function referenced");

    // If argument mismatch error.
    if (CalleeF->arg_size() != expr->Args.size())
        return LogErrorV("Incorrect # arguments passed");

    std::vector<Value *> ArgsV;
    for (unsigned i = 0, e = expr->Args.size(); i != e; ++i) {
        ArgsV.push_back(codegen(expr->Args[i].get()));

        if (!ArgsV.back())
            return nullptr;
    }

    return Builder->CreateCall(CalleeF, ArgsV, "calltmp");
}

Function *kaleidoscope::CodeGen::codegen(kaleidoscope::PrototypeAST *expr) {
    std::vector<Type *> Doubles(expr->Args.size(), Type::getDoubleTy(*TheContext));
    FunctionType *FT =
            FunctionType::get(Type::getDoubleTy(*TheContext), Doubles, false);

    Function *F =
            Function::Create(FT, Function::ExternalLinkage, expr->Name, TheModule.get());

    // Set names for all arguments.
    unsigned Idx = 0;
    for (auto &Arg: F->args())
        Arg.setName(expr->Args[Idx++]);

    return F;
}

Function *kaleidoscope::CodeGen::codegen(kaleidoscope::FunctionAST *expr) {
    Function *TheFunction = TheModule->getFunction(expr->Proto->getName());

    if (!TheFunction)
        TheFunction = codegen(expr->Proto.get());

    if (!TheFunction)
        return nullptr;

    // Create a new basic block to start insertion into.
    BasicBlock *BB = BasicBlock::Create(*TheContext, "entry", TheFunction);
    Builder->SetInsertPoint(BB);

    // Record the function arguments in the NamedValues map.
    NamedValues.clear();
    for (auto &Arg: TheFunction->args())
        NamedValues[std::string(Arg.getName())] = &Arg;

    if (Value *RetVal = codegen(expr->Body.get())) {
        // Finish off the function.
        Builder->CreateRet(RetVal);

        // Validate the generated code, checking for consistency.
        verifyFunction(*TheFunction);
        TheFPM->run(*TheFunction);

        return TheFunction;
    }

    // Error reading body, remove function.
    TheFunction->eraseFromParent();
    return nullptr;
}

Value *kaleidoscope::CodeGen::LogErrorV(const char *Str) {
    LogError(Str);
    return nullptr;
}

Value *kaleidoscope::CodeGen::LogError(const char *Str) {
    fprintf(stderr, "Error: %s\n", Str);
    return nullptr;
}

Value *kaleidoscope::CodeGen::codegen(kaleidoscope::ExprAST *expr) {
    if (dynamic_cast<kaleidoscope::NumberExprAST *>(expr) != nullptr) {
        return codegen(dynamic_cast<kaleidoscope::NumberExprAST *>(expr));
    } else if (dynamic_cast<kaleidoscope::VariableExprAST *>(expr) != nullptr) {
        return codegen(dynamic_cast<kaleidoscope::VariableExprAST *>(expr));
    } else if (dynamic_cast<kaleidoscope::BinaryExprAST *>(expr) != nullptr) {
        return codegen(dynamic_cast<kaleidoscope::BinaryExprAST *>(expr));
    } else if (dynamic_cast<kaleidoscope::CallExprAST *>(expr) != nullptr) {
        return codegen(dynamic_cast<kaleidoscope::CallExprAST *>(expr));
    } else {
        return LogErrorV("Unknown expression type");
    }
}


void kaleidoscope::CodeGen::HandleDefinition() {
    if (auto FnAST = parser.ParseDefinition()) {
        if (auto *FnIR = codegen(FnAST.get())) {
            fprintf(stderr, "Read function definition:");
            FnIR->print(errs());
            fprintf(stderr, "\n");
        }
    } else {
        // Skip token for error recovery.
        parser.getNextToken();
    }
}

void kaleidoscope::CodeGen::HandleExtern() {
    if (auto ProtoAST = parser.ParseExtern()) {
        if (auto *FnIR = codegen(ProtoAST.get())) {
            fprintf(stderr, "Read extern: ");
            FnIR->print(errs());
            fprintf(stderr, "\n");
        }
    } else {
        // Skip token for error recovery.
        parser.getNextToken();
    }
}

void kaleidoscope::CodeGen::HandleTopLevelExpression() {
    if (auto FnAST = parser.ParseTopLevelExpr()) {
        if (auto *FnIR = codegen(FnAST.get())) {
            fprintf(stderr, "Read top-level expression:");
            FnIR->print(errs());
            fprintf(stderr, "\n");

            // Remove the anonymous expression.
            FnIR->eraseFromParent();
        }
    } else {
        // Skip token for error recovery.
        parser.getNextToken();
    }
}

void kaleidoscope::CodeGen::MainLoop() {
    fprintf(stderr, "ready> ");
    parser.getNextToken();
    while (true) {
        fprintf(stderr, "ready> ");
        switch (parser.CurTok) {
            case tok_eof:
                return;
            case ';': // ignore top-level semicolons.
                parser.getNextToken();
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

void kaleidoscope::CodeGen::InitializeModuleAndPassManager() {
    TheModule = std::make_unique<Module>("my cool jit", *TheContext);
    // Function Pass Manager
    TheFPM = std::make_unique<legacy::FunctionPassManager>(TheModule.get());
    // Do simple "peephole" optimizations and bit-twiddling optzns.
    TheFPM->add(createInstructionCombiningPass());
    // Reassociate expressions.
    TheFPM->add(createReassociatePass());
    // Eliminate Common SubExpressions.
    TheFPM->add(createGVNPass());
    // Simplify the control flow graph (deleting unreachable blocks, etc).
    TheFPM->add(createCFGSimplificationPass());

    TheFPM->doInitialization();
}
