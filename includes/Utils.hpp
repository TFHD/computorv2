#ifndef UTILS_HPP
# define UTILS_HPP

# include "Token.hpp"
# include <map>
# include <sstream>
# include <cmath>
# include <iomanip>
# include <algorithm>

struct SymbolicExpr {
    std::map<std::string, double> coeffs;
    double constant = 0;
};

enum ValueType {
    SCALAR,
    MATRIX,
    COMPLEX,
    SYMBOLIC
};

struct Value {
    ValueType type;
    double scalar;
    Complex cplx;
    Matrice matrix;
    std::string symbolic_expr;
    SymbolicExpr symbolic;

    Value() {}
    Value(double d) : type(ValueType::SCALAR), scalar(d) {}
    Value(const Matrice &m) : type(ValueType::MATRIX), matrix(m) {}
    Value(const Complex &cplx) : type(ValueType::COMPLEX), cplx(cplx) {}
    Value(const SymbolicExpr &s) : type(SYMBOLIC), symbolic(s) {}
    
};

struct data {
    std::string expr;
    std::string functionVar;
    int type;
    std::vector<Token> tokens;
};

bool        isOperator(char c);
bool        isVariable(std::string &str);
int         precedence(char op);
bool        isLeftAssoc(char op);
bool        hasVariable(std::vector<Token> &tokens);
std::string str_toupper(std::string str);
bool        isAlphaString(std::string &str);
bool        isInteger(double x);
std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);
bool        CheckParenthesis(std::string &expr);
std::string printFormat(Value &val);

std::string typeToString(int type);
void        printTable(const std::vector<Token>& tokens);
std::string betterPrint(std::string str);
std::string &NoSpace(std::string str);


#endif