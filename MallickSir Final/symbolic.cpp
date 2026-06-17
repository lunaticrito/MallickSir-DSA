#include "symbolic.h"
#include "expression_engine.h"
#include <stdexcept>
#include <cmath>

struct IntegrationError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

namespace {

Ep makeNum(double n) {
    auto e = std::make_shared<Expr>();
    e->type = NodeType::NUM;
    e->num = n;
    return e;
}

Ep makeVar(const std::string& v) {
    auto e = std::make_shared<Expr>();
    e->type = NodeType::VAR;
    e->var = v;
    return e;
}

Ep makeAdd(Ep a, Ep b) {
    auto e = std::make_shared<Expr>();
    e->type = NodeType::ADD;
    e->children = {a, b};
    return e;
}

Ep makeMul(Ep a, Ep b) {
    auto e = std::make_shared<Expr>();
    e->type = NodeType::MUL;
    e->children = {a, b};
    return e;
}

Ep makePow(Ep a, Ep b) {
    auto e = std::make_shared<Expr>();
    e->type = NodeType::POW;
    e->children = {a, b};
    return e;
}

Ep makeNeg(Ep a) {
    auto e = std::make_shared<Expr>();
    e->type = NodeType::NEG;
    e->children = {a};
    return e;
}

Ep makeSin(Ep a) {
    auto e = std::make_shared<Expr>();
    e->type = NodeType::SIN;
    e->children = {a};
    return e;
}

Ep makeCos(Ep a) {
    auto e = std::make_shared<Expr>();
    e->type = NodeType::COS;
    e->children = {a};
    return e;
}

Ep makeLn(Ep a) {
    auto e = std::make_shared<Expr>();
    e->type = NodeType::LN;
    e->children = {a};
    return e;
}

Ep makeExp(Ep a) {
    auto e = std::make_shared<Expr>();
    e->type = NodeType::EXP;
    e->children = {a};
    return e;
}

Ep mapFunction(const std::string& fn, Ep arg) {
    if (fn == "sin")  return makeSin(arg);
    if (fn == "cos")  return makeCos(arg);
    if (fn == "log" || fn == "ln") return makeLn(arg);
    if (fn == "exp")  return makeExp(arg);
    if (fn == "sqrt") return makePow(arg, makeNum(0.5));
    throw std::runtime_error("Function not supported in symbolic mode: " + fn);
}

class InfixParser {
public:
    explicit InfixParser(const std::vector<Token>& tokens) : tokens_(tokens) {}

    Ep parse() {
        if (tokens_.empty()) throw std::runtime_error("Empty expression");
        Ep e = parseExpr();
        if (pos_ < tokens_.size())
            throw std::runtime_error("Unexpected token: " + tokens_[pos_].value);
        return e;
    }

private:
    const std::vector<Token>& tokens_;
    std::size_t pos_ = 0;

    const Token& peek() const { return tokens_.at(pos_); }
    bool atEnd() const { return pos_ >= tokens_.size(); }

    Token consume() { return tokens_.at(pos_++); }

    bool match(Token::Type t) const {
        return !atEnd() && peek().type == t;
    }

    bool matchOp(const std::string& op) const {
        return match(Token::Type::Operator) && peek().value == op;
    }

    Ep parseExpr() {
        Ep left = parseTerm();
        while (matchOp("+") || matchOp("-")) {
            std::string op = consume().value;
            Ep right = parseTerm();
            left = (op == "+") ? makeAdd(left, right) : makeAdd(left, makeNeg(right));
        }
        return left;
    }

    Ep parseTerm() {
        Ep left = parsePow();
        while (matchOp("*") || matchOp("/") || matchOp("%")) {
            std::string op = consume().value;
            Ep right = parsePow();
            if (op == "*") left = makeMul(left, right);
            else if (op == "/") left = makeMul(left, makePow(right, makeNum(-1)));
            else throw std::runtime_error("Modulo not supported in symbolic mode");
        }
        return left;
    }

    Ep parsePow() {
        Ep left = parseUnary();
        if (matchOp("^")) {
            consume();
            return makePow(left, parsePow());
        }
        return left;
    }

    Ep parseUnary() {
        if (matchOp("-")) {
            consume();
            return makeNeg(parseUnary());
        }
        return parsePrimary();
    }

    Ep parsePrimary() {
        if (match(Token::Type::Number)) {
            return makeNum(std::stod(consume().value));
        }
        if (match(Token::Type::Variable)) {
            return makeVar(consume().value);
        }
        if (match(Token::Type::Function)) {
            std::string fn = consume().value;
            if (!match(Token::Type::LeftParen))
                throw std::runtime_error("Expected '(' after function " + fn);
            consume();
            Ep arg = parseExpr();
            if (!match(Token::Type::RightParen))
                throw std::runtime_error("Expected ')' after function argument");
            consume();
            return mapFunction(fn, arg);
        }
        if (match(Token::Type::LeftParen)) {
            consume();
            Ep e = parseExpr();
            if (!match(Token::Type::RightParen))
                throw std::runtime_error("Mismatched parentheses");
            consume();
            return e;
        }
        throw std::runtime_error("Unexpected token: " + (atEnd() ? "end" : peek().value));
    }
};

Ep simp(Ep e) {
    for (auto& c : e->children) c = simp(c);

    if (e->type == NodeType::ADD) {
        auto& a = e->children[0];
        auto& b = e->children[1];
        if (a->type == NodeType::NUM && b->type == NodeType::NUM) return makeNum(a->num + b->num);
        if (a->type == NodeType::NUM && a->num == 0) return b;
        if (b->type == NodeType::NUM && b->num == 0) return a;
    }
    if (e->type == NodeType::MUL) {
        auto& a = e->children[0];
        auto& b = e->children[1];
        if (a->type == NodeType::NUM && b->type == NodeType::NUM) return makeNum(a->num * b->num);
        if (a->type == NodeType::NUM && a->num == 0) return makeNum(0);
        if (b->type == NodeType::NUM && b->num == 0) return makeNum(0);
        if (a->type == NodeType::NUM && a->num == 1) return b;
        if (b->type == NodeType::NUM && b->num == 1) return a;
    }
    if (e->type == NodeType::POW) {
        auto& a = e->children[0];
        auto& b = e->children[1];
        if (b->type == NodeType::NUM && b->num == 0) return makeNum(1);
        if (b->type == NodeType::NUM && b->num == 1) return a;
        if (a->type == NodeType::NUM && a->num == 1) return makeNum(1);
    }
    if (e->type == NodeType::NEG) {
        auto& a = e->children[0];
        if (a->type == NodeType::NUM) return makeNum(-a->num);
    }
    return e;
}

Ep diff(Ep e, const std::string& wrt) {
    switch (e->type) {
    case NodeType::NUM:
        return makeNum(0);

    case NodeType::VAR:
        return makeNum(e->var == wrt ? 1 : 0);

    case NodeType::ADD: {
        auto f = e->children[0], g = e->children[1];
        return simp(makeAdd(diff(f, wrt), diff(g, wrt)));
    }

    case NodeType::NEG:
        return simp(makeNeg(diff(e->children[0], wrt)));

    case NodeType::MUL: {
        auto f = e->children[0], g = e->children[1];
        return simp(makeAdd(
            makeMul(diff(f, wrt), g),
            makeMul(f, diff(g, wrt))));
    }

    case NodeType::POW: {
        auto f = e->children[0], g = e->children[1];
        if (f->type == NodeType::VAR && f->var == wrt && g->type == NodeType::NUM)
            return simp(makeMul(g, makePow(f, makeNum(g->num - 1))));
        if (f->type == NodeType::NUM && g->type == NodeType::VAR && g->var == wrt)
            return simp(makeMul(e, makeLn(f)));
        return simp(makeMul(e,
            makeAdd(makeMul(diff(g, wrt), makeLn(f)),
                    makeMul(g, makeMul(diff(f, wrt), makePow(f, makeNum(-1)))))));
    }

    case NodeType::SIN:
        return simp(makeMul(makeCos(e->children[0]), diff(e->children[0], wrt)));

    case NodeType::COS:
        return simp(makeMul(makeNeg(makeSin(e->children[0])), diff(e->children[0], wrt)));

    case NodeType::LN:
        return simp(makeMul(diff(e->children[0], wrt),
                            makePow(e->children[0], makeNum(-1))));

    case NodeType::EXP:
        return simp(makeMul(e, diff(e->children[0], wrt)));

    default:
        return makeNum(0);
    }
}

bool hasVar(Ep e, const std::string& v) {
    if (e->type == NodeType::VAR) return e->var == v;
    for (auto& c : e->children)
        if (hasVar(c, v)) return true;
    return false;
}

bool linearIn(Ep e, const std::string& wrt, double& k) {
    if (e->type == NodeType::VAR && e->var == wrt) { k = 1; return true; }
    if (e->type == NodeType::MUL) {
        auto& a = e->children[0];
        auto& b = e->children[1];
        if (a->type == NodeType::NUM && b->type == NodeType::VAR && b->var == wrt) {
            k = a->num; return true;
        }
        if (b->type == NodeType::NUM && a->type == NodeType::VAR && a->var == wrt) {
            k = b->num; return true;
        }
    }
    if (e->type == NodeType::NEG) {
        double inner;
        if (linearIn(e->children[0], wrt, inner)) { k = -inner; return true; }
    }
    return false;
}

std::string printExpr(Ep e);

Ep integ(Ep e, const std::string& wrt) {
    switch (e->type) {
    case NodeType::NUM:
        return simp(makeMul(e, makeVar(wrt)));

    case NodeType::VAR:
        if (e->var == wrt)
            return simp(makeMul(makeNum(0.5), makePow(makeVar(wrt), makeNum(2))));
        return simp(makeMul(e, makeVar(wrt)));

    case NodeType::ADD: {
        auto f = e->children[0], g = e->children[1];
        return simp(makeAdd(integ(f, wrt), integ(g, wrt)));
    }

    case NodeType::NEG:
        return simp(makeNeg(integ(e->children[0], wrt)));

    case NodeType::MUL: {
        auto f = e->children[0], g = e->children[1];
        if (!hasVar(f, wrt)) return simp(makeMul(f, integ(g, wrt)));
        if (!hasVar(g, wrt)) return simp(makeMul(g, integ(f, wrt)));
        throw IntegrationError("Cannot integrate product: " + printExpr(e));
    }

    case NodeType::POW: {
        auto f = e->children[0], g = e->children[1];
        if (f->type == NodeType::VAR && f->var == wrt && g->type == NodeType::NUM) {
            if (g->num == -1) return makeLn(makeVar(wrt));
            return simp(makeMul(makePow(makeVar(wrt), makeNum(g->num + 1)),
                                makeNum(1.0 / (g->num + 1))));
        }
        if (f->type == NodeType::NUM && g->type == NodeType::VAR && g->var == wrt)
            return simp(makeMul(e, makePow(makeLn(f), makeNum(-1))));
        throw IntegrationError("Cannot integrate power: " + printExpr(e));
    }

    case NodeType::SIN: {
        Ep u = e->children[0];
        if (u->type == NodeType::VAR && u->var == wrt)
            return simp(makeNeg(makeCos(u)));
        double k;
        if (linearIn(u, wrt, k) && k != 0)
            return simp(makeMul(makeNeg(makeCos(u)), makeNum(1.0 / k)));
        throw IntegrationError("Cannot integrate sin: " + printExpr(e));
    }

    case NodeType::COS: {
        Ep u = e->children[0];
        if (u->type == NodeType::VAR && u->var == wrt)
            return makeSin(u);
        double k;
        if (linearIn(u, wrt, k) && k != 0)
            return simp(makeMul(makeSin(u), makeNum(1.0 / k)));
        throw IntegrationError("Cannot integrate cos: " + printExpr(e));
    }

    case NodeType::LN: {
        Ep u = e->children[0];
        if (u->type == NodeType::VAR && u->var == wrt)
            return simp(makeAdd(makeMul(makeVar(wrt), makeLn(makeVar(wrt))),
                                makeNeg(makeVar(wrt))));
        throw IntegrationError("Cannot integrate ln: " + printExpr(e));
    }

    case NodeType::EXP: {
        Ep u = e->children[0];
        if (u->type == NodeType::VAR && u->var == wrt)
            return e;
        double k;
        if (linearIn(u, wrt, k) && k != 0)
            return simp(makeMul(e, makeNum(1.0 / k)));
        throw IntegrationError("Cannot integrate exp: " + printExpr(e));
    }

    default:
        throw IntegrationError("Unsupported node for integration");
    }
}

std::string printExpr(Ep e) {
    switch (e->type) {
    case NodeType::NUM:
        if (e->num == static_cast<int>(e->num)) return std::to_string(static_cast<int>(e->num));
        return std::to_string(e->num);
    case NodeType::VAR:  return e->var;
    case NodeType::ADD:  return "(" + printExpr(e->children[0]) + " + " + printExpr(e->children[1]) + ")";
    case NodeType::MUL:  return "(" + printExpr(e->children[0]) + " * " + printExpr(e->children[1]) + ")";
    case NodeType::POW:  return "(" + printExpr(e->children[0]) + "^" + printExpr(e->children[1]) + ")";
    case NodeType::NEG:  return "(-" + printExpr(e->children[0]) + ")";
    case NodeType::SIN:  return "sin(" + printExpr(e->children[0]) + ")";
    case NodeType::COS:  return "cos(" + printExpr(e->children[0]) + ")";
    case NodeType::LN:   return "ln(" + printExpr(e->children[0]) + ")";
    case NodeType::EXP:  return "exp(" + printExpr(e->children[0]) + ")";
    default: return "?";
    }
}

} // namespace

Ep SymbolicEngine::parseInfix(const std::string& infix) {
    auto tokens = ExpressionEngine::tokenise(infix);
    InfixParser parser(tokens);
    return parser.parse();
}

Ep SymbolicEngine::differentiate(Ep e, const std::string& wrt) {
    return diff(e, wrt);
}

Ep SymbolicEngine::integrate(Ep e, const std::string& wrt) {
    return simp(integ(e, wrt));
}

Ep SymbolicEngine::simplify(Ep e) {
    return simp(e);
}

std::string SymbolicEngine::toString(Ep e) {
    return printExpr(e);
}

SymbolicDiffResult SymbolicEngine::diffInfix(const std::string& infix, const std::string& wrt) {
    SymbolicDiffResult res;
    try {
        auto ast = parseInfix(infix);
        res.original = toString(ast);
        res.derivative = toString(differentiate(ast, wrt));
        res.ok = true;
    } catch (const std::exception& e) {
        res.ok = false;
        res.error = e.what();
    }
    return res;
}

SymbolicIntegResult SymbolicEngine::integInfix(const std::string& infix, const std::string& wrt) {
    SymbolicIntegResult res;
    try {
        auto ast = parseInfix(infix);
        res.original = toString(ast);
        res.antiderivative = toString(integrate(ast, wrt));
        res.ok = true;
    } catch (const std::exception& e) {
        res.ok = false;
        res.error = e.what();
    }
    return res;
}
