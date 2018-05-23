#include "lex.hxx"
#include "node.hxx"
#include "parse.hxx"
#include "soloc.hxx"

#include <cassert>
#include <stdexcept>
#include <utility>
#include <vector>

// FIXME: Currently the parser uses _new_ to allocate memory. This is very bad
// and results in memory leaks. Instead, the _parser_ class should contain a
// non-owning pointer to an arena, in which to allocate memory for the nodes.
// However, I have no yet found a convincing polymorphic arena allocator that
// fits the needs, so I may have to write one myself.

namespace
{
    dc::lexeme expect(dc::parser& p, dc::lexeme_type ty)
    {
        auto lm = p.l->read();
        if (lm.type != ty)
        {
            throw dc::parse_error(lm.begin);
        }
        return lm;
    }

    template<typename F, typename G>
    auto many(F&& is_rule, G&& rule) -> std::vector<decltype(rule())>
    {
        std::vector<decltype(rule())> nodes;
        while (is_rule())
        {
            nodes.push_back(rule());
        }
        return nodes;
    }

    template<typename F>
    auto separated_1(
        dc::parser& p,
        dc::lexeme_type by,
        F&& rule
    ) -> std::vector<decltype(rule())>
    {
        std::vector<decltype(rule())> nodes;
        for (;;)
        {
            nodes.push_back(rule());
            if (p.l->peek().type == by)
            {
                p.l->read();
            }
            else
            {
                break;
            }
        }
        return nodes;
    }

    template<typename F, typename G>
    auto separated(
        dc::parser& p,
        dc::lexeme_type by,
        F&& is_rule,
        G&& rule
    ) -> std::vector<decltype(rule())>
    {
        if (!is_rule())
        {
            return {};
        }
        return separated_1(p, by, std::forward<G>(rule));
    }
}

dc::parser::parser(lexer& l)
    : l(&l)
{
}

dc::source_unit* dc::parser::source_unit()
{
    auto definitions = many(
        [this] { return is_definition(); },
        [this] { return definition(); }
    );
    expect(*this, lexeme_type::eof);
    return new dc::source_unit(soloc{0, 0}, std::move(definitions));
}

bool dc::parser::is_subroutine_parameter() const
{
    return l->peek().type == lexeme_type::identifier;
}

std::vector<dc::subroutine_parameter> dc::parser::subroutine_parameter_list()
{
    expect(*this, lexeme_type::pu_left_parenthesis);
    auto parameters = separated(
        *this,
        lexeme_type::pu_comma,
        [this] { return is_subroutine_parameter(); },
        [this] { return subroutine_parameter(); }
    );
    expect(*this, lexeme_type::pu_right_parenthesis).begin;
    return parameters;
}

dc::subroutine_parameter dc::parser::subroutine_parameter()
{
    auto name_lm = expect(*this, lexeme_type::identifier);
    auto parameter_type = type();
    return dc::subroutine_parameter{
        name_lm.begin,
        std::move(name_lm.value),
        parameter_type,
    };
}

bool dc::parser::is_definition() const
{
    return is_function_definition()
        || is_procedure_definition();
}

bool dc::parser::is_function_definition() const
{
    return l->peek().type == lexeme_type::kw_function;
}

bool dc::parser::is_procedure_definition() const
{
    return l->peek().type == lexeme_type::kw_procedure;
}

dc::definition* dc::parser::definition()
{
    if (is_function_definition())
    {
        return function_definition();
    }

    if (is_procedure_definition())
    {
        return procedure_definition();
    }

    throw parse_error(l->peek().begin);
}

dc::subroutine_definition* dc::parser::function_definition()
{
    return subroutine_definition(subroutine_kind::function);
}

dc::subroutine_definition* dc::parser::procedure_definition()
{
    return subroutine_definition(subroutine_kind::procedure);
}

dc::subroutine_definition* dc::parser::subroutine_definition(subroutine_kind kind)
{
    lexeme_type keyword;
    switch (kind)
    {
    case subroutine_kind::function: keyword = lexeme_type::kw_function; break;
    case subroutine_kind::procedure: keyword = lexeme_type::kw_procedure; break;
    default: assert(false);
    }

    auto begin = expect(*this, keyword).begin;
    auto name = expect(*this, lexeme_type::identifier).value;
    auto parameters = subroutine_parameter_list();
    auto return_type = type();
    expect(*this, lexeme_type::pu_period);

    auto body = many(
        [this] { return is_statement(); },
        [this] { return statement(); }
    );

    expect(*this, lexeme_type::kw_end);
    expect(*this, keyword);
    expect(*this, lexeme_type::pu_period);

    return new dc::subroutine_definition(
        begin,
        kind,
        std::move(name),
        std::move(parameters),
        return_type,
        std::move(body)
    );
}

bool dc::parser::is_statement() const
{
    return is_return_statement();
}

bool dc::parser::is_return_statement() const
{
    return l->peek().type == lexeme_type::kw_return;
}

dc::statement* dc::parser::statement()
{
    if (is_return_statement())
    {
        return return_statement();
    }

    throw parse_error(l->peek().begin);
}

dc::return_statement* dc::parser::return_statement()
{
    auto begin = expect(*this, lexeme_type::kw_return).begin;
    auto value = expression();
    expect(*this, lexeme_type::pu_period);
    return new dc::return_statement(begin, value);
}

dc::expression* dc::parser::expression()
{
    return variable_expression();
}

dc::variable_expression* dc::parser::variable_expression()
{
    auto lm = expect(*this, lexeme_type::identifier);
    auto e = new dc::variable_expression(lm.begin, lm.value);

    // TODO: Remove this. It was for testing code generation prior to having a
    // type checker.
    if (lm.value == "x")
    {
        e->type = new dc::character_type(lm.begin);
    }
    else
    {
        e->type = new dc::int_type(lm.begin);
    }

    return e;
}

dc::type* dc::parser::type()
{
    if (l->peek().type == lexeme_type::kw_character)
    {
        auto begin = expect(*this, lexeme_type::kw_character).begin;
        return new dc::character_type(begin);
    }

    auto begin = expect(*this, lexeme_type::kw_int).begin;
    return new dc::int_type(begin);
}

dc::parse_error::parse_error(soloc current)
    : std::runtime_error::runtime_error("Parse error")
    , current(current)
{
}
