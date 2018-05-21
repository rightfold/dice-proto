#pragma once

#include "soloc.hxx"

#include <cassert>
#include <string>
#include <vector>

namespace dc
{
    enum class node_type
    {
        source_unit,

        parameter_list,
        parameter,

        function_definition,
        procedure_definition,

        statement_list,
        return_statement,

        variable_expression,

        int_type,
    };

    struct node
    {
        soloc begin;
        node_type type;
        std::string string_value;
        std::vector<node> children;

#       define DC_NODE_GETTER(ty, na, i)                                 \
            inline node const& ty##_##na() const                         \
            {                                                            \
                assert(type == node_type::ty);                           \
                return children[i];                                      \
            }

        DC_NODE_GETTER(parameter, type, 0);

        DC_NODE_GETTER(function_definition, parameters, 0);
        DC_NODE_GETTER(function_definition, return_type, 1);
        DC_NODE_GETTER(function_definition, body, 2);

        DC_NODE_GETTER(return_statement, value, 0);

#       undef DC_NODE_GETTER

        static node make_source_unit(soloc begin, std::vector<node> definitions);

        static node make_parameter_list(soloc begin, std::vector<node> parameters);
        static node make_parameter(soloc begin, std::string name, node type);

        static node make_function_definition(soloc begin, std::string name, node parameters, node return_type, node body);
        static node make_procedure_definition(soloc begin, std::string name, node parameters, node return_type, node body);

        static node make_statement_list(soloc begin, std::vector<node> statements);
        static node make_return_statement(soloc begin, node value);

        static node make_variable_expression(soloc begin, std::string name);

        static node make_int_type(soloc begin);
    };
}
