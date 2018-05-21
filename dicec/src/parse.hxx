#pragma once

#include "lex.hxx"
#include "node.hxx"
#include "soloc.hxx"

#include <stdexcept>

namespace dc
{
    namespace parse
    {
        node source_unit(lexer& l);

        bool is_parameter(lexer& l);
        node parameter_list(lexer& l);
        node parameter(lexer& l);

        bool is_definition(lexer& l);
        bool is_function_definition(lexer& l);
        bool is_procedure_definition(lexer& l);
        node definition(lexer& l);
        node function_definition(lexer& l);
        node procedure_definition(lexer& l);

        bool is_statement(lexer& l);
        bool is_return_statement(lexer& l);
        node statement(lexer& l);
        node return_statement(lexer& l);

        node expression(lexer& l);
        node variable_expression(lexer& l);

        node type(lexer& l);

        struct error : std::runtime_error
        {
            explicit error(soloc current);

            soloc current;
        };
    }
}
