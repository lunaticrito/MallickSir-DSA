#include "expression_engine.h"
#include "stack_queue.h"
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <functional>

// Helpers 
bool ExpressionEngine::isOperator(const std::string &s)
{
    return s == "+" || s == "-" || s == "*" || s == "/" ||
           s == "^" || s == "%";
}

bool ExpressionEngine::isFunction(const std::string &s)
{
    static const std::vector<std::string> fns =
        {"sin", "cos", "tan", "asin", "acos", "atan", "sqrt", "log", "log2", "log10",
         "exp", "abs", "ceil", "floor", "round"};
    return std::find(fns.begin(), fns.end(), s) != fns.end();
}

int ExpressionEngine::precedence(const std::string &op)
{
    if (op == "+" || op == "-")
        return 1;
    if (op == "*" || op == "/" || op == "%")
        return 2;
    if (op == "^")
        return 3;
    if (isFunction(op))
        return 4;
    return 0;
}

bool ExpressionEngine::isLeftAssoc(const std::string &op)
{
    return op != "^"; // only ^ is right-associative
}

double ExpressionEngine::applyOp(const std::string &op, double a, double b)
{
    if (op == "+")
        return a + b;
    if (op == "-")
        return a - b;
    if (op == "*")
        return a * b;
    if (op == "/")
    {
        if (b == 0)
            throw std::runtime_error("Division by zero");
        return a / b;
    }
    if (op == "^")
        return std::pow(a, b);
    if (op == "%")
    {
        if (b == 0)
            throw std::runtime_error("Modulo by zero");
        return std::fmod(a, b);
    }
    throw std::runtime_error("Unknown operator: " + op);
}

double ExpressionEngine::applyFunc(const std::string &fn, double a)
{
    if (fn == "sin")
        return std::sin(a);
    if (fn == "cos")
        return std::cos(a);
    if (fn == "tan")
        return std::tan(a);
    if (fn == "asin")
        return std::asin(a);
    if (fn == "acos")
        return std::acos(a);
    if (fn == "atan")
        return std::atan(a);
    if (fn == "sqrt")
        return std::sqrt(a);
    if (fn == "log")
        return std::log(a);
    if (fn == "log2")
        return std::log2(a);
    if (fn == "log10")
        return std::log10(a);
    if (fn == "exp")
        return std::exp(a);
    if (fn == "abs")
        return std::abs(a);
    if (fn == "ceil")
        return std::ceil(a);
    if (fn == "floor")
        return std::floor(a);
    if (fn == "round")
        return std::round(a);
    throw std::runtime_error("Unknown function: " + fn);
}

// Tokeniser 
std::vector<Token> ExpressionEngine::tokenise(const std::string &expr)
{
    std::vector<Token> tokens;
    int i = 0, n = (int)expr.size();

    while (i < n)
    {
        if (std::isspace(expr[i]))
        {
            ++i;
            continue;
        }

        // Number (integer or float)
        if (std::isdigit(expr[i]) || (expr[i] == '.' && i + 1 < n && std::isdigit(expr[i + 1])))
        {
            int j = i;
            while (j < n && (std::isdigit(expr[j]) || expr[j] == '.'))
                ++j;
            tokens.push_back({Token::Type::Number, expr.substr(i, j - i)});
            i = j;
            continue;
        }

        // Identifier: function or variable
        if (std::isalpha(expr[i]) || expr[i] == '_')
        {
            int j = i;
            while (j < n && (std::isalnum(expr[j]) || expr[j] == '_'))
                ++j;
            std::string id = expr.substr(i, j - i);
            tokens.push_back({isFunction(id) ? Token::Type::Function : Token::Type::Variable, id});
            i = j;
            continue;
        }

        // Parentheses
        if (expr[i] == '(')
        {
            tokens.push_back({Token::Type::LeftParen, "("});
            ++i;
            continue;
        }
        if (expr[i] == ')')
        {
            tokens.push_back({Token::Type::RightParen, ")"});
            ++i;
            continue;
        }

        // Operators (handle unary minus)
        if (expr[i] == '-')
        {
            bool unary = tokens.empty() ||
                         tokens.back().type == Token::Type::Operator ||
                         tokens.back().type == Token::Type::LeftParen;
            if (unary)
            {
                // Represent unary minus as "0" "-"
                tokens.push_back({Token::Type::Number, "0"});
                tokens.push_back({Token::Type::Operator, "-"});
            }
            else
            {
                tokens.push_back({Token::Type::Operator, "-"});
            }
            ++i;
            continue;
        }

        // Other single-char operators
        std::string op(1, expr[i]);
        if (isOperator(op))
        {
            tokens.push_back({Token::Type::Operator, op});
            ++i;
            continue;
        }

        throw std::runtime_error(std::string("Unknown character: ") + expr[i]);
    }
    return tokens;
}

// Shunting-Yard → Postfix 
std::string ExpressionEngine::infixToPostfix(const std::string &expr)
{
    auto tokens = tokenise(expr);
    Stack opStack;
    Queue outQueue;

    for (auto &tok : tokens)
    {
        if (tok.type == Token::Type::Number || tok.type == Token::Type::Variable)
        {
            outQueue.enqueue(tok.value);
        }
        else if (tok.type == Token::Type::Function)
        {
            opStack.push(tok.value);
        }
        else if (tok.type == Token::Type::Operator)
        {
            while (!opStack.empty())
            {
                const auto &top = opStack.peek();
                if (top == "(")
                    break;
                if (isFunction(top))
                {
                    outQueue.enqueue(opStack.pop());
                    continue;
                }
                if (isOperator(top) &&
                    (precedence(top) > precedence(tok.value) ||
                     (precedence(top) == precedence(tok.value) && isLeftAssoc(tok.value))))
                    outQueue.enqueue(opStack.pop());
                else
                    break;
            }
            opStack.push(tok.value);
        }
        else if (tok.type == Token::Type::LeftParen)
        {
            opStack.push("(");
        }
        else if (tok.type == Token::Type::RightParen)
        {
            while (!opStack.empty() && opStack.peek() != "(")
                outQueue.enqueue(opStack.pop());
            if (opStack.empty())
                throw std::runtime_error("Mismatched parentheses");
            opStack.pop(); // pop "("
            if (!opStack.empty() && isFunction(opStack.peek()))
                outQueue.enqueue(opStack.pop());
        }
    }
    while (!opStack.empty())
    {
        if (opStack.peek() == "(" || opStack.peek() == ")")
            throw std::runtime_error("Mismatched parentheses");
        outQueue.enqueue(opStack.pop());
    }

    // Drain queue to string
    std::ostringstream oss;
    while (!outQueue.empty())
    {
        if (oss.tellp())
            oss << " ";
        oss << outQueue.dequeue();
    }
    return oss.str();
}

// Infix → Prefix 
std::string ExpressionEngine::infixToPrefix(const std::string &expr)
{
    // Reverse token stream, swap parens, get postfix, reverse result
    auto tokens = tokenise(expr);
    std::string rev;
    for (int i = (int)tokens.size() - 1; i >= 0; --i)
    {
        if (!rev.empty())
            rev += " ";
        const auto &v = tokens[i].value;
        if (v == "(")
            rev += ")";
        else if (v == ")")
            rev += "(";
        else
            rev += v;
    }
    std::string postRev = infixToPostfix(rev);
    // Reverse token order
    std::vector<std::string> parts;
    std::istringstream iss(postRev);
    std::string t;
    while (iss >> t)
        parts.push_back(t);
    std::reverse(parts.begin(), parts.end());
    std::ostringstream oss;
    for (int i = 0; i < (int)parts.size(); ++i)
    {
        if (i)
            oss << " ";
        oss << parts[i];
    }
    return oss.str();
}

// Evaluate Postfix
double ExpressionEngine::evalPostfix(const std::string &postfix, const VarMap &vars)
{
    Stack s;
    std::istringstream iss(postfix);
    std::string tok;
    while (iss >> tok)
    {
        if (isOperator(tok))
        {
            double b = std::stod(s.pop());
            double a = std::stod(s.pop());
            s.push(std::to_string(applyOp(tok, a, b)));
        }
        else if (isFunction(tok))
        {
            double a = std::stod(s.pop());
            s.push(std::to_string(applyFunc(tok, a)));
        }
        else
        {
            // number or variable
            double val;
            try
            {
                val = std::stod(tok);
            }
            catch (...)
            {
                auto it = vars.find(tok);
                if (it == vars.end())
                    throw std::runtime_error("Undefined variable: " + tok);
                val = it->second;
            }
            s.push(std::to_string(val));
        }
    }
    if (s.size() != 1)
        throw std::runtime_error("Invalid postfix expression");
    return std::stod(s.pop());
}

// Evaluate Prefix 
double ExpressionEngine::evalPrefix(const std::string &prefix, const VarMap &vars)
{
    // Read tokens into a vector, evaluate right-to-left with a stack
    std::vector<std::string> tokens;
    std::istringstream iss(prefix);
    std::string tok;
    while (iss >> tok)
        tokens.push_back(tok);

    Stack s;
    for (int i = (int)tokens.size() - 1; i >= 0; --i)
    {
        const auto &t = tokens[i];
        if (isOperator(t))
        {
            double a = std::stod(s.pop());
            double b = std::stod(s.pop());
            s.push(std::to_string(applyOp(t, a, b)));
        }
        else if (isFunction(t))
        {
            double a = std::stod(s.pop());
            s.push(std::to_string(applyFunc(t, a)));
        }
        else
        {
            double val;
            try
            {
                val = std::stod(t);
            }
            catch (...)
            {
                auto it = vars.find(t);
                if (it == vars.end())
                    throw std::runtime_error("Undefined variable: " + t);
                val = it->second;
            }
            s.push(std::to_string(val));
        }
    }
    if (s.size() != 1)
        throw std::runtime_error("Invalid prefix expression");
    return std::stod(s.pop());
}

double ExpressionEngine::evalInfix(const std::string &expr, const VarMap &vars)
{
    return evalPostfix(infixToPostfix(expr), vars);
}
