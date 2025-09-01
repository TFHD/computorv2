#ifndef COMPUTOR_HPP
# define COMPUTOR_HPP

# include <iostream>
# include <string>
# include <map>
# include <algorithm>
# include <regex>
# include <vector>
# include "Complex.hpp"
# include "Matrice.hpp"
# include <readline/readline.h>
# include <readline/history.h>
# include <iomanip>
# include <sstream>

enum TokenType { NUMBER, OPERATOR, PAREN_LEFT, PAREN_RIGHT, VARIABLE, FUNCTION, MATRICE, COMPLEXS, NO_TYPE };

struct Token {
    TokenType type = NO_TYPE;
    std::string var = "0";
    std::string functionVar = "";
    double value = 0;
    char op = '0';
    Complex cplx_value = {};
    Matrice mat = {};
};

enum ValueType { SCALAR, MATRIX, COMPLEX, SYMBOLIC };

struct Value {
    ValueType type;
    double scalar;
    Complex cplx;
    Matrice matrix;
    std::string symbolic_expr;

    Value(double d) : type(ValueType::SCALAR), scalar(d) {}
    Value(const Matrice& m) : type(ValueType::MATRIX), matrix(m) {}
    Value(const Complex& cplx) : type(ValueType::COMPLEX), cplx(cplx) {}
    Value(const std::string& expr) : type(ValueType::SYMBOLIC), symbolic_expr(expr) {}
};

enum type {
    RATIONAL_EXPR,
    COMPLEX_EXPR,
    MATRICIAL_EXPR,
    POLYNOMIAL_EXPR,
    FUNCTION_EXPR,
    RESOLUTION_EXPR
};

struct data {
    std::string expr;
    std::string functionVar;
    int type;
    std::vector<Token> tokens;
};

class Computor {

    private :
        std::map<std::string, data> map;
    public :
        Computor(void);
        ~Computor(void);
        void parsingExpr(std::string &text);
        void printMap(void);

        Value evalRPN(const std::vector<Token> &rpn);

        void polynomeHandler(std::string &name, std::vector<Token> &tokens);
        bool ParseToToken(std::string &expr, std::vector<Token> &tokens, type type);
        void insertInfosInMap(std::string &name, data &data);
        bool functionHandler(std::string &name, std::string &expr, data &data);
        bool extractVariables(std::vector<Token> &tokens);
        std::vector<Token> toRPN(const std::vector<Token> &tokens);
};

#endif