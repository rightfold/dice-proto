#include "../llvm.hxx"
#include "../soloc.hxx"
#include "type.hxx"

#include <llvm/IR/Type.h>

dc::int_type::int_type(soloc begin)
    : type(begin)
{
}

llvm::Type* dc::int_type::llvm_type(llvm_context& c) const
{
    return llvm::Type::getInt32Ty(c.m->getContext());
}
