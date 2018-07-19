std::tuple<EvaluableExpression *, moku::error> NewEvaluableExpression(
        std::string expression)
{
    std::map<std::string, ExpressionFunction> functions{};

    functions = make(std::map<std::string, ExpressionFunction>);
    return NewEvaluableExpressionWithFunctions(expression, functions);
}
std::tuple<EvaluableExpression *, moku::error> NewEvaluableExpressionFromTokens(
        moku::slice<ExpressionToken> tokens)
{
    moku::error err{};
    EvaluableExpression *ret{std::nullptr};

    ret = new (EvaluableExpression);
    ret->QueryDateFormat = isoDateFormat;
    err = checkBalance(tokens);
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    err = checkExpressionSyntax(tokens);
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    std::tie(ret->tokens, err) = optimizeTokens(tokens);
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    std::tie(ret->evaluationStages, err) = planStages(ret->tokens);
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    ret->ChecksTypes = true;
    return {ret, nil};
}
std::tuple<EvaluableExpression *, moku::error>
NewEvaluableExpressionWithFunctions(
        std::string expression,
        std::map<std::string, ExpressionFunction> functions)
{
    moku::error err{};
    EvaluableExpression *ret{std::nullptr};

    ret = new (EvaluableExpression);
    ret->QueryDateFormat = isoDateFormat;
    ret->inputExpression = expression;
    std::tie(ret->tokens, err) = parseTokens(expression, functions);
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    err = checkBalance(ret->tokens);
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    err = checkExpressionSyntax(ret->tokens);
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    std::tie(ret->tokens, err) = optimizeTokens(ret->tokens);
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    std::tie(ret->evaluationStages, err) = planStages(ret->tokens);
    if (!moku::is_nil(err)) {
        return {nil, err};
    }
    ret->ChecksTypes = true;
    return {ret, nil};
}
std::tuple<moku::interface, moku::error> EvaluableExpression::_EvaluateByValue(
        std::map<std::string, moku::interface> parameters)
{
    if (moku::is_nil(parameters)) {
        return this->Eval(nil);
    }
    return this->Eval(MapParameters(parameters));
}
std::tuple<moku::interface, moku::error> EvaluableExpression::_EvalByValue(
        Parameters parameters)
{
    if (moku::is_nil(this->evaluationStages)) {
        return {nil, nil};
    }
    if (!moku::is_nil(parameters)) {
        parameters = &sanitizedParameters{parameters};
    } else {
        parameters = DUMMY_PARAMETERS;
    }
    return this->evaluateStage(this->evaluationStages, parameters);
}
std::tuple<moku::interface, moku::error>
EvaluableExpression::_evaluateStageByValue(evaluationStage *stage,
                                           Parameters parameters)
{
    moku::error err{};
    moku::interface left{};
    moku::interface right{};

    if (!moku::is_nil(stage->leftStage)) {
        std::tie(left, err) = this->evaluateStage(stage->leftStage, parameters);
        if (!moku::is_nil(err)) {
            return {nil, err};
        }
    }
    if (stage->isShortCircuitable()) {
        if ((stage->symbol == AND)) {
        _ident_0_:
            if (left == false) {
                return {false, nil};
            }
        } else if ((stage->symbol == OR)) {
        _ident_1_:
            if (left == true) {
                return {true, nil};
            }
        } else if ((stage->symbol == COALESCE)) {
        _ident_2_:
            if (!moku::is_nil(left)) {
                return {left, nil};
            }
        } else if ((stage->symbol == TERNARY_TRUE)) {
        _ident_3_:
            if (left == false) {
                right = shortCircuitHolder;
            }
        } else if ((stage->symbol == TERNARY_FALSE)) {
        _ident_4_:
            if (!moku::is_nil(left)) {
                right = shortCircuitHolder;
            }
        }
    }
    if (right != shortCircuitHolder && !moku::is_nil(stage->rightStage)) {
        std::tie(right, err) =
                this->evaluateStage(stage->rightStage, parameters);
        if (!moku::is_nil(err)) {
            return {nil, err};
        }
    }
    if (this->ChecksTypes) {
        if (moku::is_nil(stage->typeCheck)) {
            err = typeCheck(stage->leftTypeCheck,
                            moku::make_iface<moku::interface>(left),
                            stage->symbol,
                            stage->typeErrorFormat);
            if (!moku::is_nil(err)) {
                return {nil, err};
            }
            err = typeCheck(stage->rightTypeCheck,
                            moku::make_iface<moku::interface>(right),
                            stage->symbol,
                            stage->typeErrorFormat);
            if (!moku::is_nil(err)) {
                return {nil, err};
            }
        } else {
            if (!stage->typeCheck(left, right)) {
                errorMsg = fmt::Sprintf(
                        stage->typeErrorFormat, left, stage->symbol.String());
                return {nil, errors::New(errorMsg)};
            }
        }
    }
    return stage->operator(left, right, parameters);
}
moku::error typeCheck(stageTypeCheck check,
                      moku::interface value,
                      OperatorSymbol symbol,
                      std::string format)
{
    std::string errorMsg{""};

    if (moku::is_nil(check)) {
        return nil;
    }
    if (check(value)) {
        return nil;
    }
    errorMsg = fmt::Sprintf(format, value, symbol.String());
    return errors::New(errorMsg);
}
moku::slice<ExpressionToken> EvaluableExpression::_TokensByValue()
{
    return this->tokens;
}
std::string EvaluableExpression::_StringByValue()
{
    return this->inputExpression;
}
moku::slice<std::string> EvaluableExpression::_VarsByValue()
{
    moku::slice<std::string> varlist{};

    {
        ExpressionToken val{};
        for (val :
             moku::range_value<moku::slice<ExpressionToken>>(this->Tokens())) {
            if (val.Kind == VARIABLE) {
                varlist = append(varlist,
                                 moku::type_assert<std::string>(val.Value));
            }
        }
    }
    return varlist;
}
void init();
int main()
{
    init();
    DUMMY_PARAMETERS =
            MapParameters(std::map<std::string, moku::empty_interface>{});
    comparatorSymbols = std::map<std::string, OperatorSymbol>{{"==", EQ},
                                                              {"!=", NEQ},
                                                              {">", GT},
                                                              {">=", GTE},
                                                              {"<", LT},
                                                              {"<=", LTE},
                                                              {"=~", REQ},
                                                              {"!~", NREQ},
                                                              {"in", IN}};
    logicalSymbols =
            std::map<std::string, OperatorSymbol>{{"&&", AND}, {"||", OR}};
    bitwiseSymbols = std::map<std::string, OperatorSymbol>{
            {"^", BITWISE_XOR}, {"&", BITWISE_AND}, {"|", BITWISE_OR}};
    bitwiseShiftSymbols = std::map<std::string, OperatorSymbol>{
            {">>", BITWISE_RSHIFT}, {"<<", BITWISE_LSHIFT}};
    additiveSymbols =
            std::map<std::string, OperatorSymbol>{{"+", PLUS}, {"-", MINUS}};
    multiplicativeSymbols = std::map<std::string, OperatorSymbol>{
            {"*", MULTIPLY}, {"/", DIVIDE}, {"%", MODULUS}};
    exponentialSymbolsS =
            std::map<std::string, OperatorSymbol>{{"**", EXPONENT}};
    prefixSymbols = std::map<std::string, OperatorSymbol>{
            {"-", NEGATE}, {"!", INVERT}, {"~", BITWISE_NOT}};
    ternarySymbols = std::map<std::string, OperatorSymbol>{
            {"?", TERNARY_TRUE}, {":", TERNARY_FALSE}, {"??", COALESCE}};
    modifierSymbols =
            std::map<std::string, OperatorSymbol>{{"+", PLUS},
                                                  {"-", MINUS},
                                                  {"*", MULTIPLY},
                                                  {"/", DIVIDE},
                                                  {"%", MODULUS},
                                                  {"**", EXPONENT},
                                                  {"&", BITWISE_AND},
                                                  {"|", BITWISE_OR},
                                                  {"^", BITWISE_XOR},
                                                  {">>", BITWISE_RSHIFT},
                                                  {"<<", BITWISE_LSHIFT}};
    separatorSymbols = std::map<std::string, OperatorSymbol>{{",", SEPARATE}};
    _true = moku::empty_interface(true);
    _false = moku::empty_interface(false);
    validLexerStates = moku::slice<lexerState>{
            lexerState{{kind, UNKNOWN},
                       {isEOF, false},
                       {isNullable, true},
                       {validNextKinds,
                        moku::slice<TokenKind>{PREFIX,
                                               NUMERIC,
                                               BOOLEAN,
                                               VARIABLE,
                                               PATTERN,
                                               FUNCTION,
                                               ACCESSOR,
                                               STRING,
                                               TIME,
                                               CLAUSE}}},
            lexerState{{kind, CLAUSE},
                       {isEOF, false},
                       {isNullable, true},
                       {validNextKinds,
                        moku::slice<TokenKind>{PREFIX,
                                               NUMERIC,
                                               BOOLEAN,
                                               VARIABLE,
                                               PATTERN,
                                               FUNCTION,
                                               ACCESSOR,
                                               STRING,
                                               TIME,
                                               CLAUSE,
                                               CLAUSE_CLOSE}}},
            lexerState{{kind, CLAUSE_CLOSE},
                       {isEOF, true},
                       {isNullable, true},
                       {validNextKinds,
                        moku::slice<TokenKind>{COMPARATOR,
                                               MODIFIER,
                                               NUMERIC,
                                               BOOLEAN,
                                               VARIABLE,
                                               STRING,
                                               PATTERN,
                                               TIME,
                                               CLAUSE,
                                               CLAUSE_CLOSE,
                                               LOGICALOP,
                                               TERNARY,
                                               SEPARATOR}}},
            lexerState{{kind, NUMERIC},
                       {isEOF, true},
                       {isNullable, false},
                       {validNextKinds,
                        moku::slice<TokenKind>{MODIFIER,
                                               COMPARATOR,
                                               LOGICALOP,
                                               CLAUSE_CLOSE,
                                               TERNARY,
                                               SEPARATOR}}},
            lexerState{{kind, BOOLEAN},
                       {isEOF, true},
                       {isNullable, false},
                       {validNextKinds,
                        moku::slice<TokenKind>{MODIFIER,
                                               COMPARATOR,
                                               LOGICALOP,
                                               CLAUSE_CLOSE,
                                               TERNARY,
                                               SEPARATOR}}},
            lexerState{{kind, STRING},
                       {isEOF, true},
                       {isNullable, false},
                       {validNextKinds,
                        moku::slice<TokenKind>{MODIFIER,
                                               COMPARATOR,
                                               LOGICALOP,
                                               CLAUSE_CLOSE,
                                               TERNARY,
                                               SEPARATOR}}},
            lexerState{{kind, TIME},
                       {isEOF, true},
                       {isNullable, false},
                       {validNextKinds,
                        moku::slice<TokenKind>{MODIFIER,
                                               COMPARATOR,
                                               LOGICALOP,
                                               CLAUSE_CLOSE,
                                               SEPARATOR}}},
            lexerState{{kind, PATTERN},
                       {isEOF, true},
                       {isNullable, false},
                       {validNextKinds,
                        moku::slice<TokenKind>{MODIFIER,
                                               COMPARATOR,
                                               LOGICALOP,
                                               CLAUSE_CLOSE,
                                               SEPARATOR}}},
            lexerState{{kind, VARIABLE},
                       {isEOF, true},
                       {isNullable, false},
                       {validNextKinds,
                        moku::slice<TokenKind>{MODIFIER,
                                               COMPARATOR,
                                               LOGICALOP,
                                               CLAUSE_CLOSE,
                                               TERNARY,
                                               SEPARATOR}}},
            lexerState{{kind, MODIFIER},
                       {isEOF, false},
                       {isNullable, false},
                       {validNextKinds,
                        moku::slice<TokenKind>{PREFIX,
                                               NUMERIC,
                                               VARIABLE,
                                               FUNCTION,
                                               ACCESSOR,
                                               STRING,
                                               BOOLEAN,
                                               CLAUSE,
                                               CLAUSE_CLOSE}}},
            lexerState{{kind, COMPARATOR},
                       {isEOF, false},
                       {isNullable, false},
                       {validNextKinds,
                        moku::slice<TokenKind>{PREFIX,
                                               NUMERIC,
                                               BOOLEAN,
                                               VARIABLE,
                                               FUNCTION,
                                               ACCESSOR,
                                               STRING,
                                               TIME,
                                               CLAUSE,
                                               CLAUSE_CLOSE,
                                               PATTERN}}},
            lexerState{{kind, LOGICALOP},
                       {isEOF, false},
                       {isNullable, false},
                       {validNextKinds,
                        moku::slice<TokenKind>{PREFIX,
                                               NUMERIC,
                                               BOOLEAN,
                                               VARIABLE,
                                               FUNCTION,
                                               ACCESSOR,
                                               STRING,
                                               TIME,
                                               CLAUSE,
                                               CLAUSE_CLOSE}}},
            lexerState{{kind, PREFIX},
                       {isEOF, false},
                       {isNullable, false},
                       {validNextKinds,
                        moku::slice<TokenKind>{NUMERIC,
                                               BOOLEAN,
                                               VARIABLE,
                                               FUNCTION,
                                               ACCESSOR,
                                               CLAUSE,
                                               CLAUSE_CLOSE}}},
            lexerState{{kind, TERNARY},
                       {isEOF, false},
                       {isNullable, false},
                       {validNextKinds,
                        moku::slice<TokenKind>{PREFIX,
                                               NUMERIC,
                                               BOOLEAN,
                                               STRING,
                                               TIME,
                                               VARIABLE,
                                               FUNCTION,
                                               ACCESSOR,
                                               CLAUSE,
                                               SEPARATOR}}},
            lexerState{{kind, FUNCTION},
                       {isEOF, false},
                       {isNullable, false},
                       {validNextKinds, moku::slice<TokenKind>{CLAUSE}}},
            lexerState{
                    {kind, ACCESSOR},
                    {isEOF, true},
                    {isNullable, false},
                    {validNextKinds,
                     moku::slice<TokenKind>{CLAUSE,
                                            MODIFIER,
                                            COMPARATOR,
                                            LOGICALOP,
                                            CLAUSE_CLOSE,
                                            TERNARY,
                                            SEPARATOR}}},
            lexerState{{kind, SEPARATOR},
                       {isEOF, false},
                       {isNullable, true},
                       {validNextKinds,
                        moku::slice<TokenKind>{PREFIX,
                                               NUMERIC,
                                               BOOLEAN,
                                               STRING,
                                               TIME,
                                               VARIABLE,
                                               FUNCTION,
                                               ACCESSOR,
                                               CLAUSE}}}};
    stageSymbolMap = std::map<OperatorSymbol, evaluationOperator>{
            {EQ, equalStage},
            {NEQ, notEqualStage},
            {GT, gtStage},
            {LT, ltStage},
            {GTE, gteStage},
            {LTE, lteStage},
            {REQ, regexStage},
            {NREQ, notRegexStage},
            {AND, andStage},
            {OR, orStage},
            {IN, inStage},
            {BITWISE_OR, bitwiseOrStage},
            {BITWISE_AND, bitwiseAndStage},
            {BITWISE_XOR, bitwiseXORStage},
            {BITWISE_LSHIFT, leftShiftStage},
            {BITWISE_RSHIFT, rightShiftStage},
            {PLUS, addStage},
            {MINUS, subtractStage},
            {MULTIPLY, multiplyStage},
            {DIVIDE, divideStage},
            {MODULUS, modulusStage},
            {EXPONENT, exponentStage},
            {NEGATE, negateStage},
            {INVERT, invertStage},
            {BITWISE_NOT, bitwiseNotStage},
            {TERNARY_TRUE, ternaryIfStage},
            {TERNARY_FALSE, ternaryElseStage},
            {COALESCE, ternaryElseStage},
            {SEPARATE, separatorStage}};
    _main();
    return 0;
}
