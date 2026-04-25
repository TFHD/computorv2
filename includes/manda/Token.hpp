#ifndef TOKEN_HPP
# define TOKEN_HPP

# include "Complex.hpp"
# include "Matrice.hpp"

enum ExpressionType {
    RATIONAL_EXPR,
    COMPLEX_EXPR,
    MATRICIAL_EXPR,
    POLYNOMIAL_EXPR,
    RESOLUTION_EXPR,
    FUNCTION_EXPR
};

enum TokenType { NUMBER, OPERATOR, PAREN_LEFT, PAREN_RIGHT, VARIABLE, FUNCTION, MATRICE, COMPLEXS, NO_TYPE };

struct Token {
    TokenType type = NO_TYPE;
    std::string var = "0";
    std::string functionVar = "";
    double value = 0;
    char op = '0';
    Complex cplx_value = {};
    Matrice mat = {};

    static bool                 parseToToken(std::string &expr, std::vector<Token> &tokens, ExpressionType type);
    static std::string          tokenToString(std::vector<Token> &tokens);
    static int                  matriceHandler(std::string expr, std::vector<Token> &tokens);
    static bool                 complexHandler(std::vector<Token> &tokens);
    static std::vector<Token>   toRPN(const std::vector<Token> &tokens);
};

typedef std::vector<Token> Tokens;

#endif