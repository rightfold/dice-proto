#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

namespace dc
{
    struct llvm_context
    {
        llvm::Module* m;
    };

    void llvm_retain(llvm::IRBuilder<>& ib, llvm::Value* v);
    void llvm_release(llvm::IRBuilder<>& ib, llvm::Value* v);
}
