#include <Access/ContextAccess.h>
#include <Interpreters/Context.h>
#include <Interpreters/FunctionNameNormalizer.h>
#include <Interpreters/InterpreterDropFunctionQuery.h>
#include <Interpreters/UserDefinedSQLObjectsLoader.h>
#include <Interpreters/UserDefinedSQLFunctionFactory.h>
#include <Parsers/ASTDropFunctionQuery.h>


namespace DB
{

BlockIO InterpreterDropFunctionQuery::execute()
{
    auto current_context = getContext();
    current_context->checkAccess(AccessType::DROP_FUNCTION);

    FunctionNameNormalizer().visit(query_ptr.get());
    auto & drop_function_query = query_ptr->as<ASTDropFunctionQuery &>();

    auto & user_defined_functions_factory = UserDefinedSQLFunctionFactory::instance();

    if (drop_function_query.if_exists && !user_defined_functions_factory.has(drop_function_query.function_name))
        return {};

    UserDefinedSQLFunctionFactory::instance().unregisterFunction(drop_function_query.function_name);
    UserDefinedSQLObjectsLoader::instance().removeObject(current_context, UserDefinedSQLObjectType::Function, drop_function_query.function_name);

    return {};
}

}
