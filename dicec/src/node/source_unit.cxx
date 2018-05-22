#include "../llvm.hxx"
#include "../scope.hxx"
#include "../soloc.hxx"
#include "definition.hxx"
#include "source_unit.hxx"

#include <llvm/IR/Function.h>

#include <utility>
#include <vector>

dc::source_unit::source_unit(soloc begin, std::vector<definition*> definitions)
    : node(begin)
    , definitions(std::move(definitions))
{
}

void dc::source_unit::llvm_declare(llvm_context& m, scope<llvm::Function*>& global) const
{
    for (auto&& d : definitions)
    {
        d->llvm_declare(m, global);
    }
}

void dc::source_unit::llvm_define(llvm_context& m, scope<llvm::Function*> const& global) const
{
    for (auto&& def : definitions)
    {
        def->llvm_define(m, global);
    }
}
