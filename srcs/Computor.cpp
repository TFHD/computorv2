#include "../includes/Computor.hpp"
#include "../includes/Polynome_bonus.hpp"

Computor::Computor(void) {}
Computor::~Computor(void) {}


std::string typeToString(int type) {
    if (!type)
        return ("NUMBER");
    else if (type == 1)
        return "OPERATOR";
    else if (type == 2)
        return "PARENT_LEFT";
    else if (type == 3)
        return "PARENT_RIGHT";
    else if (type == 4)
        return "VARIABLE";
    else if (type == 5)
        return "FUNCTION";
    else if (type == 6)
        return "MATRICE";
    else if (type == 7)
        return "COMPLEXS";
    return "NO TYPE";
}

void printTable(const std::vector<Token>& tokens) {
    const int wValue = 11;
    const int wOp = 10;
    const int wType = 14;
    const int wVar = 18;

    const int totalWidth = wValue + wOp + wType + wVar + 5;
    (void)totalWidth;

    auto printSeparator = [&]() {
        std::cout << "+" << std::string(wValue, '-') 
                  << "+" << std::string(wOp, '-') 
                  << "+" << std::string(wType, '-') 
                  << "+" << std::string(wVar, '-') 
                  << "+" << std::endl;
    };

    printSeparator();
    std::cout << "|" << std::setw(wValue) << std::left << "VALUE"
              << "|" << std::setw(wOp) << std::left << "OP"
              << "|" << std::setw(wType) << std::left << "TYPE"
              << "|" << std::setw(wVar) << std::left << "VAR"
              << "|" << std::endl;
    printSeparator();

    for (const auto& t : tokens) {
        std::cout << "|" << std::setw(wValue) << std::left << t.value
                  << "|" << std::setw(wOp) << std::left << t.op
                  << "|" << std::setw(wType) << std::left << typeToString(t.type)
                  << "|" << std::setw(wVar) << std::left << t.var
                  << "|" << std::endl;
    }
    printSeparator();
}

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

double pow(double x, int e) {

    double res = 1;
    bool neg = e < 0;
    if (neg)
        e = -e;
    while (e-- > 0) {
        res *= x;
    }
    if (neg)
        return 1 / res;
    return res;
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

void Computor::insertInfosInMap(std::string &name, data &data) {
    if (this->map.find(name) == this->map.end()) {
        this->map.insert({name, data});
    }
    else {
        auto it = this->map.find(name);
        it->second = data;
    }
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

int matriceHandler(std::string expr, std::vector<Token> &tokens) { 
    std::regex reg_expr("(\\[\\[[-+]?([0-9.]+)(,[-+]?[0-9.]+)*\\](;\\[([-+]?[0-9.]+)(,[-+]?[0-9.]+)*\\])*\\])");
    std::smatch matchs;
    if (std::regex_search(expr, matchs, reg_expr))
    {
        tokens.push_back({MATRICE, matchs[1].str()});
        int last_tkn = tokens.size() - 1;
        int str_size = matchs[1].str().size();
        std::regex sub_expr(R"(\[[-+]?[0-9.]+(,[-+]?[[0-9.]+)*\])");
        std::string match_str = matchs[1].str();
        auto sub_begin = std::sregex_iterator(match_str.begin(), match_str.end(), sub_expr);
        auto sub_end = std::sregex_iterator();
        int k = 0;
        for (std::sregex_iterator i = sub_begin; i != sub_end; ++i) {
            std::smatch match = *i;
            for (size_t j = 0; j < match.str().size(); j++) {
                std::string num = "";
                if (std::isdigit(match.str()[j]) || match.str()[j] == '+' || match.str()[j] == '-') {
                    size_t j_copy = j++;
                    while (j < match.str().size() && (std::isdigit(match.str()[j]) || match.str()[j] == '.'))
                        ++j;
                    num = match.str().substr(j_copy, j - j_copy);
                    if ((int)tokens[last_tkn].mat.getMat().size() <= k) {
                        tokens[last_tkn].mat.getMat().resize(k + 1);
                    }
                    tokens[last_tkn].mat.getMat()[k].push_back(std::strtod(num.c_str(), 0));
                }
            }
            k++;
        }
        return str_size;
    }
    return 0;
}

bool ComplexHandler(std::vector<Token> &tokens) {
    bool    isComplex = false;
    int     complexIndex = 0;
    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i].type == TokenType::VARIABLE && tokens[i].var == "i") {
            isComplex = true;
            complexIndex = i;
            break;
        }
    }
    if (!isComplex)
        return false;
    
    Complex cplx(0, 1);

    int operatorIndex = complexIndex;
    int numberIndex = complexIndex;

    for (int i = operatorIndex; i > -1; i--) {
        if (tokens[i].type == TokenType::OPERATOR) {
            operatorIndex = i;
            break;
        }
    }
    for (int i = numberIndex; i > -1; i--) {
        if (tokens[i].type == TokenType::NUMBER) {
            numberIndex = i;
            break;
        }
    }
    if (numberIndex >= 0 && tokens[operatorIndex].op == '*') {
        cplx.setIm(tokens[numberIndex].value);
        tokens.erase(tokens.begin() + numberIndex, tokens.begin() + complexIndex + 1);
        tokens.insert(tokens.begin() + numberIndex, {{PAREN_LEFT},{COMPLEXS, "0", "0", 0, '0', cplx}});
    } else if (numberIndex >= 0 && tokens[operatorIndex].op == '+') {
        tokens.erase(tokens.begin() + complexIndex);
        tokens.insert(tokens.begin() + complexIndex, {COMPLEXS, "0", "0", 0, '0', cplx});

    } else if (numberIndex == complexIndex || operatorIndex == complexIndex) {
        tokens.erase(tokens.begin() + complexIndex);
        tokens.insert(tokens.begin() + complexIndex, {COMPLEXS, "0", "0", 0, '0', cplx});
    } else {
        return false;
    }
    ComplexHandler(tokens);
    return true;
}

bool Computor::ParseToToken(std::string &expr, std::vector<Token> &tokens, type type) {
    for (size_t i = 0; i < expr.size();) {
        if (std::isalpha(expr[i])) {
            size_t j = i;
            while (j < expr.size() && std::isalpha(expr[j]))
                ++j;
            tokens.push_back({PAREN_LEFT});
            if (expr[j] == '(') {
                size_t j_copy = j;
                while (j < expr.size() && expr[j] != ')')
                    ++j;
                tokens.push_back({FUNCTION, expr.substr(i, j_copy - i + 1), expr.substr(j_copy + 1, j - j_copy - 1)});
                ++j;
            }
            else
                tokens.push_back({VARIABLE, expr.substr(i, j - i)});
            tokens.push_back({PAREN_RIGHT});
            i = j;
        }
        else if (std::isdigit(expr[i]) || expr[i] == '.') {
            size_t j = i;
            while (j < expr.size() && (std::isdigit(expr[j]) || expr[j] == '.'))
                ++j;
            tokens.push_back({NUMBER, "0", "0",std::stod(expr.substr(i, j - i))});
            if (j < expr.size() && (std::isalpha(expr[j]) || expr[j] == '('))
                tokens.push_back({OPERATOR, "0", "0", 0, '*'});
            i = j;
        } else if (expr[i] == '(') {
            tokens.push_back({PAREN_LEFT});
            ++i;
        } else if (expr[i] == '[') {
            int j = matriceHandler(expr.substr(i, expr.size() - i), tokens);
            if (!j)
                return false;
            i += j;
            continue;
        } else if (expr[i] == ')') {
            tokens.push_back({PAREN_RIGHT});
            ++i;
        } else if (isOperator(expr[i])){
            if (i + 1 < expr.size() && (std::isdigit(expr[i + 1]) || std::isalpha(expr[i + 1])) && (expr[i] == '-' || expr[i] == '+')) {
                size_t j = ++i;
                if (std::isdigit(expr[i])) {
                    while (j < expr.size() && (std::isdigit(expr[j]) || expr[j] == '.'))
                        ++j;
                    if (tokens.size() != 0 && tokens[tokens.size() - 1].type != TokenType::OPERATOR &&
                            tokens[tokens.size() - 1].type != TokenType::PAREN_LEFT)
                        tokens.push_back({OPERATOR, "0", "0", 0, '+'});
                    tokens.push_back({NUMBER, "0", "0", std::stod(expr.substr(i - 1, j - i + 1))});
                    if (j < expr.size() && (std::isalpha(expr[j]) || expr[j] == '('))
                        tokens.push_back({OPERATOR, "0", "0", 0, '*'});
                }
                else {
                    while (j < expr.size() && std::isalpha(expr[j]))
                        ++j;
                    if (tokens.size() != 0 && tokens[tokens.size() - 1].type != TokenType::OPERATOR &&
                            tokens[tokens.size() - 1].type != TokenType::PAREN_LEFT)
                        tokens.push_back({OPERATOR, "0", "0", 0, '+'});
                    if (expr[i - 1] == '-') {
                        tokens.push_back({NUMBER, "0", "0", -1});
                        tokens.push_back({OPERATOR, "0", "0", 0, '*', 0});
                    }
                    if (expr[j] == '(')
                        continue;
                    tokens.push_back({PAREN_LEFT});
                    tokens.push_back({VARIABLE, expr.substr(i, j - i)});   
                    tokens.push_back({PAREN_RIGHT});
                }
                i = j;
                continue;
            }
            if (i + 1 < expr.size() && expr[i] == '*' && expr[i + 1] == '*') {
                tokens.push_back({OPERATOR, "0", "0", 0, '&', 0});
                i++;
            }
            else
                tokens.push_back({OPERATOR, "0", "0", 0, expr[i], 0});
            ++i;
        } else {
            return false;
        }
    }
    if (type != type::FUNCTION_EXPR)
        ComplexHandler(tokens);
    return true;
}

std::vector<Token> Computor::toRPN(const std::vector<Token> &tokens) {
    std::vector<Token> output;
    std::stack<Token> ops;
    for (const auto &token : tokens) {
        if (token.type == TokenType::NUMBER ||
            token.type == TokenType::MATRICE ||
            token.type == TokenType::COMPLEXS)
                output.push_back(token);
        else if (token.type == OPERATOR) {
            while (!ops.empty() && ops.top().type == OPERATOR) {
                char op1 = token.op, op2 = ops.top().op;
                if ((isLeftAssoc(op1) && precedence(op1) <= precedence(op2)) ||
                    (!isLeftAssoc(op1) && precedence(op1) < precedence(op2))) {
                    output.push_back(ops.top());
                    ops.pop();
                } else break;
            }
            ops.push(token);
        } else if (token.type == PAREN_LEFT) {
            ops.push(token);
        } else if (token.type == PAREN_RIGHT) {
            while (!ops.empty() && ops.top().type != PAREN_LEFT) {
                output.push_back(ops.top());
                ops.pop();
            }
            if (!ops.empty() && ops.top().type == PAREN_LEFT) ops.pop();
        }
    }
    while (!ops.empty()) {
        output.push_back(ops.top());
        ops.pop();
    }
    return output;
}

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
        if (a.type == ValueType::SCALAR && b.type == ValueType::SCALAR) 
            return Value(a.scalar / b.scalar);
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
            return Value(pow(a.scalar, b.scalar));
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
            if (s.size() <= 1) throw std::runtime_error("invalid expression");
            Value b = s.top(); s.pop();
            Value a = s.top(); s.pop();
            s.push(apply_op(a, b, token.op));
        }
    }
    if (s.size() != 1) throw std::runtime_error("invalid expression");
    return s.top();
}

bool Computor::extractVariables(std::vector<Token> &tokens) {
    std::map<std::string, size_t> varName;
    for (size_t i = 0; i < tokens.size(); i++) {
        if ((tokens[i].type == TokenType::VARIABLE && tokens[i].var != "i") || tokens[i].type == TokenType::FUNCTION)
            varName.insert({tokens[i].var, i});
    }
    if (varName.size() == 0) {
        ComplexHandler(tokens);
        return true;
    }
    for (auto &pair : varName) {
        if (pair.first != "i" && this->map.find(str_toupper(pair.first)) == this->map.end()) {
            throw std::runtime_error("Error : " + pair.first + " is not definited !");
            return false;
        }
        const auto it = this->map.find(str_toupper(pair.first));
        for (size_t i = 0; i < tokens.size(); i++) {
            if (tokens[i].type == TokenType::VARIABLE || tokens[i].type == TokenType::FUNCTION) {
                if (tokens[i].type == TokenType::FUNCTION) {
                    std::vector<Token> token_args;
                    std::vector<Token> resFunctionToken;
                    ParseToToken(tokens[i].functionVar, token_args, RATIONAL_EXPR);
                    for (size_t i = 0; i < it->second.tokens.size(); i++) { // fa(x) = x^2; fb(x) = fa(x) ; fb(1) marche pas
                        if (it->second.tokens[i].type == TokenType::VARIABLE && str_toupper(it->second.tokens[i].var) == it->second.functionVar) {
                            for (size_t j = 0; j < token_args.size(); j++) {
                                resFunctionToken.push_back(token_args[j]);
                            }
                        }
                        else
                            resFunctionToken.push_back(it->second.tokens[i]);
                    }
                    tokens.erase(tokens.begin() + i);
                    tokens.insert(tokens.begin() + i, resFunctionToken.begin(), resFunctionToken.end());
                    return false;
                }
                pair.second = i;
                break;
            }
        }
        tokens.erase(tokens.begin() + pair.second);
        tokens.insert(tokens.begin() + pair.second, it->second.tokens.begin(), it->second.tokens.end());
    }
    return false;
}

bool Computor::functionHandler(std::string &name, std::string &expr, data &data) {
    std::regex name_expr("^([a-zA-Z]+)(\\()([a-zA-Z]+)(\\))$");
    if (std::regex_match(name, name_expr)) {
        data.type = FUNCTION_EXPR;
        auto words_begin = std::sregex_iterator(name.begin(), name.end(), name_expr);
        auto words_end = std::sregex_iterator();
        std::smatch match = *words_begin;
        std::string var = match.str(3);
        bool find = false;
        data.expr = str_toupper(expr);
        for (size_t i = 0; i < expr.size(); i++) {
            if (std::isalpha(expr[i])) {
                size_t j = i;
                while (j < expr.size() && std::isalpha(expr[j]))
                    ++j;
                std::string sub = str_toupper(expr.substr(i, j - i));
                if (sub == var)
                    find = true;
                i = j;
            }
        }
        if (!find) {std::cout << "what's the interest to create a function" << std::endl; return false;}
        data.functionVar = name.substr(name.find('(') + 1, (name.size()) - (name.find('(') + 1) - 1);
        name = name.substr(0, name.find('(') + 1);
        ParseToToken(expr, data.tokens, FUNCTION_EXPR);
        printTable(data.tokens);
        for (auto token : data.tokens)
            if (str_toupper(token.var) == name)
                return false;
        insertInfosInMap(name, data);
        return true;
    }
    return true;
}

std::string TokenToString(std::vector<Token> &tokens) {
    std::string res = "";
    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i].type == TokenType::NUMBER) {
            std::ostringstream oss;
            oss << tokens[i].value;
            res.append(oss.str());
        } else if (tokens[i].type == TokenType::OPERATOR) {
            res.push_back(tokens[i].op);
        } else if (tokens[i].type == TokenType::VARIABLE) {
            res.append(tokens[i].var);
        }
    }
    return res;
}

std::string BetterPrint(std::string str) {
    for (size_t i = 0; i < str.size(); i ++) {
        if ((isOperator(str[i]) || str[i] == '=') && str[i] != '^') {
            str.insert(str.begin() + i, ' ');
            str.insert(str.begin() + i + 2, ' ');
            i += 2;
        }
    }
    return str;
}

void Computor::polynomeHandler(std::string &name, std::vector<Token> &tokens) {
    std::vector<Token> nameTokens;
    std::string varFunction = "";
    if (!ParseToToken(name, nameTokens, RESOLUTION_EXPR)) { std::cout << "invalid expression" << std::endl; return; }
    for (size_t i = 0; i < nameTokens.size(); i++) {
        if (nameTokens[i].type == TokenType::FUNCTION)
            varFunction = nameTokens[i].functionVar;
    }
    try 
    {
        extractVariables(nameTokens);
        std::string literal = "";
        literal = TokenToString(nameTokens);
        literal.push_back('=');
        literal.append(TokenToString(tokens));
        std::cout << BetterPrint(literal) << std::endl;
        literal = ReplaceAll(literal, varFunction, "X");
        Polynome poly(literal);
        poly.printSolutions();
    }
    catch (std::exception &e) { std::cout << e.what() << std::endl; }
}

void Computor::parsingExpr(std::string &text) {
    std::string before = text;
    std::vector<Token> forRPNRead;
    text.erase(remove(text.begin(), text.end(), ' '), text.end());

    unsigned long it = text.find('=');
    if (it == std::string::npos){
        if (this->map.find(str_toupper(text)) == this->map.end())
            std::cout << "invalid expression" << std::endl;
        else {
            auto ite = this->map.find(str_toupper(text));
            std::cout << ite->second.expr << std::endl;
        }
        return;
    }

    std::string name = str_toupper(text.substr(0, it));
    std::string expr = text.substr(it + 1, text.length());
    if (!expr.length() || !CheckParenthesis(text)) { std::cout << "invalid expression" << std::endl; return; }
    data data;

    data.type = RATIONAL_EXPR;
    data.expr = expr;

    if (expr[expr.size() - 1] == '?') 
    {
        if (expr != "?") {
            //std::cout << "invalid expression" << std::endl;
            data.type = type::POLYNOMIAL_EXPR;
            expr.pop_back();
        } else {
            data.type = type::RESOLUTION_EXPR;
            expr = name;
        }
    }
    if (data.type != type::POLYNOMIAL_EXPR)
        if (!functionHandler(name, expr, data)) { std::cout << "invalid expression" << std::endl; return; }
    if (data.type == type::FUNCTION_EXPR) {
        std::cout << BetterPrint(expr) << std::endl;
        return;
    }
    if (!ParseToToken(expr, data.tokens, RATIONAL_EXPR)) { printTable(data.tokens); std::cout << "invalid expression" << std::endl; return; }
    if (!hasVariable(data.tokens))
    {
        if (data.type == type::POLYNOMIAL_EXPR) {
            polynomeHandler(name, data.tokens);
            return;
        }
        forRPNRead = toRPN(data.tokens);
        // printTable(forRPNRead);
        try {
            Value val = evalRPN(forRPNRead);
            printFormat(val);
        } catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
            this->map.extract(str_toupper(name));
            return;
        }
    } else {
        try {
            while (!extractVariables(data.tokens));
            if (data.type == type::POLYNOMIAL_EXPR) {
                polynomeHandler(name, data.tokens);
                return;
            }
            forRPNRead = toRPN(data.tokens);
            Value val = evalRPN(forRPNRead);
            printFormat(val);
        } catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
            this->map.extract(str_toupper(name));
            return;
        }
    }
    if (data.type != type::RESOLUTION_EXPR || data.type != type::POLYNOMIAL_EXPR)
        insertInfosInMap(name, data);
}

void Computor::printMap(void) {
    for (auto it = this->map.begin(); it != this->map.end(); ++it) {
        std::cout << " {" << it->first << ", " << it->second.expr << "} " << std::endl;
    }
}