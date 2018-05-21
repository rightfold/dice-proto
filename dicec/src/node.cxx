#include "node.hxx"
#include "soloc.hxx"

#include <string>
#include <utility>

dc::node dc::node::make_parameter_list(soloc begin, std::vector<node> parameters)
{
    return {begin, node_type::parameter_list, "", std::move(parameters)};
}

dc::node dc::node::make_parameter(soloc begin, std::string name, node type)
{
    return {begin, node_type::parameter, std::move(name), {std::move(type)}};
}

dc::node dc::node::make_statement_list(soloc begin, std::vector<node> statements)
{
    return {begin, node_type::statement_list, "", std::move(statements)};
}

dc::node dc::node::make_function_definition(soloc begin, std::string name, node parameters, node body)
{
    return {begin, node_type::function_definition, std::move(name),
            {std::move(parameters), std::move(body)}};
}

dc::node dc::node::make_procedure_definition(soloc begin, std::string name, node parameters, node body)
{
    return {begin, node_type::procedure_definition, std::move(name),
            {std::move(parameters), std::move(body)}};
}

dc::node dc::node::make_return_statement(soloc begin, node value)
{
    return {begin, node_type::return_statement, "", {std::move(value)}};
}

dc::node dc::node::make_variable_expression(soloc begin, std::string name)
{
    return {begin, node_type::variable_expression, std::move(name), {}};
}

dc::node dc::node::make_int_type(soloc begin)
{
    return {begin, node_type::int_type, "", {}};
}
