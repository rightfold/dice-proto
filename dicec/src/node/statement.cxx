#include "../llvm.hxx"
#include "../soloc.hxx"
#include "expression.hxx"
#include "statement.hxx"

#include <llvm/IR/IRBuilder.h>

dc::return_statement::return_statement(soloc begin, expression* value)
    : statement(begin)
    , value(value)
{
}

void dc::return_statement::llvm_generate(
    llvm_context& c,
    llvm::IRBuilder<>& ib,
    scope<llvm::Value*>& local
) const
{
    auto llvalue = value->llvm_generate(c, ib, local);
    ib.CreateRet(llvalue);
}
