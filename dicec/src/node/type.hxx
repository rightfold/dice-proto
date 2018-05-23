#pragma once

#include "../llvm.hxx"
#include "../soloc.hxx"
#include "base.hxx"

#include <llvm/IR/Type.h>

namespace dc
{
    enum class type_kind
    {
        value,
        reference,
    };

    class type : public node
    {
    public:
        using node::node;

        virtual type_kind kind() const = 0;

        virtual llvm::Type* llvm_type(llvm_context& c) const = 0;
    };

    class int_type : public type
    {
    public:
        explicit int_type(soloc begin);

        type_kind kind() const override;

        llvm::Type* llvm_type(llvm_context& c) const override;
    };

    class character_type : public type
    {
    public:
        explicit character_type(soloc begin);

        type_kind kind() const override;

        llvm::Type* llvm_type(llvm_context& c) const override;
    };
}
