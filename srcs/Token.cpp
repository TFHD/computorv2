#include "Computor.hpp"

#include "Computor.hpp"

int Token::matriceHandler(std::string expr, Tokens &tokens) { 
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

bool Token::complexHandler(Tokens &tokens) {
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
        tokens.insert(tokens.begin() + numberIndex, {{PAREN_LEFT},{COMPLEXS, std::to_string(cplx.getIm()).append("i"), "0", 0, '0', cplx}});
    } else if (numberIndex >= 0 && tokens[operatorIndex].op == '+') {
        tokens.erase(tokens.begin() + complexIndex);
        tokens.insert(tokens.begin() + complexIndex, {COMPLEXS, std::to_string(cplx.getIm()).append("i"), "0", 0, '0', cplx});

    } else if (numberIndex == complexIndex || operatorIndex == complexIndex) {
        tokens.erase(tokens.begin() + complexIndex);
        tokens.insert(tokens.begin() + complexIndex, {COMPLEXS, std::to_string(cplx.getIm()).append("i"), "0", 0, '0', cplx});
    } else {
        return false;
    }
    complexHandler(tokens);
    return true;
}

std::string Token::tokenToString(Tokens &tokens) {
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

bool Token::parseToToken(std::string &expr, Tokens &tokens, ExpressionType type) {
    tokens.clear();
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
        else if (std::isdigit(expr[i]) || (expr[i] == '.' && i + 1 < expr.size() && std::isdigit(expr[i + 1]))) {
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
            if (i + 1 < expr.size() && (std::isdigit(expr[i + 1]) || std::isalpha(expr[i + 1]) || expr[i + 1] == '(') && (expr[i] == '-' || expr[i] == '+')) {
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
    if (type != ExpressionType::FUNCTION_EXPR)
        complexHandler(tokens);
    return true;
}

Tokens Token::toRPN(const Tokens &tokens) {
    Tokens output;
    std::stack<Token> ops;
    for (const auto &token : tokens) {
        if (token.type == TokenType::NUMBER ||
            token.type == TokenType::MATRICE ||
            token.type == TokenType::COMPLEXS ||
            token.type == TokenType::VARIABLE)
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