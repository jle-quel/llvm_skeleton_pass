#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/ADT/MapVector.h"

////////////////////////////////////////////////////////////////////////////////
// PASS IMPLEMENTATION
////////////////////////////////////////////////////////////////////////////////

namespace {
    struct MyPass : llvm::AnalysisInfoMixin<MyPass> {
        static llvm::AnalysisKey Key;

        llvm::MapVector<const llvm::Function *, unsigned> runOnModule(llvm::Module &M) {
            llvm::MapVector<const llvm::Function *, unsigned> Res;

            for (const auto &F: M.getFunctionList()) {
                Res.insert(std::make_pair(&F, 0));
            }

            return Res;
        }
    };

    struct MyPassWrapper : public llvm::ModulePass {
        static char ID;
        MyPass MP;
        llvm::MapVector<const llvm::Function *, unsigned> Res;

        MyPassWrapper() : llvm::ModulePass(this->ID) {}

        bool runOnModule(llvm::Module &M) {
            this->Res = MP.runOnModule(M);

            return false;
        }

        void print(llvm::raw_ostream &O, const llvm::Module *M) const {
            for (const auto &Val: this->Res) {
                O << "[+] function: " << Val.first->getName() << "\n";
            }
        }
    };
};

////////////////////////////////////////////////////////////////////////////////
// PASS REGISTER
////////////////////////////////////////////////////////////////////////////////

char MyPassWrapper::ID = 0;

llvm::RegisterPass<MyPassWrapper> RP("my-pass", "MyPass", true, true);
