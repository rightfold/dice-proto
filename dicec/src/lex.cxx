#include "lex.hxx"
#include "soloc.hxx"

#include <stdexcept>
#include <string>
#include <utility>

dc::lexer::lexer(char* begin, char* end)
    : begin(begin)
    , end(end)
{
}

dc::lexeme dc::lexer::peek() const
{
    auto copy = *this;
    return copy.read();
}

dc::lexeme dc::lexer::read()
{
    skip_whitespace();

    auto c = peek_char();

    if (c == -1)
    {
        return {current, lexeme_type::eof, ""};
    }

    if (is_identifier_head(c))
    {
        return read_identifier_or_keyword();
    }

    switch (c)
    {
#       define DC_LEX_PUNCTUATION_1(pu, ty)                              \
            do                                                           \
            {                                                            \
                case pu:                                                 \
                    read_char();                                         \
                    return {current, lexeme_type::ty, ""};               \
                    break;                                               \
            }                                                            \
            while (false)

#       define DC_LEX_PUNCTUATION_N(pu, body)                            \
            do                                                           \
            {                                                            \
                case pu:                                                 \
                    read_char();                                         \
                    c = peek_char();                                     \
                    switch (c)                                           \
                    body                                                 \
            }                                                            \
            while (false)

        DC_LEX_PUNCTUATION_1(',', pu_comma);
        DC_LEX_PUNCTUATION_1('(', pu_left_parenthesis);
        DC_LEX_PUNCTUATION_1('.', pu_period);
        DC_LEX_PUNCTUATION_1(')', pu_right_parenthesis);

        DC_LEX_PUNCTUATION_N(':', {
            DC_LEX_PUNCTUATION_1(':', pu_colon_colon);
        });

#       undef DC_LEX_PUNCTUATION_2
#       undef DC_LEX_PUNCTUATION_1
    }

    if (c == '\'')
    {
        return read_string_literal();
    }

    if (is_digit(c))
    {
        return read_numeric_literal();
    }

    throw lex_error(current);
}

void dc::lexer::skip_whitespace()
{
    for (;;)
    {
        auto c = peek_char();
        if (!is_space(c))
        {
            break;
        }
        read_char();
    }
}

dc::lexeme dc::lexer::read_identifier_or_keyword()
{
    std::string name;

    for (;;)
    {
        auto c = peek_char();
        if (!is_identifier_tail(c))
        {
            break;
        }
        read_char();
        name.push_back(c);
    }

#   define DC_LEX_KEYWORD(kw, ty)                                        \
        do                                                               \
        {                                                                \
            if (name == kw)                                              \
            {                                                            \
                return {current, lexeme_type::ty, ""};                   \
            }                                                            \
        }                                                                \
        while (false)

    DC_LEX_KEYWORD("END", kw_end);
    DC_LEX_KEYWORD("FUNCTION", kw_function);
    DC_LEX_KEYWORD("INT", kw_int);
    DC_LEX_KEYWORD("PROCEDURE", kw_procedure);
    DC_LEX_KEYWORD("RETURN", kw_return);
    DC_LEX_KEYWORD("USING", kw_using);

#   undef DC_LEX_KEYWORD

    return {current, lexeme_type::identifier, std::move(name)};
}

dc::lexeme dc::lexer::read_string_literal()
{
    std::string value;
    read_char();
    while (peek_char() != '\'')
    {
        value.push_back(read_char());
    }
    read_char();
    return {current, lexeme_type::string_literal, std::move(value)};
}

dc::lexeme dc::lexer::read_numeric_literal()
{
    std::string value;
    while (is_digit(peek_char()))
    {
        value.push_back(read_char());
    }
    return {current, lexeme_type::int_literal, std::move(value)};
}

char dc::lexer::peek_char() const
{
    auto copy = *this;
    return copy.read_char();
}

char dc::lexer::read_char()
{
    if (begin == end)
    {
        return -1;
    }
    else
    {
        auto c = *begin;
        ++begin;
        current.advance(c);
        return c;
    }
}

bool dc::lexer::is_space(char c)
{
    return c == ' ' || c == '\n';
}

bool dc::lexer::is_identifier_head(char c)
{
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || c == '_';
}

bool dc::lexer::is_identifier_tail(char c)
{
    return is_identifier_head(c)
        || is_digit(c)
        || c == '!' || c == '?';
}

bool dc::lexer::is_digit(char c)
{
    return (c >= '0' && c <= '9');
}

dc::lex_error::lex_error(soloc current)
    : std::runtime_error::runtime_error("Lex error")
    , current(current)
{
}
