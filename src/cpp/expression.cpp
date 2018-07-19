// Copyright 2018 Couchbase, Inc. All rights reserved.

#include <string>
#include <vector>
#include <memory>

#include <utility>
#include <set>
#include <algorithm>
#include <string>
#include <strstream>
struct Err: std::exception{};
struct Value: public void*
{

};
struct FieldExpr;
typedef std::vector<FieldExpr> FieldExprVct;
typedef std::set<FieldExpr> FieldExprSet;

struct Expression{
    virtual std::shared_ptr<std::string> String() const =0;
    virtual std::shared_ptr<FieldExprSet> RootRefs() const = 0;

};

template <typename STRINGITERABLE>
std::string join(STRINGITERABLE Path, const std::string &separator)
{
    return *(Path.begin()) + std::accumulate(++Path.begin(),
                                             Path.end(),
                                             "",
                                             [](const std::string &sum,
                                                const std::string &addition) {
                                                 return sum + "." + addition;
                                             });
}
struct FieldExpr: public Expression  {
	int Root;
	std::vector<std::string> Path;
	bool operator==(const FieldExpr& rhs) const;
    std::shared_ptr<std::string> String() const {
        static std::shared_ptr<std::string> DOC=std::make_shared("$doc");
        std::shared_ptr<std::string> rootStr = DOC;
        if (Root != 0) {
            std::strstream result;
            result<<"$"<<Root;
            rootStr = result.str();
        }

        if (!Path.empty()) {
            return std::make_shared(rootStr + join(Path,"."));

        } else {
            return rootStr;
        }
    };

    std::shared_ptr<FieldExprSet> RootRefs() const {
        if (Root) {
            return nullptr;
        }
        return std::make_shared<FieldExprSet>({*this});
    };
};

bool fieldExprCompare(const FieldExpr &lhs, const FieldExpr &rhs)
{
    return rhs.Root == lhs.Root && rhs.Path.size() == lhs.Path.size() &&
           lhs.Path == rhs.Path;
}

bool bool FieldExpr::operator==(const FieldExpr &rhs) const {
	return fieldExprCompare(*this,rhs);
}

std::shared_ptr<const FieldExprSet> rootSetAdd(
        std::shared_ptr<const FieldExprSet> exprSet,
        const std::vector<FieldExpr> &items)
{
    FieldExprSet output;
    std::set_difference(exprSet->begin(),
                        exprSet->end(),
                        items.begin(),
                        items.end(),
                        output.begin());
    if (output.empty()) {
        return exprSet;
    }
    auto freshSet(std::make_shared<FieldExprSet>(exprSet));
    freshSet->insert(output.begin(), output.end());
    return freshSet;
}
std::shared_ptr<const FieldExprSet> rootSetAddOne(
        std::shared_ptr<const FieldExprSet> exprSet, const FieldExpr &item)
{
    return rootSetAdd(exprSet, std::vector<FieldExpr>{item});
}

struct TrueExpr : public Expression {
    std::shared_ptr<std::string> String() const {
        static std::shared_ptr<std::string> TRUE=std::make_shared("True");
        return TRUE;
    }

    std::shared_ptr<FieldExprSet> RootRefs() const {return nullptr;}
}

struct FalseExpr: public Expression {
    std::shared_ptr<std::string> String() const {
        static std::shared_ptr<std::string> TRUE=std::make_shared("False");
        return TRUE;
    }

    std::shared_ptr<FieldExprSet> RootRefs() const {return nullptr;}
};
struct Any{
    union {
        std::string text;
        unsigned int uint;
    } data;
    enum {ANY_TEXT,ANY_UINT} type;
    template <typename STREAM>
    void write(STREAM& stream) const{
        switch (type){
            case ANY_TEXT:
                stream<<data.text;
                break;
            case ANY_UINT:
                stream<<data.uint;
                break;
        }
    }

};

struct ValueExpr: public Expression  {
	Any Value;
    std::shared_ptr<std::string> String() const {
        std::strstream result;
        Value.write(result);
        return std::make_shared(result.str());
    }
    std::shared_ptr<FieldExprSet> RootRefs() const{
        return nullptr;
    }
}

struct AndExpr{


    std::shared_ptr<std::string> String() const {
	if len(expr) == 0 {
		return "%%ERROR%%"
	} else if len(expr) == 1 {
		return expr[0].String()
	} else {
		value := reindentString(expr[0].String(), "  ")
		for i := 1; i < len(expr); i++ {
			value += "\nAND\n"
			value += reindentString(expr[i].String(), "  ")
		}
		return value
	}


    std::shared_ptr<FieldExprSet> RootRefs() const{
        var out []FieldExpr
        for _, subexpr := range expr {
            out = rootSetAdd(out, subexpr.RootRefs()...)
        }
        return out
    }
};

type OrExpr []Expression

func (expr OrExpr) String() string {
	if len(expr) == 0 {
		return "%%ERROR%%"
	} else if len(expr) == 1 {
		return expr[0].String()
	} else {
		value := reindentString(expr[0].String(), "  ")
		for i := 1; i < len(expr); i++ {
			value += "\nOR\n"
			value += reindentString(expr[i].String(), "  ")
		}
		return value
	}
}

func (expr OrExpr) RootRefs() []FieldExpr {
	var out []FieldExpr
	for _, subexpr := range expr {
		out = rootSetAdd(out, subexpr.RootRefs()...)
	}
	return out
}


type AnyInExpr struct {
	VarId   int
	InExpr  Expression
	SubExpr Expression
}

func (expr AnyInExpr) String() string {
	exprStr := reindentString(expr.SubExpr.String(), "  ")
	return fmt.Sprintf("any $%d in %s\n%s\nend", expr.VarId, expr.InExpr, exprStr)
}

func (expr AnyInExpr) RootRefs() []FieldExpr {
	var out []FieldExpr
	out = rootSetAdd(out, expr.InExpr.RootRefs()...)
	out = rootSetAdd(out, expr.SubExpr.RootRefs()...)
	return out
}

type EveryInExpr struct {
	VarId   int
	InExpr  Expression
	SubExpr Expression
}

func (expr EveryInExpr) String() string {
	exprStr := reindentString(expr.SubExpr.String(), "  ")
	return fmt.Sprintf("every $%d in %s\n%s\nend", expr.VarId, expr.InExpr, exprStr)
}

func (expr EveryInExpr) RootRefs() []FieldExpr {
	var out []FieldExpr
	out = rootSetAdd(out, expr.InExpr.RootRefs()...)
	out = rootSetAdd(out, expr.SubExpr.RootRefs()...)
	return out
}

type EqualsExpr struct {
	Lhs Expression
	Rhs Expression
}

func (expr EqualsExpr) String() string {
	return fmt.Sprintf("%s = %s", expr.Lhs, expr.Rhs)
}

func (expr EqualsExpr) RootRefs() []FieldExpr {
	var out []FieldExpr
	out = rootSetAdd(out, expr.Lhs.RootRefs()...)
	out = rootSetAdd(out, expr.Rhs.RootRefs()...)
	return out
}

type LessThanExpr struct {
	Lhs Expression
	Rhs Expression
}

func (expr LessThanExpr) String() string {
	return fmt.Sprintf("%s < %s", expr.Lhs, expr.Rhs)
}

func (expr LessThanExpr) RootRefs() []FieldExpr {
	var out []FieldExpr
	out = rootSetAdd(out, expr.Lhs.RootRefs()...)
	out = rootSetAdd(out, expr.Rhs.RootRefs()...)
	return out
}

type GreaterEqualExpr struct {
	Lhs Expression
	Rhs Expression
}

func (expr GreaterEqualExpr) String() string {
	return fmt.Sprintf("%s >= %s", expr.Lhs, expr.Rhs)
}

func (expr GreaterEqualExpr) RootRefs() []FieldExpr {
	var out []FieldExpr
	out = rootSetAdd(out, expr.Lhs.RootRefs()...)
	out = rootSetAdd(out, expr.Rhs.RootRefs()...)
	return out
}

type InExpr struct {
	Lhs Expression
	Rhs []interface{}
}
*/