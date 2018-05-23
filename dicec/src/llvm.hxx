#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Value.h>

namespace dc
{
    class type;

    struct llvm_context
    {
        explicit llvm_context(llvm::Module& m);

        llvm::Module* m;

        llvm::IntegerType* reference_count_type;
        llvm::PointerType* character_type;
    };

    void llvm_retain(llvm_context& c, llvm::IRBuilder<>& ib, type* t, llvm::Value* v);
    void llvm_release(llvm_context& c, llvm::IRBuilder<>& ib, type* t, llvm::Value* v);
}
