#include "llvm.hxx"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

void dc::llvm_retain(llvm::IRBuilder<>& ib, llvm::Value* v)
{
    (void)ib;
    (void)v;
}

void dc::llvm_release(llvm::IRBuilder<>& ib, llvm::Value* v)
{
    (void)ib;
    (void)v;
}
