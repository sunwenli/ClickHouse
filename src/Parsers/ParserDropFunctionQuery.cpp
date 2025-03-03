#include <Parsers/ASTDropFunctionQuery.h>
#include <Parsers/ASTIdentifier.h>
#include <Parsers/CommonParsers.h>
#include <Parsers/ExpressionElementParsers.h>
#include <Parsers/ParserDropFunctionQuery.h>

namespace DB
{

bool ParserDropFunctionQuery::parseImpl(IParser::Pos & pos, ASTPtr & node, Expected & expected)
{
    ParserKeyword s_drop("DROP");
    ParserKeyword s_function("FUNCTION");
    ParserKeyword s_if_exists("IF EXISTS");

    ParserIdentifier function_name_p;
    bool if_exists = false;

    ASTPtr function_name;

    if (!s_drop.ignore(pos, expected))
        return false;

    if (!s_function.ignore(pos, expected))
        return false;

    if (s_if_exists.ignore(pos, expected))
        if_exists = true;

    if (!function_name_p.parse(pos, function_name, expected))
        return false;

    auto drop_function_query = std::make_shared<ASTDropFunctionQuery>();
    drop_function_query->if_exists = if_exists;
    node = drop_function_query;

    drop_function_query->function_name = function_name->as<ASTIdentifier &>().name();

    return true;
}

}
