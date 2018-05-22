#pragma once

#include "../llvm.hxx"
#include "../soloc.hxx"
#include "base.hxx"

#include <llvm/IR/Type.h>

namespace dc
{
    class type : public node
    {
    public:
        using node::node;

        virtual llvm::Type* llvm_type(llvm_context& c) const = 0;
    };

    class int_type : public type
    {
    public:
        explicit int_type(soloc begin);

        llvm::Type* llvm_type(llvm_context& c) const override;
    };
}
