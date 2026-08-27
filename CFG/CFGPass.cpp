#include "llvm/Config/llvm-config.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/DenseMap.h"

#include "llvm/ADT/SmallVector.h"

using namespace llvm;

namespace {

struct CFGPass : public PassInfoMixin<CFGPass> {

    PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {

        errs() << "当前函数的CFG："
               << F.getName() << "\n";
/*
        for (BasicBlock &BB : F) {

            errs() << "\n当前BasicBlock： ";

            if(BB.hasName())
                errs() << BB.getName();
            else
                errs() << "未命名";

            errs() << "\n";

            for (BasicBlock *Succ : successors(&BB)) {

                errs() << "   ->  ";

                if(Succ->hasName())
                    errs() << Succ->getName();
                else
                    errs() << "未命名";

                errs() << "\n";
            }
        }*/

        DenseMap<const BasicBlock *, unsigned> BID;

        unsigned Counter = 0;

        for (BasicBlock &BB : F) {

            BID[&BB] = Counter;
            Counter++;
        }

        for (BasicBlock &BB : F) {
    
            for (BasicBlock *Succ : successors(&BB)) {
        
                errs() << "BB"
                       << BID[&BB]
                       << " -> BB"
                       << BID[Succ]
                       << "\n";
                       
            }
        }

        return PreservedAnalyses::all();
        
    }


};

}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION,
        "CFGPass",
        LLVM_VERSION_STRING,
        [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, 
                   FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                    if (Name == "prt-cfg") {
                        FPM.addPass(CFGPass());
                        return true;
                    }
                    return false;
                });
        }

    };
}

