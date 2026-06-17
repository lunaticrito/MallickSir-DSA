#pragma once
#include <string>
#include <vector>
#include <unordered_map>


// Tokenisation
// Infix -> Postfix  (Shunting-Yard, uses Stack + Queue internally)
// Infix -> Prefix
// Postfix evaluation
// Prefix  evaluation
// Supports: +  -  *  /  ^  %  unary-minus  parentheses  variables  functions

struct Token {
    enum class Type { Number, Variable, Operator, LeftParen, RightParen, Function };
    Type        type;
    std::string value;
};

class ExpressionEngine {
public:
    using VarMap = std::unordered_map<std::string, double>;

    // Core conversions 
    static std::vector<Token> tokenise(const std::string& expr);
    static std::string        infixToPostfix(const std::string& expr);
    static std::string        infixToPrefix (const std::string& expr);

    // Evaluation 
    static double evalPostfix(const std::string& postfix,
                              const VarMap& vars = {});
    static double evalPrefix (const std::string& prefix,
                              const VarMap& vars = {});
    static double evalInfix  (const std::string& expr,
                              const VarMap& vars = {});   // convenience

    // Helpers 
    static bool   isOperator(const std::string& s);
    static int    precedence(const std::string& op);
    static bool   isLeftAssoc(const std::string& op);
    static bool   isFunction(const std::string& s);
    static double applyOp(const std::string& op, double a, double b);
    static double applyFunc(const std::string& fn, double a);
};
