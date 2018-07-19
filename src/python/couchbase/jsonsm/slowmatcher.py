# Copyright 2018 Couchbase, Inc. All rights reserved.



import json
from _ctypes import ArgumentError

class SlowMatcher:
    def __init__(self, exprs):#[]Expression)
        
        self.exprs= exprs
        self.exprMatches =list(False * len(exprs))#bool
        self.vars={}#map[int]interface{}

    def resolveFieldParam(self,expr):#FieldExpr) (interface{}, error) {
        rootVal = self.vars[expr.Root]
    
        curVal = rootVal
        for (_, field) in expr.Path:
            mapVal = curVal
            if mapVal:
                curVal = mapVal[field]
            else:
                raise ArgumentError("invalid path")
          
      

            return curVal

    def resolveParam(self,expr):# Expression) (interface{}, error) {
        if isinstance(expr,FieldExpr):
            return self.resolveFieldParam(expr)
        if isinstance(expr,ValueExpr):
            return expr.Value

        raise SyntaxError("unexpected param expression")

    def matchOrExpr(self, expr):# OrExpr) (bool, error) {
        for (_, subexpr) in expr:
            res, err = self.matchOne(subexpr)
            if err:
                return False, err
    
            if res:
                return True, None

            return False, None


    def matchAndExpr(self, expr):#AndExpr) (bool, error) {
        if not expr:
            return False, None

        for (_, subexpr) in expr:
            res, err = self.matchOne(subexpr)
            if err:
                return False, err
    
            if not res:
                return False, None

            return True, None
        
    def compareExprs(self,lhs,rhs):## lhs """Expression""", rhs """Expression"""):
        lhsVal, err = self.resolveParam(lhs)
        if err != None:
            return 0, err
        

        rhsVal, err = self.resolveParam(rhs)
        if err != None:
            return 0, err

        responses = switch lhsVal := lhsVal.(type) {
        case string:
            switch rhsVal := rhsVal.(type) {
            case string:
                return strings.Compare(lhsVal, rhsVal), nil
            }
            return 0, errors.New("invalid type comparisons")
        case float64:
            switch rhsVal := rhsVal.(type) {
            case float64:
                if lhsVal < rhsVal {
                    return -1, nil
                } else if lhsVal > rhsVal {
                    return 1, nil
                }
                return 0, nil
            }
            return 0, errors.New("invalid type comparisons")
        case bool:
            switch rhsVal := rhsVal.(type) {
            case bool:
                if lhsVal == true && rhsVal == false {
                    return 1, nil
                } else if lhsVal == false && rhsVal == true {
                    return -1, nil
                }
                return 0, nil
            }
        case nil:
            switch rhsVal.(type) {
            case nil:
                return 0, nil
            }
            return -1, nil
        }

        return 0, errors.New("unexpected lhs type")


func (m *SlowMatcher) matchAnyInExpr(expr AnyInExpr) (bool, error) {
    vals, err := m.resolveParam(expr.InExpr)
    if err != nil {
        return false, err
    }

    switch vals := vals.(type) {
    case map[string]interface{}:
        for _, val := range vals {
            m.vars[expr.VarId] = val
            res, err := m.matchOne(expr.SubExpr)
            delete(m.vars, expr.VarId)

            if err != nil {
                return false, err
            }

            if res {
                return true, nil
            }
        }

        return false, nil
    case []interface{}:
        for _, val := range vals {
            m.vars[expr.VarId] = val
            res, err := m.matchOne(expr.SubExpr)
            delete(m.vars, expr.VarId)

            if err != nil {
                return false, err
            }

            if res {
                return true, nil
            }
        }

        return false, nil
    }

    panic("unexpected any in param type")
}

func (m *SlowMatcher) matchEqualsExpr(expr EqualsExpr) (bool, error) {
    val, err := m.compareExprs(expr.Lhs, expr.Rhs)
    if err != nil {
        return false, err
    }

    return val == 0, nil
}

func (m *SlowMatcher) matchLessThanExpr(expr LessThanExpr) (bool, error) {
    val, err := m.compareExprs(expr.Lhs, expr.Rhs)
    if err != nil {
        return false, err
    }

    return val < 0, nil
}

func (m *SlowMatcher) matchGreaterEqualExpr(expr GreaterEqualExpr) (bool, error) {
    val, err := m.compareExprs(expr.Lhs, expr.Rhs)
    if err != nil {
        return false, err
    }

    return val >= 0, nil
}

func (m *SlowMatcher) matchOne(expr Expression) (bool, error) {
    switch expr := expr.(type) {
    case OrExpr:
        return m.matchOrExpr(expr)
    case AndExpr:
        return m.matchAndExpr(expr)
    case AnyInExpr:
        return m.matchAnyInExpr(expr)
    case EqualsExpr:
        return m.matchEqualsExpr(expr)
    case LessThanExpr:
        return m.matchLessThanExpr(expr)
    case GreaterEqualExpr:
        return m.matchGreaterEqualExpr(expr)
    }

    panic("unexpected expression")
}

func (m *SlowMatcher) matchOneRootExpr(expr Expression) (bool, error) {
    // We do this to match the Matcher requirement that all non-root
    // expressions be already compressed of all constant values.
    switch expr.(type) {
    case TrueExpr:
        return true, nil
    case FalseExpr:
        return false, nil
    }
    return m.matchOne(expr)
}

func (m *SlowMatcher) Reset() {
    for i := range m.vars {
        delete(m.vars, i)
    }
    for i := range m.exprMatches {
        m.exprMatches[i] = false
    }
}

func (m *SlowMatcher) Match(data []byte) (bool, error) {
    var parsedData interface{}
    if err := json.Unmarshal(data, &parsedData); err != nil {
        return false, err
    }

    if m.vars == nil {
        m.vars = make(map[int]interface{})
    }
    m.vars[0] = parsedData

    matched := false
    for i, expr := range m.exprs {
        res, err := m.matchOneRootExpr(expr)
        if err != nil {
            return false, err
        }

        m.exprMatches[i] = res

        if i == 0 && res {
            matched = true
        } else if !res {
            matched = false
        }
    }

    delete(m.vars, 0)
    return matched, nil
}

func (m *SlowMatcher) ExpressionMatched(expressionIdx int) bool {
    return m.exprMatches[expressionIdx]
}
