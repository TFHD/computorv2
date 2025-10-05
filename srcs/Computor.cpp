#include "Computor.hpp"

Computor::Computor(void) {}
Computor::~Computor(void) {}


void Computor::insertInfosInMap(std::string &name, data &data) {
    if (this->map.find(name) == this->map.end()) {
        this->map.insert({name, data});
    }
    else {
        auto it = this->map.find(name);
        it->second = data;
    }
}

bool Computor::extractVariables(Tokens &tokens) {
    bool replaced = false;
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i].type == TokenType::VARIABLE && tokens[i].var != "i") {
            std::string varUp = str_toupper(tokens[i].var);
            auto it = this->map.find(varUp);
            if (it != this->map.end()) {
                // On remplace la variable par ses tokens définis dans la map
                for (auto token : it->second.tokens)
                    if (str_toupper(token.var) == varUp) {
                        return true;
                    }
                tokens.erase(tokens.begin() + i);
                tokens.insert(tokens.begin() + i, it->second.tokens.begin(), it->second.tokens.end());
                replaced = true;
                i = -1;
            }
        } else if (tokens[i].type == TokenType::FUNCTION) {
            std::string funcNameUp = str_toupper(tokens[i].var);
            auto it = this->map.find(funcNameUp);
            if (it != this->map.end()) {
                Tokens token_args;
                Token::parseToToken(tokens[i].functionVar, token_args, RATIONAL_EXPR);

                Tokens resFunctionToken;
                for (auto& funcToken : it->second.tokens) {
                    if (funcToken.type == TokenType::VARIABLE &&
                        str_toupper(funcToken.var) == it->second.functionVar) {
                        resFunctionToken.insert(resFunctionToken.end(), token_args.begin(), token_args.end());
                    } else {
                        if (funcToken.type == TokenType::FUNCTION)
                            funcToken.functionVar = tokens[i].functionVar;
                        resFunctionToken.push_back(funcToken);
                    }
                }
                tokens.erase(tokens.begin() + i);
                tokens.insert(tokens.begin() + i, resFunctionToken.begin(), resFunctionToken.end());
                replaced = true;
                i = -1;
            }
        }
    }
    if (!replaced) {
        Token::complexHandler(tokens);
        return true;
    }
    return false;
}

bool Computor::functionHandler(std::string &name, std::string &expr, data &data) {
    std::regex name_expr("^([a-zA-Z]+)(\\()([a-zA-Z]+)(\\))$");
    if (std::regex_match(name, name_expr)) {
        data.type = FUNCTION_EXPR;
        std::cout << "Function found: " << name << std::endl;
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
        try {
            Token::parseToToken(expr, data.tokens, FUNCTION_EXPR);
            extractVariables(data.tokens);
            printTable(data.tokens);
            Value val = evalRPN(Token::toRPN(data.tokens));
            data.expr = printFormat(val);
            std::cout << data.expr << std::endl;
        }
        catch (std::exception &e) { std::cout << "invalid expression : " << e.what() << std::endl; return false; }

        for (auto token : data.tokens)
            if (str_toupper(token.var) == name) {
                std::cout << "invalid expression" << std::endl;
                return false;
            }
        insertInfosInMap(name, data);
        return true;
    }
    return true;
}

void Computor::polynomeHandler(std::string &name, Tokens &tokens) {
    Tokens nameTokens;
    std::string varFunction = "";
    try {
        if (!Token::parseToToken(name, nameTokens, RESOLUTION_EXPR)) { std::cout << "invalid expression" << std::endl; return; }
        for (size_t i = 0; i < nameTokens.size(); i++) {
            if (nameTokens[i].type == TokenType::FUNCTION)
                varFunction = nameTokens[i].functionVar;
        }
        extractVariables(nameTokens);
        printTable(nameTokens);
        std::string literal = "";
        literal = Token::tokenToString(nameTokens);
        literal.push_back('=');
        literal.append(Token::tokenToString(tokens));
        std::cout << betterPrint(literal) << std::endl;
        literal = ReplaceAll(literal, varFunction, "X");
        Polynome poly(literal);
        poly.printSolutions();
    }
    catch (std::exception &e) { std::cout << e.what() << std::endl; }
}

bool isWithVariable(TokenType type, char op) {
    if (type == TokenType::OPERATOR && (op == '+' || op == '-'))
        return false;
    return true;
}

bool ReducePrintForm(Tokens &tokens) {
    Tokens res;
    int start = 0;
    int end = 0;
    int index = -1;

    for (size_t i = 0; i < tokens.size(); i++)
        if (tokens[i].type == TokenType::VARIABLE) { index = i; break; }
    
    if (index == -1)
        return false;

    for (int i = index - 1; i >= 0; i--) {
        if (isWithVariable(tokens[i].type, tokens[i].op) || i == index - 1) { start = i;}
        else break;
    }

    for (int i = index + 1; i < (int)tokens.size(); i++) {
        if (isWithVariable(tokens[i].type, tokens[i].op) || i == index + 1) { end = i;}
        else break;
    }
    std::cout << start << std::endl;
    std::cout << end << std::endl;
    return true;
}

bool Computor::writeVaraibleContent(std::string &text, unsigned long &it) {

    if (it == std::string::npos){
        if (this->map.find(str_toupper(text)) == this->map.end())
            std::cout << "invalid expression" << std::endl;
        else {
            auto ite = this->map.find(str_toupper(text));
            std::cout << ite->second.expr << std::endl;
        }
        return true;
    }
    return false;
}

void GetTypeExpression(data &data, std::string &expr, std::string &name) {
    
    data.type = RATIONAL_EXPR;
    data.expr = expr;

    if (expr[expr.size() - 1] == '?') 
    {
        if (expr != "?") {
            data.type = ExpressionType::POLYNOMIAL_EXPR;
            expr.pop_back();
        } else {
            data.type = ExpressionType::RESOLUTION_EXPR;
            expr = name;
        }
    }
    name = str_toupper(name);
}

bool Computor::notVariableHandler(data &data, std::string &name) {
    Tokens forRPNRead;

    if (!isAlphaString(name) && data.type == ExpressionType::RATIONAL_EXPR)
        { std::cout << "a variable can contain only letter" << std::endl; return false; }
    if (data.type == ExpressionType::POLYNOMIAL_EXPR) { polynomeHandler(name, data.tokens); return false; }
    
    forRPNRead = Token::toRPN(data.tokens);

    try {
        Value val = evalRPN(forRPNRead);
        data.expr = printFormat(val);
        std::cout << data.expr << std::endl;
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        this->map.extract(str_toupper(name));
        return false;
    }
    return true;
}

bool Computor::variableHandler(data &data, std::string &name) {
    Tokens forRPNRead;
    try {
        Tokens TokenDataCopy = data.tokens;
        if (!isAlphaString(name) && data.type == ExpressionType::RATIONAL_EXPR)
            { std::cout << "a variable can contain only letter" << std::endl; return false; }
        while (!extractVariables(TokenDataCopy));
        if (data.type == ExpressionType::POLYNOMIAL_EXPR) {
            polynomeHandler(name, TokenDataCopy);
            return false;
        }
        forRPNRead = Token::toRPN(TokenDataCopy);
        Value val = evalRPN(forRPNRead);
        data.expr = printFormat(val);
        std::cout << data.expr << std::endl;
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        this->map.extract(str_toupper(name));
        return false;
    }
    return true;
}

void Computor::parsingExpr(std::string &text) {
    std::string before = text;
    Tokens forRPNRead;
    text = NoSpace(text);

    unsigned long it = text.find('=');
    if(writeVaraibleContent(text, it)) return;

    std::string name = text.substr(0, it);
    std::string expr = text.substr(it + 1, text.length());
    if (!expr.length() || !CheckParenthesis(text)) { std::cout << "invalid expression" << std::endl; return; }
    
    data data;
    GetTypeExpression(data, expr, name);

    if (data.type != ExpressionType::POLYNOMIAL_EXPR && !functionHandler(name, expr, data)) { return; }
    if (data.type == ExpressionType::FUNCTION_EXPR) { return; }
    if (!Token::parseToToken(expr, data.tokens, RATIONAL_EXPR)) { std::cout << "invalid expression" << std::endl; return; }
    
    printTable(data.tokens);
    if (!hasVariable(data.tokens)) { if (!notVariableHandler(data, name)) return ; }
    else { if (!variableHandler(data, name)) return; }

    if (data.type != ExpressionType::RESOLUTION_EXPR && data.type != ExpressionType::POLYNOMIAL_EXPR)
        insertInfosInMap(name, data);

}
void Computor::printMap(void) {
    if (this->map.empty()) {
        std::cout << "\033[1;90m[Map vide]\033[0m" << std::endl;
        return;
    }
    size_t maxName = 3, maxExpr = 10;
    for (const auto& [name, data] : this->map) {
        maxName = std::max(maxName, name.size());
        maxExpr = std::max(maxExpr, data.expr.size());
    }
    std::string top = "+-" + std::string(maxName, '-') + "-+-" + std::string(maxExpr, '-') + "-+";
    std::string sep = top;
    std::cout << "\n\033[1;34m" << top << "\033[0m\n";
    std::cout << "\033[1;44m| "
              << std::left << std::setw(maxName) << "Nom" << " | "
              << std::left << std::setw(maxExpr) << "Expression" << " |\033[0m\n";
    std::cout << "\033[1;34m" << sep << "\033[0m\n";
    for (const auto& [name, data] : this->map) {
        std::cout << "| "
                  << "\033[1;36m" << std::left << std::setw(maxName) << name << "\033[0m" << " | "
                  << "\033[1;32m" << std::left << std::setw(maxExpr) << data.expr << "\033[0m" << " |"
                  << std::endl;
    }
    std::cout << "\033[1;34m" << top << "\033[0m\n" << std::endl;
}