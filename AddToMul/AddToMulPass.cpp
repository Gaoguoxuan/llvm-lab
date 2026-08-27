#include "llvm/Config/llvm-config.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/ADT/SmallVector.h"

using namespace llvm;

namespace {

struct AddToMulPass : public PassInfoMixin<AddToMulPass> {

    PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {

        errs() << "正在对当前函数执行加法转乘法优化："
               << F.getName() << "\n";

        bool Changed = false;

        SmallVector<BinaryOperator *,16> Adds;

        for (BasicBlock &BB : F) {

            for (Instruction &I : BB) {

                if (BinaryOperator *bo = dyn_cast<BinaryOperator>(&I)) {

                    if (!bo) continue;

                    if (bo -> getOpcode() == Instruction::Add ||
                        bo -> getOpcode() == Instruction::FAdd) {

                        errs() << "找见加法指令：";
                        bo -> print(errs());
                        errs() << "\n";

                        Adds.push_back(bo);
                    }
                }
            }

        }

        for (BinaryOperator *Add:Adds) {

            IRBuilder<> Builder(Add);

            Value *opA = Add -> getOperand(0);
            Value *opB = Add -> getOperand(1);
            Value *Mul = nullptr;

            //旧逻辑未考虑整形运算与浮点运算两种情形
            //Value *Mul = Builder.CreateMul(opA,opB,Add->getName()+"_mul");
            if (Add-> getOpcode() == Instruction::Add) {
                Mul = Builder.CreateMul(opA,opB,Add->getName()+"_mul");
            }
            else if (Add-> getOpcode() == Instruction::FAdd) {
                Mul = Builder.CreateFMul(opA,opB,Add->getName()+"_fmul");
            }

            Add -> replaceAllUsesWith(Mul);
            Add -> eraseFromParent();

            Changed = true;
        }

        return Changed ? PreservedAnalyses::none():PreservedAnalyses::all();
    }

};

}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION,
        "AddToMulPass",
        LLVM_VERSION_STRING,
        [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, 
                   FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                    if (Name == "add-to-mul") {
                        FPM.addPass(AddToMulPass());
                        return true;
                    }
                    return false;
                });
        }

    };
}
