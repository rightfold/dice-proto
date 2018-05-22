#include "../llvm.hxx"
#include "../scope.hxx"
#include "../soloc.hxx"
#include "definition.hxx"
#include "statement.hxx"
#include "type.hxx"

#include <llvm/ADT/SmallVector.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

#include <algorithm>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

dc::subroutine_definition::subroutine_definition(
    soloc begin,
    subroutine_kind kind,
    std::string name,
    std::vector<subroutine_parameter> parameters,
    type* return_type,
    std::vector<statement*> body
)
    : definition(begin)
    , kind(kind)
    , name(std::move(name))
    , parameters(std::move(parameters))
    , return_type(return_type)
    , body(std::move(body))
{
}

llvm::FunctionType* dc::subroutine_definition::llvm_function_type(llvm_context& c) const
{
    llvm::SmallVector<llvm::Type*, 8> param_types;
    std::transform(
        parameters.begin(), parameters.end(),
        std::back_inserter(param_types),
        [&] (auto&& param)
        {
            return param.type->llvm_type(c);
        }
    );

    return llvm::FunctionType::get(
        return_type->llvm_type(c),
        param_types,
        /* isVarArg */ false
    );
}

void dc::subroutine_definition::llvm_declare(llvm_context& c, scope<llvm::Function*>& global) const
{
    auto llfun = llvm::Function::Create(
        llvm_function_type(c),
        llvm::Function::ExternalLinkage,
        name,
        c.m
    );
    global.declare(name, llfun);
}

void dc::subroutine_definition::llvm_define(llvm_context& c, scope<llvm::Function*> const& global) const
{
    auto llfun = global.find(name);

    llvm::IRBuilder<> ib(c.m->getContext());

    // Create the entry basic block. This block will be visited first when the
    // subroutine is called. Position the IR builder at this entry.
    auto entry = llvm::BasicBlock::Create(c.m->getContext(), "entry", llfun);
    ib.SetInsertPoint(entry);

    // Collect the subroutine parameters into the local scope.
    scope<llvm::Value*> local(global);
    for (auto& llarg : llfun->args())
    {
        auto parameter = parameters[llarg.getArgNo()];
        local.declare(parameter.name, &llarg);
    }

    // Generate the body of the function.
    for (auto&& stmt : body)
    {
        stmt->llvm_generate(c, ib, local);
    }
}
