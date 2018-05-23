#include "../llvm.hxx"
#include "../soloc.hxx"
#include "type.hxx"

#include <llvm/IR/Type.h>

dc::int_type::int_type(soloc begin)
    : type(begin)
{
}

dc::type_kind dc::int_type::kind() const
{
    return type_kind::value;
}

llvm::Type* dc::int_type::llvm_type(llvm_context& c) const
{
    return llvm::Type::getInt32Ty(c.m->getContext());
}

dc::character_type::character_type(soloc begin)
    : type(begin)
{
}

dc::type_kind dc::character_type::kind() const
{
    return type_kind::reference;
}

llvm::Type* dc::character_type::llvm_type(llvm_context& c) const
{
    return c.character_type;
}
