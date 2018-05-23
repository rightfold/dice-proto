#include "../src/lex.hxx"
#include "../src/parse.hxx"
#include "../src/scope.hxx"

#include <catch.hpp>

#include <llvm/ADT/Optional.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

#include <string>

TEST_CASE("llvm works", "[llvm]")
{
    std::string source(R"(
        FUNCTION f(x CHARACTER, y INT) CHARACTER.
            RETURN x.
        END FUNCTION.

        FUNCTION g(x CHARACTER, y INT) INT.
            RETURN y.
        END FUNCTION.
    )");
    dc::lexer lexer(&*source.begin(), &*source.end());
    dc::parser parser(lexer);

    auto source_unit = parser.source_unit();

    llvm::LLVMContext llvm_context;
    llvm::Module module("", llvm_context);
    dc::llvm_context context(module);

    dc::scope<llvm::Function*> scope;
    source_unit->llvm_declare(context, scope);
    source_unit->llvm_define(context, scope);

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    auto triple = llvm::sys::getDefaultTargetTriple();
    std::string target_error;
    auto target = llvm::TargetRegistry::lookupTarget(triple, target_error);

    llvm::TargetOptions target_options;
    llvm::Optional<llvm::Reloc::Model> reloc_model;
    auto target_machine = target->createTargetMachine(
        triple,
        /* CPU */ "generic",
        /* features */ "",
        target_options,
        reloc_model
    );

    module.setDataLayout(target_machine->createDataLayout());
    module.setTargetTriple(triple);

    llvm::raw_fd_ostream stdout(1, false);

    llvm::legacy::PassManager pm;
    target_machine->addPassesToEmitFile(
        pm,
        stdout,
        llvm::TargetMachine::CodeGenFileType::CGFT_AssemblyFile
    );

    stdout << source;
    module.print(stdout, nullptr);
    pm.run(module);
}
