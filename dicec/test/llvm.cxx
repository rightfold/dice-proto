#include "../src/lex.hxx"
#include "../src/parse.hxx"
#include "../src/scope.hxx"

#include <catch.hpp>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>

#include <string>

TEST_CASE("llvm works", "[llvm]")
{
    std::string source(R"(
        FUNCTION square(x INT, y INT) INT.
            RETURN x.
        END FUNCTION.
    )");
    dc::lexer lexer(&*source.begin(), &*source.end());
    dc::parser parser(lexer);

    auto source_unit = parser.source_unit();

    llvm::LLVMContext llvm_context;
    llvm::Module module("", llvm_context);
    dc::llvm_context context{&module};

    dc::scope<llvm::Function*> scope;
    source_unit->llvm_declare(context, scope);
    source_unit->llvm_define(context, scope);

    llvm::outs() << source;
    module.print(llvm::outs(), nullptr);
}
