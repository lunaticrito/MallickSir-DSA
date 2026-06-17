#pragma once
#include <string>
#include <vector>
#include <memory>

// CAS Module - Symbolic expression tree, simplification, differentiation, integration
// Parses infix strings via tokeniser.
// Supports: + - * / ^ unary-  sin cos log ln exp sqrt  variables  numbers

enum class NodeType
{
    NUM,
    VAR,
    ADD,
    MUL,
    POW,
    NEG,
    SIN,
    COS,
    LN,
    EXP
};

struct Expr
{
    NodeType type;
    double num = 0;
    std::string var;
    std::vector<std::shared_ptr<Expr>> children;
};

using Ep = std::shared_ptr<Expr>;

struct SymbolicDiffResult
{
    bool ok = false;
    std::string original;
    std::string derivative;
    std::string error;
};

struct SymbolicIntegResult
{
    bool ok = false;
    std::string original;
    std::string antiderivative;
    std::string error;
};

class SymbolicEngine
{
public:
    static Ep parseInfix(const std::string &infix);
    static Ep differentiate(Ep e, const std::string &wrt);
    static Ep integrate(Ep e, const std::string &wrt);
    static Ep simplify(Ep e);
    static std::string toString(Ep e);
    static SymbolicDiffResult diffInfix(const std::string &infix, const std::string &wrt);
    static SymbolicIntegResult integInfix(const std::string &infix, const std::string &wrt);
};
