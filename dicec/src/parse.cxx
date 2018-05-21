#include "lex.hxx"
#include "node.hxx"
#include "parse.hxx"
#include "soloc.hxx"

#include <stdexcept>
#include <utility>
#include <vector>

namespace
{
    dc::lexeme expect(dc::lexer& l, dc::lexeme_type ty)
    {
        auto lm = l.read();
        if (lm.type != ty)
        {
            throw dc::parse::error(lm.begin);
        }
        return lm;
    }

    template<typename F, typename G>
    std::vector<dc::node> many(dc::lexer& l, F&& is_rule, G&& rule)
    {
        std::vector<dc::node> nodes;
        while (is_rule(l))
        {
            nodes.push_back(rule(l));
        }
        return nodes;
    }

    template<typename F>
    std::vector<dc::node> separated_1(dc::lexer& l, dc::lexeme_type by, F&& rule)
    {
        std::vector<dc::node> nodes;
        for (;;)
        {
            nodes.push_back(rule(l));
            if (l.peek().type == by)
            {
                l.read();
            }
            else
            {
                break;
            }
        }
        return nodes;
    }

    template<typename F, typename G>
    std::vector<dc::node> separated(dc::lexer& l, dc::lexeme_type by, F&& is_rule, G&& rule)
    {
        if (!is_rule(l))
        {
            return {};
        }
        return separated_1(l, by, std::forward<G>(rule));
    }
}

dc::node dc::parse::source_unit(lexer& l)
{
    auto definitions = many(l, &is_definition, &definition);
    expect(l, lexeme_type::eof);
    return dc::node::make_source_unit({0, 0}, definitions);
}

bool dc::parse::is_parameter(lexer& l)
{
    return l.peek().type == lexeme_type::identifier;
}

dc::node dc::parse::parameter_list(lexer& l)
{
    auto begin = expect(l, lexeme_type::pu_left_parenthesis).begin;
    auto parameters = separated(l, lexeme_type::pu_comma, &is_parameter, &parameter);
    expect(l, lexeme_type::pu_right_parenthesis).begin;
    return node::make_parameter_list(begin, parameters);
}

dc::node dc::parse::parameter(lexer& l)
{
    auto name_lm = expect(l, lexeme_type::identifier);
    auto parameter_type = type(l);
    return node::make_parameter(name_lm.begin, name_lm.value, parameter_type);
}

bool dc::parse::is_definition(lexer& l)
{
    return is_function_definition(l)
        || is_procedure_definition(l);
}

bool dc::parse::is_function_definition(lexer& l)
{
    return l.peek().type == lexeme_type::kw_function;
}

bool dc::parse::is_procedure_definition(lexer& l)
{
    return l.peek().type == lexeme_type::kw_procedure;
}

dc::node dc::parse::definition(lexer& l)
{
    if (is_function_definition(l))
    {
        return function_definition(l);
    }

    if (is_procedure_definition(l))
    {
        return procedure_definition(l);
    }

    throw error(l.peek().begin);
}

dc::node dc::parse::function_definition(lexer& l)
{
    auto begin = expect(l, lexeme_type::kw_function).begin;
    auto name = expect(l, lexeme_type::identifier).value;
    auto parameters = parameter_list(l);
    auto return_type = type(l);
    auto body_begin = expect(l, lexeme_type::pu_period).begin;

    auto body = node::make_statement_list(
        body_begin,
        many(l, &is_statement, &statement)
    );

    expect(l, lexeme_type::kw_end);
    expect(l, lexeme_type::kw_function);
    expect(l, lexeme_type::pu_period);

    return node::make_function_definition(begin, name, parameters, return_type, body);
}

dc::node dc::parse::procedure_definition(lexer& l)
{
    auto begin = expect(l, lexeme_type::kw_procedure).begin;
    auto name = expect(l, lexeme_type::identifier).value;
    auto parameters = parameter_list(l);
    auto return_type = type(l);
    expect(l, lexeme_type::pu_period);

    expect(l, lexeme_type::kw_end);
    expect(l, lexeme_type::kw_procedure);
    expect(l, lexeme_type::pu_period);

    return node::make_procedure_definition(begin, name, parameters, return_type, {});
}

bool dc::parse::is_statement(lexer& l)
{
    return is_return_statement(l);
}

bool dc::parse::is_return_statement(lexer& l)
{
    return l.peek().type == lexeme_type::kw_return;
}

dc::node dc::parse::statement(lexer& l)
{
    if (is_return_statement(l))
    {
        return return_statement(l);
    }

    throw error(l.peek().begin);
}

dc::node dc::parse::return_statement(lexer& l)
{
    auto begin = expect(l, lexeme_type::kw_return).begin;
    auto value = expression(l);
    expect(l, lexeme_type::pu_period);
    return node::make_return_statement(begin, value);
}

dc::node dc::parse::expression(lexer& l)
{
    return variable_expression(l);
}

dc::node dc::parse::variable_expression(lexer& l)
{
    auto lm = expect(l, lexeme_type::identifier);
    return node::make_variable_expression(lm.begin, lm.value);
}

dc::node dc::parse::type(lexer& l)
{
    auto begin = expect(l, lexeme_type::kw_int).begin;
    return node::make_int_type(begin);
}

dc::parse::error::error(soloc current)
    : std::runtime_error::runtime_error("Parse error")
    , current(current)
{
}
