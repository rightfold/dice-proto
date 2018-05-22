#pragma once

#include "lex.hxx"
#include "node.hxx"
#include "soloc.hxx"

#include <stdexcept>
#include <utility>
#include <vector>

namespace dc
{
    class parser
    {
    public:
        explicit parser(lexer& l);

        dc::source_unit* source_unit();

        bool is_subroutine_parameter() const;
        std::vector<dc::subroutine_parameter> subroutine_parameter_list();
        dc::subroutine_parameter subroutine_parameter();

        bool is_definition() const;
        bool is_function_definition() const;
        bool is_procedure_definition() const;
        dc::definition* definition();
        dc::subroutine_definition* function_definition();
        dc::subroutine_definition* procedure_definition();
        dc::subroutine_definition* subroutine_definition(subroutine_kind kind);

        bool is_statement() const;
        bool is_return_statement() const;
        dc::statement* statement();
        dc::return_statement* return_statement();

        dc::expression* expression();
        dc::variable_expression* variable_expression();

        dc::type* type();

        lexer* l;
    };

    struct parse_error : std::runtime_error
    {
        explicit parse_error(soloc current);

        soloc current;
    };
}
