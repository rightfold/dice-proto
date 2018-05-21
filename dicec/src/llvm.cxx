#include "llvm.hxx"
#include "node.hxx"

#include <llvm/ADT/StringMap.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

#include <cassert>

void dc::llvm::convert_source_unit(::llvm::Module* c, node const& n)
{
    assert(n.type == node_type::source_unit);

    // Map Dice function names to LLVM functions.
    ::llvm::StringMap<::llvm::Function*> functions;

    // First, declare all the functions. This allows them to be called before
    // they are defined.
    for (auto&& d : n.children)
    {
        switch (d.type)
        {
        case node_type::function_definition:
            {
                auto f = declare_function_definition(c, d);
                functions.try_emplace(d.string_value, f);
                break;
            }

        default:
            assert(false);
        }
    }

    // Then, define the functions by giving them bodies.
    for (auto&& d : n.children)
    {
        switch (d.type)
        {
        case node_type::function_definition:
            {
                assert(functions.count(d.string_value));
                auto f = functions.lookup(d.string_value);
                define_function_definition(c, f, d);
                break;
            }

        default:
            assert(false);
        }
    }
}

::llvm::SmallVector<::llvm::Type*, 8> dc::llvm::get_parameter_types(::llvm::Module* c, node const& n)
{
    assert(n.type == node_type::parameter_list);

    ::llvm::SmallVector<::llvm::Type*, 8> result;
    result.reserve(n.children.size());

    // Collect the LLVM parameter types so that they can be used in an LLVM
    // function type.
    for (auto&& p : n.children)
    {
        auto t = get_type(c, p.parameter_type());
        result.push_back(t);
    }

    return result;
}

llvm::Function* dc::llvm::declare_function_definition(::llvm::Module* c, node const& n)
{
    assert(n.type == node_type::function_definition);

    // Create the function type.
    auto function_type = ::llvm::FunctionType::get(
        get_type(c, n.function_definition_return_type()),
        get_parameter_types(c, n.function_definition_parameters()),
        false
    );

    // Declare the function.
    auto function = ::llvm::Function::Create(
        function_type,
        ::llvm::Function::ExternalLinkage,
        n.string_value,
        c
    );

    return function;
}

void dc::llvm::define_function_definition(::llvm::Module* c, ::llvm::Function* f, node const& n)
{
    ::llvm::IRBuilder<> builder(c->getContext());

    // Create the basic block that is entered when the subroutine is called.
    auto entry = ::llvm::BasicBlock::Create(c->getContext(), "entry", f);
    builder.SetInsertPoint(entry);

    // Collect the parameters into the environment.
    ::llvm::StringMap<::llvm::Value*> e;
    auto&& ps = n.function_definition_parameters().children;
    for (auto&& arg : f->args())
    {
        e.try_emplace(ps[arg.getArgNo()].string_value, &arg);
    }

    // Generate the body of the function.
    for (auto&& s : n.function_definition_body().children)
    {
        generate_statement(c, builder, e, s);
    }
}

void dc::llvm::generate_statement(
    ::llvm::Module* c,
    ::llvm::IRBuilder<>& b,
    ::llvm::StringMap<::llvm::Value*> const& e,
    node const& n
)
{
    switch (n.type)
    {
    case node_type::return_statement:
        {
            auto value = generate_expression(c, b, e, n.return_statement_value());
            b.CreateRet(value);
            return;
        }

    default:
        assert(false);
    }
}

llvm::Value* dc::llvm::generate_expression(
    ::llvm::Module*,
    ::llvm::IRBuilder<>&,
    ::llvm::StringMap<::llvm::Value*> const& e,
    node const& n
)
{
    switch (n.type)
    {
    case node_type::variable_expression:
        assert(e.count(n.string_value));
        return e.lookup(n.string_value);

    default:
        assert(false);
    }
}

llvm::Type* dc::llvm::get_type(::llvm::Module* c, node const& n)
{
    switch (n.type)
    {
    case node_type::int_type:
        return ::llvm::Type::getInt32Ty(c->getContext());

    default:
        assert(false);
    }
}
