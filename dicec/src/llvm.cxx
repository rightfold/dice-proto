#include "llvm.hxx"
#include "node/type.hxx"

#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

#include <array>

dc::llvm_context::llvm_context(llvm::Module& m)
    : m(&m)
{
    reference_count_type = llvm::Type::getInt32Ty(m.getContext());

    std::array<llvm::Type*, 3> character_type_fields = {
        reference_count_type,
        llvm::Type::getInt64Ty(m.getContext()),
        llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(m.getContext())),
    };
    auto character_type_struct = llvm::StructType::create(character_type_fields);
    character_type = llvm::PointerType::getUnqual(character_type_struct);
}

void dc::llvm_retain(llvm_context& c, llvm::IRBuilder<>& ib, type* t, llvm::Value* v)
{
    if (t->kind() != type_kind::reference)
    {
        return;
    }

    auto zero = llvm::ConstantInt::get(llvm::Type::getInt64Ty(c.m->getContext()), 0);
    auto reference_count = ib.CreateInBoundsGEP(v, {zero});
    ib.CreateAtomicRMW(
        llvm::AtomicRMWInst::BinOp::Add,
        reference_count,
        llvm::ConstantInt::get(c.reference_count_type, 1),
        llvm::AtomicOrdering::SequentiallyConsistent
    );
}

void dc::llvm_release(llvm_context& c, llvm::IRBuilder<>& ib, type* t, llvm::Value* v)
{
    if (t->kind() != type_kind::reference)
    {
        return;
    }

    (void)c;
    (void)ib;
    (void)v;
}
