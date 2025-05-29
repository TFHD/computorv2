#include "../includes/Computor.hpp"

Computor::Computor(void) {}
Computor::~Computor(void) {}


std::string typeToString(int type) {
    if (type == 0)
        return "VARIABLE";
    else if (type == 1)
        return "NUMBER";
    else if (type == 2)
        return "OPERATOR";
    return "NO TYPE";
}

bool isOperator(char c) {
    if (c == '*' || c == '+' || c == '-' || c == '/')
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


bool Computor::createToken(std::string &str) {
    Token token;
    TokenType type;
    if (isOperator(str[0]))
        type = OPERATOR;
    else if (isVariable(str))
        type = VARIABLE;
    else if (std::isdigit(str[0]))
        type = NUMBER;
    else { std::cout << "i don't understand this shit" << std::endl; return false; }

    token.type = type;
    token.value = str;
    this->lexer.tokens.push_back(token);
    return true;

}

void Computor::ParseToToken(std::string &expr) {
    std::string substr = "";
    for (int i = 0 ; i < (int)expr.length(); i++) {
        if (std::isalpha(expr[i]))
            substr.push_back(expr[i]);
        if (!std::isalpha(expr[i]) || i + 1 >= (int)expr.length()) {
            if (!substr.length())
                substr.push_back(expr[i]);
            bool res = createToken(substr);
            substr.clear();
            if (!res)
                return ;
        }
    }
}


void Computor::parsingExpr(std::string &text) {
    text.erase(remove(text.begin(), text.end(), ' '), text.end());

    unsigned long it = text.find('=');
    if (it == std::string::npos) { std::cout << "invalid expression" << std::endl; return; }

    std::string name        = text.substr(0, it);
    std::string expr        = text.substr(it + 1, text.length());
    data data = {expr, RATIONAL_EXPR};

    if (this->map.find(name) == this->map.end()) {
        this->map.insert({name, data});
    }
    else {
        auto it = this->map.find(name);
        it->second = data;
    }
    ParseToToken(expr);
    for (int i = 0; i < (int)this->lexer.tokens.size(); i++) {
        std::cout << "TOKEN : " << this->lexer.tokens[i].value << "\n" << "TOKEN TYPE : " << typeToString(this->lexer.tokens[i].type) << std::endl << std::endl;
    }
}

data Computor::determineType(std::string &name, std::string &expr) {

    int type1 = -1;
    int type2 = -1;
    data data;
    (void)name;
    (void)expr;
    (void)type2;
    std::regex name_expr("^([a-zA-Z]+)$");
    if (std::regex_match(name, name_expr)) {
        std::regex name_expr("^([a-zA-Z]+)(\\()([a-zA-Z]+)(\\))$");
        if (std::regex_match(name, name_expr))
            type1 = FUNCTION_EXPR;
        else
            type1 = RATIONAL_EXPR;
    }
    else {
        std::cout << "Invalid expression!" << std::endl; return data;
    }
    std::regex math_expr(R"(^\s*(-?\d+[a-zA-Z]+|-?\d+|-?[a-zA-Z]+|\(\s*(-?\d+[a-zA-Z]+|-?\d+|-?[a-zA-Z]+)\s*\))(\s*[+\-*\/\^%]\s*(-?\d+[a-zA-Z]+|-?\d+|-?[a-zA-Z]+|\(\s*(-?\d+[a-zA-Z]+|-?\d+|-?[a-zA-Z]+)\s*\)))*\s*$)");
    if (std::regex_match(expr, math_expr)) {
        type2 = FUNCTION_EXPR;
    }
    std::regex math_expr1(R"(^\s*(-?\d+(\.\d+)?|\(\s*-?\d+(\.\d+)?\s*\))(\s*([+\-*\/\^%]\s*(-?\d+(\.\d+)?|\(\s*-?\d+(\.\d+)?\s*\)))*\s*$))");
    if (std::regex_match(expr, math_expr1) && type2 != -1)
        type2 = RATIONAL_EXPR;
    return data;
}

void Computor::printMap(void) {
    for (auto it = this->map.begin(); it != this->map.end(); ++it) {
        std::cout << " {" << it->first << ", " << it->second.expr << "} " << std::endl;
    }
}