#pragma once

#include "../llvm.hxx"
#include "../scope.hxx"
#include "../soloc.hxx"
#include "base.hxx"
#include "statement.hxx"
#include "type.hxx"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>

#include <string>
#include <vector>

namespace dc
{
    enum class subroutine_kind
    {
        function,
        procedure,
    };

    struct subroutine_parameter
    {
        soloc begin;
        std::string name;
        dc::type* type;
    };

    class definition : public node
    {
    public:
        using node::node;

        virtual void llvm_declare(llvm_context& c, scope<llvm::Function*>& s) const = 0;
        virtual void llvm_define(llvm_context& c, scope<llvm::Function*> const& s) const = 0;
    };

    class subroutine_definition : public definition
    {
    public:
        subroutine_definition(
            soloc begin,
            subroutine_kind kind,
            std::string name,
            std::vector<subroutine_parameter> parameters,
            type* return_type,
            std::vector<statement*> body
        );

        subroutine_kind kind;
        std::string name;
        std::vector<subroutine_parameter> parameters;
        type* return_type;
        std::vector<statement*> body;

        llvm::FunctionType* llvm_function_type(llvm_context& c) const;
        void llvm_declare(llvm_context& c, scope<llvm::Function*>& global) const override;
        void llvm_define(llvm_context& c, scope<llvm::Function*> const& global) const override;
    };
}
