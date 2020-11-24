#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

////////////////////////////////////////////////////////////////////////////////
// PASS IMPLEMENTATION
////////////////////////////////////////////////////////////////////////////////
namespace {
    void visitor(llvm::Function &F) {
        llvm::errs() << "[+] function: " << F.getName() << "\n";
    }

    struct MyPass : llvm::PassInfoMixin<MyPass> {
        llvm::PreservedAnalyses runOnFunction(llvm::Function &F) {
            visitor(F);

            return llvm::PreservedAnalyses::all();
        }
    };

    struct MyPassWrapper : public llvm::FunctionPass {
        static char ID;
        MyPass MP;

        MyPassWrapper() : llvm::FunctionPass(this->ID) {}

        bool runOnFunction(llvm::Function &F) {
            MP.runOnFunction(F);

            return false;
        }
    };
};

////////////////////////////////////////////////////////////////////////////////
// PASS REGISTER
////////////////////////////////////////////////////////////////////////////////

char MyPassWrapper::ID = 0;

llvm::RegisterPass<MyPassWrapper> RP("my-pass", "MyPass", true, false);
