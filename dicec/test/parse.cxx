#include "../src/lex.hxx"
#include "../src/node.hxx"
#include "../src/parse.hxx"

#include <catch.hpp>

#include <string>

TEST_CASE("parse empty source", "[parse]")
{
    std::string source("");
    dc::lexer lexer(&*source.begin(), &*source.end());
    dc::parser parser(lexer);

    auto source_unit = parser.source_unit();

    REQUIRE(source_unit->definitions.size() == 0);
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
    dc::parser parser(lexer);

    auto source_unit = parser.source_unit();

    REQUIRE(source_unit->definitions.size() == 3);

    REQUIRE(dynamic_cast<dc::subroutine_definition*>(source_unit->definitions[0]));
    REQUIRE(dynamic_cast<dc::subroutine_definition*>(source_unit->definitions[1]));
    REQUIRE(dynamic_cast<dc::subroutine_definition*>(source_unit->definitions[2]));
}
