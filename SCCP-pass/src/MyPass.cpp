#include <llvm-11/llvm/ADT/SmallVector.h>
#include <llvm/ADT/DenseMap.h>
#include <llvm/Analysis/AliasAnalysis.h>
#include <llvm/Analysis/ConstantFolding.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/CFG.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/Dominators.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <llvm/Pass.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/Utils/Mem2Reg.h>

#include <map>
#include <memory>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace llvm;

namespace {

/**
 * Структура, необходимая для регистрации своего прохода в менеджере проходов.
 */
struct MySCCP : PassInfoMixin<MySCCP> {
    /* PreservedAnalyses - множество анализов, которые сохраняются после данного
     * прохода, чтобы не запускать их заново.
     *
     * run() непосредственно нужен для запуска прохода.
     *
     * Так как мы просто хотим вывести: "имя функции - количество аргументов",
     * то мы возвращаем all(), что говорит о том, что ни один анализ не будет
     * испорчен */

    size_t NumDeadBlocks = 0;
    size_t NumConstantsPropagated = 0;

    enum class LatticeState { UNDEF = 0, CONST, OVERDEF };

    struct ValueState {
        LatticeState LatState;
        Constant *Constant;

        ValueState() : LatState(LatticeState::UNDEF), Constant(nullptr) {}
        ValueState(LatticeState S) : LatState(S), Constant(nullptr) {}
        ValueState(class Constant *C)
            : LatState(LatticeState::CONST), Constant(C) {}
    };

    using Lattice = DenseMap<Value *, ValueState>;
    Lattice LatticeVector;
    std::stack<Value *> SsaWL;
    std::stack<BasicBlock *> CfgWL;
    DenseMap<const BasicBlock *, bool> ExecutableBlocks;

    void markEdgeExecutable(BasicBlock *From, BasicBlock *To) {
        if (!ExecutableBlocks[To]) {
            CfgWL.push(To);
        }
    }

    ValueState latticeMeet(const ValueState &LHS, const ValueState &RHS) {
        if (LHS.LatState == LatticeState::UNDEF) {
            return RHS;
        }
        if (RHS.LatState == LatticeState::UNDEF) {
            return LHS;
        }

        if (LHS.LatState == LatticeState::OVERDEF ||
            RHS.LatState == LatticeState::OVERDEF) {
            return ValueState(LatticeState::OVERDEF);
        }

        if (LHS.Constant == RHS.Constant) { // equal constants
            return LHS;
        }
        return ValueState(LatticeState::OVERDEF); // different constants
    }

    void updateValue(Value *V, const ValueState &NewState) {
        ValueState &OldState = LatticeVector[V];

        if (OldState.LatState != NewState.LatState ||
            (OldState.LatState == LatticeState::CONST &&
             OldState.Constant != NewState.Constant)) {
            OldState = NewState;
            SsaWL.push(V);
        }
    }

    void sccpInit(Function &F) {
        LatticeVector.clear();
        SsaWL = std::stack<Value *>();
        CfgWL = std::stack<BasicBlock *>();
        ExecutableBlocks.clear();

        CfgWL.push(&F.getEntryBlock());
        ExecutableBlocks[&F.getEntryBlock()] = true;

        for (inst_iterator Iit = inst_begin(F); Iit != inst_end(F); ++Iit) {
            // outs() << Iit->getOpcodeName() << " " << isa<Constant>(*Iit)
            //        << "\n";
            if (isa<Constant>(&(*Iit))) {
                LatticeVector[&(*Iit)] = ValueState(cast<Constant>(&(*Iit)));
            } else {
                LatticeVector[&(*Iit)] = ValueState(LatticeState::UNDEF);
            }
        }

        for (Argument &Arg : F.args()) {
            LatticeVector[&Arg] = ValueState(LatticeState::OVERDEF);
        }

        return;
    }

    void visitBranch(BranchInst *BInst) {
        if (BInst->isUnconditional()) {
            markEdgeExecutable(BInst->getParent(), BInst->getSuccessor(0));
            return;
        }

        auto *BCond = BInst->getCondition();
        if (Constant *CInst = ConstantFoldInstruction(
                BInst, BInst->getModule()->getDataLayout())) {
            if (auto *CI = dyn_cast<ConstantInt>(CInst)) {
                bool CondVal = CI->getZExtValue();
                markEdgeExecutable(nullptr,
                                   BInst->getSuccessor(CondVal ? 0 : 1));
            }
        }

        auto &BState = LatticeVector[BCond];

        if (BState.LatState == LatticeState::CONST) {
            if (auto *CI = dyn_cast<ConstantInt>(BCond)) {
                bool CondVal = CI->getZExtValue();
                markEdgeExecutable(nullptr,
                                   BInst->getSuccessor(CondVal ? 0 : 1));
            }
        } else {
            markEdgeExecutable(nullptr, BInst->getSuccessor(0));
            markEdgeExecutable(nullptr, BInst->getSuccessor(1));
        }
    }

    void visitPHI(PHINode *PNode) {
        ValueState NewState(LatticeState::UNDEF);

        for (size_t i = 0; i < PNode->getNumIncomingValues(); ++i) {
            const auto *IncBlock = PNode->getIncomingBlock(i);
            if (!ExecutableBlocks[IncBlock]) {
                continue;
            }

            auto *IncValue = PNode->getIncomingValue(i);
            const auto &IncValueState = LatticeVector[IncValue];

            NewState = latticeMeet(NewState, IncValueState);
            if (NewState.LatState == LatticeState::OVERDEF) {
                break;
            }
        }

        updateValue(PNode, NewState);
    }

    void visitBinOp(Instruction *Inst) {
        auto *LHS = Inst->getOperand(0);
        auto *RHS = Inst->getOperand(1);

        auto &LHSState = LatticeVector[LHS];
        auto &RHSState = LatticeVector[RHS];

        if (auto *ConstOper = dyn_cast<Constant>(LHS)) {
            LHSState.Constant = ConstOper;
            LHSState.LatState = LatticeState::CONST;
        }
        if (auto *ConstOper = dyn_cast<Constant>(RHS)) {
            RHSState.Constant = ConstOper;
            RHSState.LatState = LatticeState::CONST;
        }

        if (LHSState.LatState == LatticeState::CONST &&
            RHSState.LatState == LatticeState::CONST) {

            SmallVector<Constant *, 2> Operands = {LHSState.Constant,
                                                   RHSState.Constant};

            if (Constant *Res = ConstantFoldInstOperands(
                    Inst, Operands, Inst->getModule()->getDataLayout())) {
                updateValue(Inst, ValueState(Res));
                return;
            }
        }

        if (LHSState.LatState == LatticeState::OVERDEF ||
            RHSState.LatState == LatticeState::OVERDEF) { // excessive?
            updateValue(Inst, ValueState(LatticeState::OVERDEF));
            return;
        }

        updateValue(Inst, ValueState(LatticeState::OVERDEF)); // mark OVERDEF
    }

    void visitOp(Instruction *Inst) {
        outs() << Inst->getOpcodeName() << '\n';
        switch (Inst->getOpcode()) {
        case Instruction::Br:
            visitBranch(cast<BranchInst>(Inst));
            break;
        case Instruction::PHI:
            visitPHI(cast<PHINode>(Inst));
            break;
        case Instruction::Add:
        case Instruction::Sub:
        case Instruction::Mul:
        case Instruction::ICmp:
            visitBinOp(Inst);
            break;
        case Instruction::Call:
            LatticeVector[Inst] = ValueState(LatticeState::OVERDEF);
            break;
        default:
            // TODO: for other instructions, mark as overdefined if not constant
            if (!isa<Constant>(Inst)) {
                LatticeVector[Inst] = LatticeState::OVERDEF;
            }
            break;
        }
    }

    bool transformFunction(Function &F, DominatorTree &DT) {
        bool Changed = false;

        for (BasicBlock &BB : F) {
            for (Instruction &I : BB) {
                ValueState &State = LatticeVector[&I];
                if (State.LatState == LatticeState::CONST && State.Constant) {
                    I.replaceAllUsesWith(State.Constant);
                    NumConstantsPropagated++;
                    Changed = true;
                }
            }
        }

        SmallVector<BasicBlock *, 32> DeadBlocks;
        for (BasicBlock &BB : F) {
            if (!ExecutableBlocks[&BB]) {
                DeadBlocks.push_back(&BB);
            }
        }

        for (BasicBlock *BB : DeadBlocks) {
            BB->dropAllReferences();
            BB->eraseFromParent();
            NumDeadBlocks++;
            Changed = true;
        }

        return Changed;
    }

    PreservedAnalyses run(Function &Function,
                          FunctionAnalysisManager &AnalysisManager) {
        sccpInit(Function);

        DominatorTree &DT =
            AnalysisManager.getResult<DominatorTreeAnalysis>(Function);

        while (!SsaWL.empty() || !CfgWL.empty()) {
            if (!SsaWL.empty()) {
                auto *Dest = SsaWL.top();
                SsaWL.pop();
                if (Instruction *Inst = dyn_cast<Instruction>(Dest)) {
                    visitOp(Inst);
                }
            } else {
                const auto *Dest = CfgWL.top();
                CfgWL.pop();
                ExecutableBlocks[Dest] = true;

                for (const Instruction &I : *Dest) {
                    // FIXME: const_cast
                    Instruction *Inst = const_cast<Instruction *>(&I);
                    visitOp(Inst);
                }

                // FIXME: const_cast?
                // auto *SingleSuccessor =
                //     const_cast<BasicBlock *>(Dest->getSingleSuccessor());
                // if (SingleSuccessor &&
                //     ExecutableBlocks[SingleSuccessor] == false) {
                //     CfgWL.push(SingleSuccessor);
                // }
            }
        }

        auto Res = transformFunction(Function, DT);

        outs() << NumConstantsPropagated << "  " << NumDeadBlocks << "\n";

        return (PreservedAnalyses::all());
    }

    /* По умолчанию данный проход будет пропущен, если функция помечена
     * атрибутом optnone (не производить оптимизаций над ней). Поэтому
     * необходимо вернуть true, чтобы мы могли обходить и их. (в режиме
     * сборки -O0 все функции помечены как неоптимизируемые) */
    static bool isRequired(void) { return (true); }
}; // namespace
} /* namespace */

/**
 * Наш проход будет реализован в виде отдельно подключаемого плагина (расширения
 * языка). Это удобный способ расширить возможности компилятора. Например,
 * сделать поддержку своей прагмы, своей оптимизации, выдачи своего
 * предупреждения.
 *
 * PassPluginLibraryInfo - структура, задающая базовые параметры для нашего
 * плагина. Её надо составить из:
 * - Версии API (для отслеживания совместимости ABI можно
 *   использовать LLVM_PLUGIN_API_VERSION)
 * - Имя плагина
 * - Версии плагина
 * - Callback для регистрации плагина через PassBuilder
 */
/**
 * По-модному это делают с помощью лямбда-функции, но можно и по-старинке.
 */
bool CallBackForPipelineParser(StringRef Name, FunctionPassManager &FPM,
                               ArrayRef<PassBuilder::PipelineElement>) {
    if (Name == "MySCCP") {
        FPM.addPass(MySCCP());
        return (true);
    } else {
        return (false);
    }
} /* CallBackForPipelineParser */

void CallBackForPassBuilder(PassBuilder &PB) {
    PB.registerPipelineParsingCallback(&CallBackForPipelineParser);
} /* CallBackForPassBuider */

PassPluginLibraryInfo getMyPassPluginInfo(void) {
    uint32_t APIversion = LLVM_PLUGIN_API_VERSION;
    const char *PluginName = "MyPass";
    const char *PluginVersion = LLVM_VERSION_STRING;

    PassPluginLibraryInfo info = {APIversion, PluginName, PluginVersion,
                                  CallBackForPassBuilder};

    return (info);
} /* getMyPassPluginInfo */

/**
 * Интерфейс, который гарантирует, что "opt" распознаст наш проход.
 * "-passes=MyPass"
 */
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
// llvmGetPassPluginInfo() {
//     return (getMyPassPluginInfo());
// } /* llvmGetPassPluginInfo */

llvmGetPassPluginInfo() {
    return {.APIVersion = LLVM_PLUGIN_API_VERSION,
            .PluginName = "My_SCCP",
            .PluginVersion = "v0.1",
            .RegisterPassBuilderCallbacks = [](PassBuilder &PB) {
                // Register your pass
                PB.registerPipelineParsingCallback(
                    [](StringRef Name, FunctionPassManager &FPM,
                       ArrayRef<PassBuilder::PipelineElement>) {
                        if (Name == "MySCCP") {
                            // Create a pipeline that runs mem2reg first
                            FPM.addPass(PromotePass()); // mem2reg
                            FPM.addPass(MySCCP());
                            return true;
                        }
                        return false;
                    });

                // Register as part of optimization pipeline
                PB.registerVectorizerStartEPCallback(
                    [](FunctionPassManager &FPM,
                       PassBuilder::OptimizationLevel Level) {
                        FPM.addPass(PromotePass());
                        FPM.addPass(MySCCP());
                    });
            }};
}