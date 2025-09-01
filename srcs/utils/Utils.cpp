#include "Utils.hpp"

bool isOperator(char c) {
    if (c == '*' || c == '+' || c == '-' || c == '/' || c == '%' || c == '^')
        return true;
    return false;
}

bool isVariable(std::string &str) {
    for (int i = 0; i < (int)str.length(); i++) {
        if (!std::isalpha(str[i]))
            return false;
    }
    return true;
}

int precedence(char op) {
    if (op == '^') return 3;
    if (op == '*' || op == '/' || op == '%' || op == '&') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

bool isLeftAssoc(char op) {
    return op != '^';
}

bool hasVariable(std::vector<Token> &tokens){
    for (int i = 0; i < (int)tokens.size(); i++) {
        if (tokens[i].type == VARIABLE || tokens[i].type == FUNCTION)
            return true;
    }
    return false;
}

std::string str_toupper(std::string str) {
    std::string new_str = "";
    for(size_t i = 0; i < str.size(); i++) {
        new_str.push_back(std::toupper(str[i]));
    }
    return new_str;
}

bool isAlphaString(std::string &str) {
    for (char c : str) {
        if (!std::isalpha(c))
            return false;
    }
    return true;
}

bool isInteger(double x) {
    return static_cast<long long>(x) == x;
}

void printFormat(Value &val) {
    if (val.type == ValueType::MATRIX)
        val.matrix.printMatrice();
    else if (val.type == ValueType::SCALAR)
        std::cout << val.scalar << std::endl;
    else if (val.type == ValueType::COMPLEX)
        std::cout << val.cplx << std::endl;
}

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    if(from.empty())
        return str;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}
bool CheckParenthesis(std::string &expr) {
    int open = 0;

    for (size_t i = 0; i < expr.size(); i++) {
        if (expr[i] == '(')
            open++;
        else if (expr[i] == ')')
            open--;
    }
    return !open;
}

std::string betterPrint(std::string str) {
    for (size_t i = 0; i < str.size(); i ++) {
        if ((isOperator(str[i]) || str[i] == '=') && str[i] != '^') {
            str.insert(str.begin() + i, ' ');
            str.insert(str.begin() + i + 2, ' ');
            i += 2;
        }
    }
    return str;
}
