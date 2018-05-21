#include "../src/lex.hxx"
#include "../src/llvm.hxx"
#include "../src/parse.hxx"

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
    auto source_unit = dc::parse::source_unit(lexer);

    llvm::LLVMContext context;
    llvm::Module module("", context);

    dc::llvm::convert_source_unit(&module, source_unit);

    llvm::outs() << source;
    module.print(llvm::outs(), nullptr);
}
