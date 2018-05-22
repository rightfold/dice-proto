#pragma once

#include "../llvm.hxx"
#include "../scope.hxx"
#include "../soloc.hxx"
#include "base.hxx"
#include "expression.hxx"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

namespace dc
{
    class statement : public node
    {
    public:
        using node::node;

        virtual void llvm_generate(
            llvm_context& c,
            llvm::IRBuilder<>& ib,
            scope<llvm::Value*>& local
        ) const = 0;
    };

    class return_statement : public statement
    {
    public:
        return_statement(soloc begin, expression* value);

        expression* value;

        void llvm_generate(
            llvm_context& c,
            llvm::IRBuilder<>& ib,
            scope<llvm::Value*>& local
        ) const override;
    };
}
