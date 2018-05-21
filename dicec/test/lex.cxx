#include "../src/lex.hxx"

#include <catch.hpp>

#include <string>

TEST_CASE("lex empty source", "[lex]")
{
    std::string source("");
    dc::lexer lexer(&*source.begin(), &*source.end());

    auto lexeme = lexer.read();
    REQUIRE(lexeme.type == dc::lexeme_type::eof);
}

TEST_CASE("lex identifier", "[lex]")
{
    std::string source("foo");
    dc::lexer lexer(&*source.begin(), &*source.end());

    auto lexeme = lexer.read();
    REQUIRE(lexeme.type == dc::lexeme_type::identifier);
    REQUIRE(lexeme.value == "foo");
}

TEST_CASE("lex keyword", "[lex]")
{
    std::string source("FUNCTION");
    dc::lexer lexer(&*source.begin(), &*source.end());

    auto lexeme = lexer.read();
    REQUIRE(lexeme.type == dc::lexeme_type::kw_function);
}

TEST_CASE("lex 'Hello, world!'", "[lex]")
{
    std::string source(R"(
        USING std::io.

        PROCEDURE main(stdout io::Writer) INT.
            io::Write(stdout, 'Hello, world!').
            RETURN 0.
        END PROCEDURE.
    )");
    dc::lexer lexer(&*source.begin(), &*source.end());

    REQUIRE(lexer.read().type == dc::lexeme_type::kw_using);
    REQUIRE(lexer.read().type == dc::lexeme_type::identifier);
    REQUIRE(lexer.read().type == dc::lexeme_type::pu_colon_colon);
    REQUIRE(lexer.read().type == dc::lexeme_type::identifier);
    REQUIRE(lexer.read().type == dc::lexeme_type::pu_period);

    REQUIRE(lexer.read().type == dc::lexeme_type::kw_procedure);
    REQUIRE(lexer.read().type == dc::lexeme_type::identifier);
    REQUIRE(lexer.read().type == dc::lexeme_type::pu_left_parenthesis);
    REQUIRE(lexer.read().type == dc::lexeme_type::identifier);
    REQUIRE(lexer.read().type == dc::lexeme_type::identifier);
    REQUIRE(lexer.read().type == dc::lexeme_type::pu_colon_colon);
    REQUIRE(lexer.read().type == dc::lexeme_type::identifier);
    REQUIRE(lexer.read().type == dc::lexeme_type::pu_right_parenthesis);
    REQUIRE(lexer.read().type == dc::lexeme_type::kw_int);
    REQUIRE(lexer.read().type == dc::lexeme_type::pu_period);

    REQUIRE(lexer.read().type == dc::lexeme_type::identifier);
    REQUIRE(lexer.read().type == dc::lexeme_type::pu_colon_colon);
    REQUIRE(lexer.read().type == dc::lexeme_type::identifier);
    REQUIRE(lexer.read().type == dc::lexeme_type::pu_left_parenthesis);
    REQUIRE(lexer.read().type == dc::lexeme_type::identifier);
    REQUIRE(lexer.read().type == dc::lexeme_type::pu_comma);
    REQUIRE(lexer.read().type == dc::lexeme_type::string_literal);
    REQUIRE(lexer.read().type == dc::lexeme_type::pu_right_parenthesis);
    REQUIRE(lexer.read().type == dc::lexeme_type::pu_period);

    REQUIRE(lexer.read().type == dc::lexeme_type::kw_return);
    REQUIRE(lexer.read().type == dc::lexeme_type::int_literal);
    REQUIRE(lexer.read().type == dc::lexeme_type::pu_period);

    REQUIRE(lexer.read().type == dc::lexeme_type::kw_end);
    REQUIRE(lexer.read().type == dc::lexeme_type::kw_procedure);
    REQUIRE(lexer.read().type == dc::lexeme_type::pu_period);

    REQUIRE(lexer.read().type == dc::lexeme_type::eof);
}
