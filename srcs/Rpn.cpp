#include "Computor.hpp"

Value apply_op(const Value &a, const Value &b, char op) {
    switch (op) {
    case '+':
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
        throw std::runtime_error("Addition: incompatible types");
    case '-':
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
        throw std::runtime_error("Soustraction: incompatible types");
    case '*':
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
        throw std::runtime_error("Multiplication: incompatible types");
    case '/':
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
        throw std::runtime_error("Division: incompatible types");
    case '%':
        if (a.type == ValueType::SCALAR && b.type == ValueType::SCALAR) {
            if (b.scalar == 0)
                throw std::runtime_error("Modulo by zero is forbidden");
            if (!isInteger(a.scalar) || !isInteger(b.scalar))
                throw std::runtime_error("Modulo with float is forbidden");
            return Value((int)(a.scalar) % (int)(b.scalar));
        }
        throw std::runtime_error("Modulo: only for scalars");
    case '^':
        if (a.type == ValueType::SCALAR && b.type == ValueType::SCALAR)
            return Value(Math::pow(a.scalar, b.scalar));
        throw std::runtime_error("Exponentiation: only for scalars");
    case '&':
        if (a.type == ValueType::MATRIX && b.type == ValueType::SCALAR)
            return Value(a.matrix * b.scalar);
        if (a.type == ValueType::SCALAR && b.type == ValueType::MATRIX)
            return Value(a.scalar * b.matrix);
        throw std::runtime_error("Multiplication: incompatible types");
    }
    throw std::runtime_error("Unknown operator");
}

Value Computor::evalRPN(const std::vector<Token> &rpn) {
    std::stack<Value> s;
    for (const auto& token : rpn) {
        if (token.type == TokenType::NUMBER) {
            s.push(Value(token.value));
        } else if (token.type == TokenType::MATRICE) {
            s.push(Value(token.mat));
        } else if (token.type == TokenType::COMPLEXS) {
            s.push(Value(token.cplx_value));
        } else if (token.type == OPERATOR) {
            if (s.size() <= 1) throw std::runtime_error("invalid expression RPN");
            Value b = s.top(); s.pop();
            Value a = s.top(); s.pop();
            s.push(apply_op(a, b, token.op));
        }
    }
    if (s.size() != 1) throw std::runtime_error("invalid expression RPN 2");
    return s.top();
}