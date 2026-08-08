#include <Computor_bonus.hpp>

static Complex parseMatrixCell(const std::string &raw) {
    std::string s;
    for (char c : raw)
        if (c != ' ')
            s.push_back(c);

    if (s.empty())
        throw std::runtime_error("Empty matrix cell");

    if (s.back() == 'i') {
        s.pop_back();
        if (!s.empty() && s.back() == '*')
            s.pop_back();

        int sep = -1;
        for (int i = (int)s.size() - 1; i >= 1; --i) {
            if (s[i] == '+' || s[i] == '-') {
                sep = i;
                break;
            }
        }
        if (sep >= 0) {
            double re = std::strtod(s.substr(0, sep).c_str(), 0);
            std::string imPart = s.substr(sep);
            double im = (imPart == "+" || imPart == "-")
                ? (imPart == "+" ? 1.0 : -1.0)
                : std::strtod(imPart.c_str(), 0);
            return Complex(re, im);
        }
        if (s.empty() || s == "+")
            return Complex(0, 1);
        if (s == "-")
            return Complex(0, -1);
        return Complex(0, std::strtod(s.c_str(), 0));
    }
    return Complex(std::strtod(s.c_str(), 0), 0);
}

int Token::matriceHandler(std::string expr, Tokens &tokens) {
    if (expr.size() < 4 || expr[0] != '[' || expr[1] != '[')
        return 0;

    int depth = 0;
    size_t end = 0;
    for (; end < expr.size(); ++end) {
        if (expr[end] == '[')
            ++depth;
        else if (expr[end] == ']') {
            --depth;
            if (depth == 0) {
                ++end;
                break;
            }
        }
    }
    if (depth != 0)
        return 0;

    std::string match_str = expr.substr(0, end);
    std::string inner = match_str.substr(1, match_str.size() - 2);

    Token tokenMatrice = {MATRICE, match_str};
    size_t cols = 0;
    int k = 0;

    try {
        for (size_t i = 0; i < inner.size();) {
            if (inner[i] == '[') {
                size_t j = i + 1;
                while (j < inner.size() && inner[j] != ']')
                    ++j;
                if (j >= inner.size())
                    return 0;

                std::string row = inner.substr(i + 1, j - i - 1);
                std::vector<std::string> cells;
                std::string cur;
                for (char c : row) {
                    if (c == ',') {
                        cells.push_back(cur);
                        cur.clear();
                    } else
                        cur.push_back(c);
                }
                cells.push_back(cur);

                if (k == 0)
                    cols = cells.size();
                else if (cells.size() != cols) {
                    std::cout << "Matrice size is not correct" << std::endl;
                    return 0;
                }

                tokenMatrice.mat.getMat().resize(k + 1);
                for (const std::string &cell : cells)
                    tokenMatrice.mat.getMat()[k].push_back(parseMatrixCell(cell));

                ++k;
                i = j + 1;
                if (i < inner.size() && inner[i] == ';')
                    ++i;
            } else if (inner[i] == ';') ++i;
            else if (inner[i] == ' ') ++i;
            else return 0;
        }
    } catch (const std::exception &) {
        return 0;
    }

    if (k == 0)
        return 0;

    tokens.push_back(tokenMatrice);
    return static_cast<int>(match_str.size());
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

    int operatorIndex = -1;
    int numberIndex = -1;

    for (int i = complexIndex - 1; i >= 0; --i) {
        if (tokens[i].type == TokenType::PAREN_LEFT || tokens[i].type == TokenType::PAREN_RIGHT)
            continue;
        if (tokens[i].type == TokenType::OPERATOR) {
            operatorIndex = i;
            break;
        }
        break;
    }
    if (operatorIndex >= 0 && tokens[operatorIndex].op == '*') {
        for (int i = operatorIndex - 1; i >= 0; --i) {
            if (tokens[i].type == TokenType::PAREN_LEFT || tokens[i].type == TokenType::PAREN_RIGHT)
                continue;
            if (tokens[i].type == TokenType::NUMBER) {
                numberIndex = i;
                break;
            }
            break;
        }
    }

    bool powered = false;
    for (size_t i = complexIndex + 1; i < tokens.size(); ++i) {
        if (tokens[i].type == TokenType::PAREN_RIGHT)
            continue;
        powered = (tokens[i].type == TokenType::OPERATOR && tokens[i].op == '^');
        break;
    }

    if (!powered && numberIndex >= 0 && operatorIndex >= 0 && tokens[operatorIndex].op == '*') {
        double im = tokens[numberIndex].value;
        cplx.setIm(im);
        tokens.erase(tokens.begin() + numberIndex, tokens.begin() + complexIndex + 1);
        tokens.insert(tokens.begin() + numberIndex, {{PAREN_LEFT},{COMPLEXS, std::to_string(cplx.getIm()).append("i"), "0", 0, '0', cplx}});
    } else {
        tokens.erase(tokens.begin() + complexIndex);
        tokens.insert(tokens.begin() + complexIndex, {COMPLEXS, std::to_string(cplx.getIm()).append("i"), "0", 0, '0', cplx});
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