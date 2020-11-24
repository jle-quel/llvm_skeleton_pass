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

    struct MyPass: llvm::PassInfoMixin<MyPass> {
        llvm::PreservedAnalyses run(llvm::Function &F, llvm::FunctionAnalysisManager &) {
			return this->runOnFunction(F);
		}

        llvm::PreservedAnalyses runOnFunction(llvm::Function &F) {
            visitor(F);

            return llvm::PreservedAnalyses::all();
        }
    };
};

////////////////////////////////////////////////////////////////////////////////
// PASS REGISTER
////////////////////////////////////////////////////////////////////////////////

bool pipelineParsingCallback(llvm::StringRef Name, llvm::FunctionPassManager &FPM, llvm::ArrayRef<llvm::PassBuilder::PipelineElement>) {
    bool Res = false;

    if (Name == "my-pass") {
        Res = true;

        FPM.addPass(MyPass());
    }

    return Res;
}

void passBuilderCallback(llvm::PassBuilder &PB) {
    PB.registerPipelineParsingCallback(pipelineParsingCallback);
}

llvm::PassPluginLibraryInfo getMyPassPluginInfo() {
    llvm::PassPluginLibraryInfo Res;

    Res.APIVersion = LLVM_PLUGIN_API_VERSION;
    Res.PluginName = "MyPass";
    Res.PluginVersion = LLVM_VERSION_STRING;
    Res.RegisterPassBuilderCallbacks = passBuilderCallback;

    return Res;
}

extern "C" LLVM_ATTRIBUTE_WEAK llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return getMyPassPluginInfo();
}
