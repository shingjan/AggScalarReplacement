#define DEBUG_TYPE "scalarrepl"
// from Write A Pass tutorial
#include "llvm/Support/raw_ostream.h"
// template file
#include "llvm/Transforms/Scalar.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/GetElementPtrTypeIterator.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Utils/PromoteMemToReg.h"
#include "llvm/Support/Debug.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Analysis/AssumptionCache.h"
// standard lib
#include <vector>
#include <queue>

using namespace llvm;

STATISTIC(NumReplaced,  "Number of aggregate allocas broken up");
STATISTIC(NumPromoted,  "Number of scalar allocas promoted to register");

namespace {

class Test : public FunctionPass {
public:
	static char ID;
	// Alloca insts to be NumPromoted
	std::vector<AllocaInst *> promotables;
	// preserve the insertion order of insts
	std::queue<AllocaInst *> currList;
	std::queue<AllocaInst *> postList;
	Test() : FunctionPass(ID) {
		//initializeTestPass(*PassRegistry::getPassRegistry());
	}
	bool runOnFunction(Function &F);

	// getAnalysisUsage - List passes required by this pass.  We also know it
	// will not alter the CFG, so say so.
	virtual void getAnalysisUsage(AnalysisUsage &AU) const {
		// ACache & DTreeWrapperPass need to be run before this pass
		AU.addRequired<AssumptionCacheTracker>();
 		AU.addRequired<DominatorTreeWrapperPass>();
		AU.setPreservesCFG();
	}
	// Public interface to create the ScalarReplAggregates pass.
	// This function is provided to you.
	FunctionPass *createTestPass() { return new Test(); }

private:
	DominatorTree *DTree = nullptr;
	AssumptionCache *ACache = nullptr;
	bool isAllocaPromotable(const AllocaInst *AI);
	bool promoteAllocas();
	bool isSafeToEliminate(AllocaInst *AI);
	bool isSafeU1U2(const Instruction *inst);
	bool eliminateAlloca(AllocaInst *AI);
};



}
// end anonymous scope
char Test::ID = 0;
static RegisterPass<Test> X("scalarreplace-ys26", "Scalar Replacement of Aggregates (by <ys26>)",
			    false /* Only looks at CFG */,
			    false /* Analysis Pass */);

//===----------------------------------------------------------------------===//
//                      SKELETON FUNCTION TO BE IMPLEMENTED
//===----------------------------------------------------------------------===//
//

bool Test::isAllocaPromotable(const AllocaInst *AI) {
	auto AllocaTy = AI->getAllocatedType();
	if (AllocaTy->isFPOrFPVectorTy() | AllocaTy->isIntOrIntVectorTy() | AllocaTy->isPtrOrPtrVectorTy()) {
		for (const User *user : AI->users()) {
	    if (const LoadInst *LI = dyn_cast<LoadInst>(user)) {
	      if (LI->isVolatile())
	        return false;
	    } else if (const StoreInst *SI = dyn_cast<StoreInst>(user)) {
	      if (SI->isVolatile())
	        return false;
	    } else {
				return false;
			}
		}
		return true;
	}
	return false;
}

bool Test::promoteAllocas() {
	NumPromoted += promotables.size();
	if(promotables.empty()) return false;
	PromoteMemToReg(promotables, *DTree, ACache);
	promotables.clear();
	return true;
}
/*
An alloca instruction can be eliminated if the resulting pointer ptr is used
only in these two ways:
(U1)
In a getelementptr instruction that satisfies both these conditions:
	∗It is of the form : getelementptr ptr, 0, constant[, ...  constant].
	∗The result of the getelementptr is only used in instructions of type U1 or
	U2, or as the pointer argument of a load or store instruction,
	i.e., the pointer stored into (not the value being stored).
(U2)
In a’eq’or’ne’comparison instruction, where the other operand is the NULL
pointer value.
*/

bool Test::isSafeToEliminate(AllocaInst *AI) {
	if (AI->use_empty()) {
		AI->eraseFromParent();
		return false;
	}
	if (AI->isArrayAllocation() || !AI->getAllocatedType()->isSized()) {
		return false;
	}
	return isSafeU1U2(AI);
}

bool Test::isSafeU1U2(const Instruction *inst) {
	bool safe = true;
	for (const User *user : inst->users()) {
		if (const GetElementPtrInst* GEPI = dyn_cast<GetElementPtrInst>(user)) {
			gep_type_iterator iter = gep_type_begin(GEPI);
			gep_type_iterator tail = gep_type_end(GEPI);
			while (iter != tail) {
				ConstantInt *tmpInt = dyn_cast<ConstantInt>(iter.getOperand());
				if (!tmpInt)
					return false;
				iter++;
			}
			errs() << "safe GEPI: " << *GEPI << "\n";
			safe |= isSafeU1U2(GEPI);
		} else if (const CmpInst *CmpI = dyn_cast<CmpInst>(user)) {
			if (CmpI->getPredicate() == CmpInst::ICMP_EQ ||
			CmpI->getPredicate() == CmpInst::ICMP_NE) {
			 	ConstantInt *firstOp = dyn_cast<ConstantInt>(CmpI->getOperand(0));
				ConstantInt *secondOp = dyn_cast<ConstantInt>(CmpI->getOperand(1));
				if(firstOp->getZExtValue() != 0 && secondOp->getZExtValue() != 0) {
					return false;
				}
				errs() << "safe CmpInst: " << *CmpI << "\n";
			} else {
				return false;
			}
		} else {
			return false;
		}
	}
	return safe;
}

// safely eliminate alloca inst and put extra allocas into postList

bool Test::eliminateAlloca(AllocaInst *AI) {
	
}


// Function runOnFunction:
// Entry point for the overall ScalarReplAggregates function pass.
// This function is provided to you.
bool Test::runOnFunction(Function &F) {

  bool Changed = false;
	errs() << "function: " << F.getName() << "\n";
	//if (skipFunction(F))
		//return Changed;
	LLVMContext *C = &F.getContext();
	DTree = &getAnalysis<DominatorTreeWrapperPass>().getDomTree();
	ACache = &getAnalysis<AssumptionCacheTracker>().getAssumptionCache(F);

	// put every alloca inst into worklist
	BasicBlock &head = F.getEntryBlock();
	BasicBlock::iterator tail = std::prev(head.end());
	BasicBlock::iterator iter = head.begin();
  while (iter != tail) {
		//Instruction* ii = &*iter;
		//errs() << *ii << "\n";
    if (AllocaInst *AI = dyn_cast<AllocaInst>(iter)) {
			currList.push(AI);
		}
		iter++;
  }

	do {
		// 1st pass: promote all promotable allocas to reg
		while (!currList.empty()) {
			AllocaInst* ai = currList.front();
			currList.pop();
			//errs() << "SROA alloca: " << ai << "\n";
			if(isAllocaPromotable(ai)){
				errs() << "SROA promotable alloca: " << *ai << "\n";
				promotables.push_back(ai);
			} else {
				postList.push(ai);
			}
		}
		Changed |= promoteAllocas();
		currList = postList;
		std::queue<AllocaInst *> empty;
		postList.swap(empty);

		// 2nd pass: replace struct to scalar
		while (!currList.empty()) {
			AllocaInst* ai = currList.front();
			currList.pop();
			// remove zero use alloca instruction
			if(isSafeToEliminate(ai)) {
				errs() << "SROA removable inst: " << *ai << "\n";
				eliminateAlloca(ai);
				Changed = true;
			}
		}
		//currList = postList;
		std::queue<AllocaInst *> empty2;
		postList.swap(empty);
	} while (!currList.empty());

	return Changed;
}
