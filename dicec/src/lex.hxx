#pragma once

#include "soloc.hxx"

#include <stdexcept>
#include <string>

namespace dc
{
    enum class lexeme_type
    {
        eof,

        pu_comma,
        pu_left_parenthesis,
        pu_period,
        pu_right_parenthesis,

        pu_colon_colon,

        kw_character,
        kw_end,
        kw_function,
        kw_int,
        kw_procedure,
        kw_return,
        kw_using,

        identifier,

        string_literal,
        int_literal,
    };

    struct lexeme
    {
        soloc begin;
        lexeme_type type;
        std::string value;
    };

    class lexer
    {
    public:
        explicit lexer(char* begin, char* end);

        lexeme peek() const;
        lexeme read();

    private:
        void skip_whitespace();
        lexeme read_identifier_or_keyword();
        lexeme read_string_literal();
        lexeme read_numeric_literal();

        char peek_char() const;
        char read_char();

        static bool is_space(char c);
        static bool is_identifier_head(char c);
        static bool is_identifier_tail(char c);
        static bool is_digit(char c);

        char* begin;
        char* end;
        soloc current;
    };

    struct lex_error : std::runtime_error
    {
        explicit lex_error(soloc current);

        soloc current;
    };
}
