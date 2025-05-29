#ifndef COMPUTOR_HPP
# define COMPUTOR_HPP

# include <iostream>
# include <string>
# include <map>
# include <algorithm>
# include <regex>
# include <vector>

enum TokenType {
    VARIABLE,
    NUMBER,
    OPERATOR
};

struct Token {
    std::string value;
    TokenType type;
};

struct Lexer {
    std::vector<Token> tokens;
};

enum type {
    RATIONAL_EXPR,
    COMPLEX_EXPR,
    MATRICIAL_EXPR,
    POLYNOMIAL_EXPR,
    FUNCTION_EXPR,
};

struct complex {

    double x;
    double y;
};

struct data {
    std::string expr;
    int type;
    complex complex = {0, 0};
    int x = 0;
};

class Computor {

    private :
        std::map<std::string, data> map;
        Lexer lexer;
    public :
        Computor(void);
        ~Computor(void);
        void parsingExpr(std::string &text);
        void printMap(void);
        bool createToken(std::string &str);
        void ParseToToken(std::string &expr);
        data determineType(std::string &name, std::string &expr);
};



#endif