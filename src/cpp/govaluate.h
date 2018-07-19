#pragma once
#include "bytes.h"
#include "errors.h"
#include "fmt.h"
#include "math.h"
#include "reflect.h"
#include "regexp.h"
#include "strconv.h"
#include "strings.h"
#include "time.h"
#include "unicode.h"
namespace govaluate {
constexpr OperatorSymbol ACCESS{32};
constexpr TokenKind ACCESSOR{10};
constexpr OperatorSymbol AND{12};
constexpr OperatorSymbol BITWISE_AND{16};
constexpr OperatorSymbol BITWISE_LSHIFT{19};
constexpr OperatorSymbol BITWISE_NOT{27};
constexpr OperatorSymbol BITWISE_OR{17};
constexpr OperatorSymbol BITWISE_RSHIFT{20};
constexpr OperatorSymbol BITWISE_XOR{18};
constexpr TokenKind BOOLEAN{3};
constexpr TokenKind CLAUSE{14};
constexpr TokenKind CLAUSE_CLOSE{15};
constexpr OperatorSymbol COALESCE{30};
constexpr TokenKind COMPARATOR{11};
constexpr OperatorSymbol DIVIDE{22};
MapParameters DUMMY_PARAMETERS;
constexpr OperatorSymbol EQ{3};
constexpr OperatorSymbol EXPONENT{24};

struct EvaluableExpression : public Parameters {
    std::string QueryDateFormat{""};
    bool ChecksTypes{false};
    moku::slice<ExpressionToken> tokens;
    evaluationStage *evaluationStages{std::nullptr};
    std::string inputExpression{""};
    inline std::tuple<moku::interface, moku::error> Evaluate(
            std::map<std::string, moku::interface> parameters)
    {
        EvaluableExpression evaluableexpression = *this;
        return evaluableexpression._EvaluateByValue(
                std::map<std::string, moku::interface> parameters);
    }
    std::tuple<moku::interface, moku::error> _EvaluateByValue(
            std::map<std::string, moku::interface> parameters);
    inline std::tuple<moku::interface, moku::error> Eval(Parameters parameters)
    {
        EvaluableExpression evaluableexpression = *this;
        return evaluableexpression._EvalByValue(Parameters parameters);
    }
    std::tuple<moku::interface, moku::error> _EvalByValue(
            Parameters parameters);
    inline std::tuple<moku::interface, moku::error> evaluateStage(
            evaluationStage *stage, Parameters parameters)
    {
        EvaluableExpression evaluableexpression = *this;
        return evaluableexpression._evaluateStageByValue(
                evaluationStage * stage, Parameters parameters);
    }
    std::tuple<moku::interface, moku::error> _evaluateStageByValue(
            evaluationStage *stage, Parameters parameters);
    inline moku::slice<ExpressionToken> Tokens()
    {
        EvaluableExpression evaluableexpression = *this;
        return evaluableexpression._TokensByValue();
    }
    moku::slice<ExpressionToken> _TokensByValue();
    inline std::string String()
    {
        EvaluableExpression evaluableexpression = *this;
        return evaluableexpression._StringByValue();
    }
    std::string _StringByValue();
    inline moku::slice<std::string> Vars()
    {
        EvaluableExpression evaluableexpression = *this;
        return evaluableexpression._VarsByValue();
    }
    moku::slice<std::string> _VarsByValue();
    inline std::tuple<std::string, moku::error> ToSQLQuery()
    {
        EvaluableExpression evaluableexpression = *this;
        return evaluableexpression._ToSQLQueryByValue();
    }
    std::tuple<std::string, moku::error> _ToSQLQueryByValue();
    inline std::tuple<std::string, moku::error> findNextSQLString(
            tokenStream *stream, expressionOutputStream *transactions)
    {
        EvaluableExpression evaluableexpression = *this;
        return evaluableexpression._findNextSQLStringByValue(
                tokenStream * stream, expressionOutputStream * transactions);
    }
    std::tuple<std::string, moku::error> _findNextSQLStringByValue(
            tokenStream *stream, expressionOutputStream *transactions);
};
template <>
inline bool moku::is_nil<EvaluableExpression>(
        const EvaluableExpression &evaluableexpression)
{
    return moku::is_nil<std::string>(evaluableexpression.QueryDateFormat) &&
           moku::is_nil<bool>(evaluableexpression.ChecksTypes) &&
           moku::is_nil<moku::slice<ExpressionToken>>(
                   evaluableexpression.tokens) &&
           moku::is_nil<evaluationStage *>(
                   evaluableexpression.evaluationStages) &&
           moku::is_nil<std::string>(evaluableexpression.inputExpression);
}
template <>
EvaluableExpression *moku::try_assert(const moku::interface &iface)
{
    if (Parameters *parameters =
                moku::type_registry::try_assert<Parameters>(iface))
        return parameters;
    return std::nullptr;
}
} // namespace govaluate
