#include "../src/lex.hxx"
#include "../src/node.hxx"
#include "../src/parse.hxx"

#include <catch.hpp>

#include <string>

TEST_CASE("parse empty source", "[parse]")
{
    std::string source("");
    dc::lexer lexer(&*source.begin(), &*source.end());

    auto source_unit = dc::parse::source_unit(lexer);

    REQUIRE(source_unit.type == dc::node_type::source_unit);
    REQUIRE(source_unit.children.size() == 0);
}

TEST_CASE("parse function and procedure definitions", "[parse]")
{
    std::string source(R"(
        FUNCTION add(x INT, y INT) INT.
        END FUNCTION.

        FUNCTION square(x INT) INT.
        END FUNCTION.

        PROCEDURE main() INT.
        END PROCEDURE.
    )");
    dc::lexer lexer(&*source.begin(), &*source.end());

    auto source_unit = dc::parse::source_unit(lexer);

    REQUIRE(source_unit.type == dc::node_type::source_unit);
    REQUIRE(source_unit.children.size() == 3);

    auto const& add = source_unit.children[0];
    REQUIRE(add.type == dc::node_type::function_definition);
    REQUIRE(add.children.size() == 2);

    auto const& parameter_list = add.children[0];
    REQUIRE(parameter_list.type == dc::node_type::parameter_list);
    REQUIRE(parameter_list.children.size() == 2);

    auto const& parameter = parameter_list.children[0];
    REQUIRE(parameter.type == dc::node_type::parameter);
    REQUIRE(parameter.string_value == "x");

    auto const& main = source_unit.children[2];
    REQUIRE(main.type == dc::node_type::procedure_definition);
    REQUIRE(main.children.size() == 2);
}
