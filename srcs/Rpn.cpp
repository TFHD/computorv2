#include "Computor.hpp"

Value apply_symbolic_scalar_operation(const Value &a, const Value &b, const std::string &op) {
    if (a.type != ValueType::SYMBOLIC || b.type != ValueType::SCALAR) {
        throw std::runtime_error("Error symbolic-scalar operation");
    }
    SymbolicExpr res;
    if (a.symbolic.coeffs.size() == 1 && a.symbolic.constant == 0) {
        auto var = a.symbolic.coeffs.begin();
        if (var->second == 1.0) {
            res.coeffs[var->first + " " + op + " " + std::to_string((int)b.scalar)] = 1;
        } else {
            std::string expr = printFormat(const_cast<Value&>(a));
            res.coeffs["(" + expr + ") " + op + " " + std::to_string((int)b.scalar)] = 1;
        }
    } else {
        std::string expr = printFormat(const_cast<Value&>(a));
        res.coeffs["(" + expr + ") " + op + " " + std::to_string((int)b.scalar)] = 1;
    }
    
    return Value(res);
}

Value apply_scalar_symbolic_operation(const Value &a, const Value &b, const std::string &op) {
    if (a.type != ValueType::SCALAR || b.type != ValueType::SYMBOLIC) {
        throw std::runtime_error("Error scalar-symbolic operation");
    }
    SymbolicExpr res;
    if (b.symbolic.coeffs.size() == 1 && b.symbolic.constant == 0) {
        auto var = b.symbolic.coeffs.begin();
        if (var->second == 1.0) {
            res.coeffs[std::to_string((int)a.scalar) + " " + op + " " + var->first] = 1;
        } else {
            std::string expr = printFormat(const_cast<Value&>(b));
            res.coeffs[std::to_string((int)a.scalar) + " " + op + " (" + expr + ")"] = 1;
        }
    } else if (b.symbolic.coeffs.empty() && b.symbolic.constant != 0) {
        if (op == "/") {
            res.constant = a.scalar / b.symbolic.constant;
        } else if (op == "%") {
            res.constant = Math::mod(a.scalar, b.symbolic.constant);
        } else if (op == "^") {
            res.constant = Math::pow(a.scalar, (int)b.symbolic.constant);
        }
        return Value(res);
    } else {
        std::string expr = printFormat(const_cast<Value&>(b));
        res.coeffs[std::to_string((int)a.scalar) + " " + op + " (" + expr + ")"] = 1;
    }
    
    return Value(res);
}

Value apply_symbolic_symbolic_operation(const Value &a, const Value &b, const std::string &op) {
    if (a.type != ValueType::SYMBOLIC || b.type != ValueType::SYMBOLIC) {
        throw std::runtime_error("Error symbolic-symbolic operation");
    }
    if (a.symbolic.coeffs.empty() && b.symbolic.coeffs.empty()) {
        if (op == "/") {
            if (b.symbolic.constant != 0)
                return Value(a.symbolic.constant / b.symbolic.constant);
        } else if (op == "%") {
            if (b.symbolic.constant != 0)
                return Value(Math::mod(a.symbolic.constant, b.symbolic.constant));
        } else if (op == "^")
            return Value(Math::pow(a.symbolic.constant, (int)b.symbolic.constant));
    }
    std::string exprA = printFormat(const_cast<Value&>(a));
    std::string exprB = printFormat(const_cast<Value&>(b));
    SymbolicExpr res;
    res.coeffs["(" + exprA + ") " + op + " (" + exprB + ")"] = 1;
    return Value(res);
}

Value apply_plus(const Value &a, const Value &b) {
    if (a.type == ValueType::SCALAR && b.type == ValueType::SCALAR)
        return Value(a.scalar + b.scalar);
    if (a.type == ValueType::MATRIX && b.type == ValueType::MATRIX)
        return Value(a.matrix + b.matrix);
    if (a.type == ValueType::SCALAR && b.type == ValueType::COMPLEX)
        return Value(a.scalar + b.cplx);
    if (a.type == ValueType::COMPLEX && b.type == ValueType::SCALAR)
        return Value(a.cplx + b.scalar);
    if (a.type == ValueType::COMPLEX && b.type == ValueType::COMPLEX)
        return Value(a.cplx + b.cplx);
    // Ajout pour le symbolique
    if (a.type == ValueType::SYMBOLIC && b.type == ValueType::SYMBOLIC) {
        SymbolicExpr res = a.symbolic;
        for (const auto& [var, coeff] : b.symbolic.coeffs)
            res.coeffs[var] += coeff;
        res.constant += b.symbolic.constant;
        return Value(res);
    }
    if (a.type == ValueType::SYMBOLIC && b.type == ValueType::SCALAR) {
        SymbolicExpr res = a.symbolic;
        res.constant += b.scalar;
        return Value(res);
    }
    if (a.type == ValueType::SCALAR && b.type == ValueType::SYMBOLIC) {
        SymbolicExpr res = b.symbolic;
        res.constant += a.scalar;
        return Value(res);
    }
    throw std::runtime_error("Error addition");
}

Value apply_minus(const Value &a, const Value &b) {
    if (a.type == ValueType::SCALAR && b.type == ValueType::SCALAR)
        return Value(a.scalar - b.scalar);
    if (a.type == ValueType::MATRIX && b.type == ValueType::MATRIX)
        return Value(a.matrix - b.matrix);
    if (a.type == ValueType::SCALAR && b.type == ValueType::COMPLEX)
        return Value(a.scalar - b.cplx);
    if (a.type == ValueType::COMPLEX && b.type == ValueType::SCALAR)
        return Value(a.cplx - b.scalar);
    if (a.type == ValueType::COMPLEX && b.type == ValueType::COMPLEX)
        return Value(a.cplx - b.cplx);
    if (a.type == ValueType::SYMBOLIC && b.type == ValueType::SYMBOLIC) {
        SymbolicExpr res = a.symbolic;
        for (const auto& [var, coeff] : b.symbolic.coeffs)
            res.coeffs[var] -= coeff;
        res.constant -= b.symbolic.constant;
        return Value(res);
    }
    if (a.type == ValueType::SYMBOLIC && b.type == ValueType::SCALAR) {
        SymbolicExpr res = a.symbolic;
        res.constant -= b.scalar;
        return Value(res);
    }
    if (a.type == ValueType::SCALAR && b.type == ValueType::SYMBOLIC) {
        SymbolicExpr res = b.symbolic;
        for (auto& [var, coeff] : res.coeffs) coeff = -coeff;
        res.constant = a.scalar - res.constant;
        return Value(res);
    }
    throw std::runtime_error("Error subtraction");
}

Value apply_times(const Value &a, const Value &b) {
    if (a.type == ValueType::SCALAR && b.type == ValueType::SCALAR)
        return Value(a.scalar * b.scalar);
    if (a.type == ValueType::MATRIX && b.type == ValueType::MATRIX)
        return Value(a.matrix * b.matrix);
    if (a.type == ValueType::SCALAR && b.type == ValueType::COMPLEX)
        return Value(a.scalar * b.cplx);
    if (a.type == ValueType::COMPLEX && b.type == ValueType::SCALAR)
        return Value(a.cplx * b.scalar);
    if (a.type == ValueType::COMPLEX && b.type == ValueType::COMPLEX)
        return Value(a.cplx * b.cplx);
    // Ajout pour le symbolique
    if (a.type == ValueType::SYMBOLIC && b.type == ValueType::SYMBOLIC) {
        // Cas simple: mono-mono (ex: x*y)
        if (a.symbolic.coeffs.size() == 1 && b.symbolic.coeffs.size() == 1
            && a.symbolic.constant == 0 && b.symbolic.constant == 0) {
            std::string exprA = printFormat(const_cast<Value&>(a));
            std::string exprB = printFormat(const_cast<Value&>(b));
            SymbolicExpr monome;
            monome.coeffs["(" + exprA + ")*(" + exprB + ")"] = 1;
            return Value(monome);
        }
        throw std::runtime_error("Error multiplication");
    }
    if (a.type == ValueType::SYMBOLIC && b.type == ValueType::SCALAR) {
        SymbolicExpr res;
        for (const auto& [var, coeff] : a.symbolic.coeffs)
            res.coeffs[var] = coeff * b.scalar;
        res.constant = a.symbolic.constant * b.scalar;
        return Value(res);
    }
    if (a.type == ValueType::SCALAR && b.type == ValueType::SYMBOLIC) {
        SymbolicExpr res;
        for (const auto& [var, coeff] : b.symbolic.coeffs)
            res.coeffs[var] = coeff * a.scalar;
        res.constant = b.symbolic.constant * a.scalar;
        return Value(res);
    }
    throw std::runtime_error("Error multiplication");
}

Value apply_div(const Value &a, const Value &b) {
    if (a.type == ValueType::SCALAR && b.type == ValueType::SCALAR) {
        if (b.scalar == 0)
            throw std::runtime_error("Division by zero is forbidden");
        return Value(a.scalar / b.scalar);
    }
    if (a.type == ValueType::MATRIX && b.type == ValueType::SCALAR) 
        return Value(a.matrix / b.scalar);
    if (a.type == ValueType::SCALAR && b.type == ValueType::COMPLEX)
        return Value(a.scalar / b.cplx);
    if (a.type == ValueType::COMPLEX && b.type == ValueType::SCALAR)
        return Value(a.cplx / b.scalar);
    if (a.type == ValueType::COMPLEX && b.type == ValueType::COMPLEX)
        return Value(a.cplx / b.cplx);
    if (a.type == ValueType::SYMBOLIC && b.type == ValueType::SCALAR) {
        if (b.scalar == 0)
            throw std::runtime_error("Division by zero is forbidden");
        return apply_symbolic_scalar_operation(a, b, "/");
    }
    if (a.type == ValueType::SCALAR && b.type == ValueType::SYMBOLIC)
        return apply_scalar_symbolic_operation(a, b, "/");

    if (a.type == ValueType::SYMBOLIC && b.type == ValueType::SYMBOLIC)
        return apply_symbolic_symbolic_operation(a, b, "/");
    throw std::runtime_error("Error division");
}

Value apply_exp(const Value &a, const Value &b) {
    if (a.type == ValueType::SCALAR && b.type == ValueType::SCALAR)
        return Value(Math::pow(a.scalar, b.scalar));
    if (a.type == ValueType::SYMBOLIC && b.type == ValueType::SCALAR) {
        if (b.scalar == 0) {
            SymbolicExpr res;
            res.constant = 1;
            return Value(res);
        }
        if (b.scalar == 1)
            return Value(a.symbolic);

        if (b.scalar > 1 && std::floor(b.scalar) == b.scalar) {
            return apply_symbolic_scalar_operation(a, b, "^");
        }
        throw std::runtime_error("Error exponentiation");
    }
    if (a.type == ValueType::SCALAR && b.type == ValueType::SYMBOLIC)
        return apply_scalar_symbolic_operation(a, b, "^");
    if (a.type == ValueType::SYMBOLIC && b.type == ValueType::SYMBOLIC)
        return apply_symbolic_symbolic_operation(a, b, "^");

    throw std::runtime_error("Error exponentiation");
}


Value apply_mod(const Value &a, const Value &b) {
    if (a.type == ValueType::SCALAR && b.type == ValueType::SCALAR) {
        if (b.scalar == 0)
            throw std::runtime_error("Modulo by zero is forbidden");
        if (!isInteger(a.scalar) || !isInteger(b.scalar))
            throw std::runtime_error("Modulo with float is forbidden");
        return Value((int)(a.scalar) % (int)(b.scalar));
    }
    if (a.type == ValueType::SYMBOLIC && b.type == ValueType::SCALAR) {
        if (b.scalar == 0)
            throw std::runtime_error("Modulo by zero is forbidden");
        return apply_symbolic_scalar_operation(a, b, "%");
    }
    if (a.type == ValueType::SCALAR && b.type == ValueType::SYMBOLIC)
        return apply_scalar_symbolic_operation(a, b, "%");
    if (a.type == ValueType::SYMBOLIC && b.type == ValueType::SYMBOLIC)
        return apply_symbolic_symbolic_operation(a, b, "%");

    throw std::runtime_error("Error modulo");
}

Value apply_op(const Value &a, const Value &b, char op) {
    switch (op) {
    case '+':
        return apply_plus(a, b);
    case '-':
        return apply_minus(a, b);
    case '*':
        return apply_times(a, b);
    case '/':
        return apply_div(a, b);
    case '%':
        return apply_mod(a, b);
    case '^':
        return apply_exp(a, b);
    case '&':
        if (a.type == ValueType::MATRIX && b.type == ValueType::SCALAR)
            return Value(a.matrix * b.scalar);
        if (a.type == ValueType::SCALAR && b.type == ValueType::MATRIX)
            return Value(a.scalar * b.matrix);
        throw std::runtime_error("Error multiplication");
    }
    throw std::runtime_error("Unknown operator");
}

Value Computor::evalRPN(const Tokens &rpn) {
    std::stack<Value> s;
    for (const auto& token : rpn) {
        if (token.type == TokenType::NUMBER) {
            s.push(Value(token.value));
        } else if (token.type == TokenType::MATRICE) {
            s.push(Value(token.mat));
        } else if (token.type == TokenType::COMPLEXS) {
            s.push(Value(token.cplx_value));
        } else if (token.type == TokenType::VARIABLE) {
            SymbolicExpr expr;
            expr.coeffs[token.var] = 1.0;
            s.push(Value(expr));
        } else if (token.type == OPERATOR) {
            if (s.size() <= 1) throw std::runtime_error("invalid expression");
            Value b = s.top(); s.pop();
            Value a = s.top(); s.pop();
            s.push(apply_op(a, b, token.op));
        }
    }
    if (s.size() != 1) throw std::runtime_error("invalid expression");
    return s.top();
}