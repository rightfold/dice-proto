#pragma once

#include "../llvm.hxx"
#include "../scope.hxx"
#include "../soloc.hxx"
#include "base.hxx"
#include "definition.hxx"

#include <llvm/IR/Function.h>

#include <vector>

namespace dc
{
    class source_unit : public node
    {
    public:
        source_unit(soloc begin, std::vector<definition*> definitions);

        std::vector<definition*> definitions;

        void llvm_declare(llvm_context& m, scope<llvm::Function*>& global) const;
        void llvm_define(llvm_context& m, scope<llvm::Function*> const& global) const;
    };
}
