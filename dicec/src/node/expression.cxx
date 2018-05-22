#include "../llvm.hxx"
#include "../scope.hxx"
#include "../soloc.hxx"
#include "expression.hxx"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

#include <string>
#include <utility>

dc::variable_expression::variable_expression(soloc begin, std::string name)
    : expression(begin)
    , name(std::move(name))
{
}

llvm::Value* dc::variable_expression::llvm_generate(
    llvm_context&,
    llvm::IRBuilder<>& ib,
    scope<llvm::Value*>& local
) const
{
    auto value = local.find(name);
    llvm_retain(ib, value);
    return value;
}
