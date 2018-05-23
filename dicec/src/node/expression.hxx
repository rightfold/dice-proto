#pragma once

#include "../llvm.hxx"
#include "../scope.hxx"
#include "../soloc.hxx"
#include "base.hxx"
#include "type.hxx"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

#include <string>

namespace dc
{
    class expression : public node
    {
    public:
        explicit expression(soloc begin);

        dc::type* type;

        virtual llvm::Value* llvm_generate(
            llvm_context& c,
            llvm::IRBuilder<>& ib,
            scope<llvm::Value*>& local
        ) const = 0;
    };

    class variable_expression : public expression
    {
    public:
        variable_expression(soloc begin, std::string name);

        std::string name;

        llvm::Value* llvm_generate(
            llvm_context& c,
            llvm::IRBuilder<>& ib,
            scope<llvm::Value*>& local
        ) const override;
    };
}
