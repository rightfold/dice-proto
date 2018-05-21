#pragma once

#include "node.hxx"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

namespace dc
{
    namespace llvm
    {
        void convert_source_unit(::llvm::Module* c, node const& n);

        ::llvm::SmallVector<::llvm::Type*, 8> get_parameter_types(::llvm::Module* c, node const& n);

        ::llvm::Function* declare_function_definition(::llvm::Module* c, node const& n);
        void define_function_definition(::llvm::Module* c, ::llvm::Function* f, node const& n);

        void generate_statement(
            ::llvm::Module* c,
            ::llvm::IRBuilder<>& b,
            ::llvm::StringMap<::llvm::Value*> const& e,
            node const& n
        );

        ::llvm::Value* generate_expression(
            ::llvm::Module* c,
            ::llvm::IRBuilder<>& b,
            ::llvm::StringMap<::llvm::Value*> const& e,
            node const& n
        );

        ::llvm::Type* get_type(::llvm::Module* c, node const& n);
    }
}
