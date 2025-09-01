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
        // On ne traite pas "i" ni les variables déjà résolues
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
                // On repart du début car le vecteur a changé
                i = -1;
            }
            // sinon on laisse la variable telle quelle
        } else if (tokens[i].type == TokenType::FUNCTION) {
            // On tente de remplacer la fonction si elle est définie
            std::string funcNameUp = str_toupper(tokens[i].var);
            auto it = this->map.find(funcNameUp);
            if (it != this->map.end()) {
                printTable(it->second.tokens);
                // On parse les arguments de la fonction appelante
                Tokens token_args;
                Token::ParseToToken(tokens[i].functionVar, token_args, RATIONAL_EXPR);

                // Remplacement dans la fonction appelée
                Tokens resFunctionToken;
                for (auto& funcToken : it->second.tokens) {
                    if (funcToken.type == TokenType::VARIABLE &&
                        str_toupper(funcToken.var) == it->second.functionVar) {
                        // Remplacer paramètre formel par l'argument effectif
                        resFunctionToken.insert(resFunctionToken.end(), token_args.begin(), token_args.end());
                        printTable(resFunctionToken);
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
            // sinon on laisse la fonction telle quelle
        }
    }
    // S'il n'y avait aucune variable/fonction à remplacer, tenter le handler Complexe
    if (!replaced) {
        Token::ComplexHandler(tokens);
        return true;
    }
    // Indique qu'il reste potentiellement des variables à résoudre
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
        Token::ParseToToken(expr, data.tokens, FUNCTION_EXPR);
        printTable(data.tokens);
        for (auto token : data.tokens)
            if (str_toupper(token.var) == name)
                return false;
        insertInfosInMap(name, data);
        return true;
    }
    return true;
}

void Computor::polynomeHandler(std::string &name, Tokens &tokens) {
    Tokens nameTokens;
    std::string varFunction = "";
    if (!Token::ParseToToken(name, nameTokens, RESOLUTION_EXPR)) { std::cout << "invalid expression Token polynom" << std::endl; return; }
    for (size_t i = 0; i < nameTokens.size(); i++) {
        if (nameTokens[i].type == TokenType::FUNCTION)
            varFunction = nameTokens[i].functionVar;
    }
    try 
    {
        extractVariables(nameTokens);
        std::string literal = "";
        literal = Token::TokenToString(nameTokens);
        literal.push_back('=');
        literal.append(Token::TokenToString(tokens));
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

void Computor::parsingExpr(std::string &text) {
    std::string before = text;
    Tokens forRPNRead;
    text.erase(remove(text.begin(), text.end(), ' '), text.end());

    unsigned long it = text.find('=');
    if (it == std::string::npos){
        if (this->map.find(str_toupper(text)) == this->map.end())
            std::cout << "invalid expression variable not found" << std::endl;
        else {
            auto ite = this->map.find(str_toupper(text));
            std::cout << ite->second.expr << std::endl;
        }
        return;
    }

    std::string name = text.substr(0, it);
    std::string expr = text.substr(it + 1, text.length());
    if (!expr.length() || !CheckParenthesis(text)) { std::cout << "invalid expression parenthesis" << std::endl; return; }
    data data;

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
    if (data.type != ExpressionType::POLYNOMIAL_EXPR)
        if (!functionHandler(name, expr, data)) { std::cout << "invalid expression function handler" << std::endl; return; }
    if (data.type == ExpressionType::FUNCTION_EXPR) {
        std::cout << betterPrint(expr) << std::endl;
        return;
    }
    if (!Token::ParseToToken(expr, data.tokens, RATIONAL_EXPR)) { printTable(data.tokens); std::cout << "invalid expression tokenizer" << std::endl; return; }
    if (!hasVariable(data.tokens))
    {
        printTable(data.tokens);
        if (data.type == ExpressionType::POLYNOMIAL_EXPR) {
            polynomeHandler(name, data.tokens);
            return;
        }
        forRPNRead = Token::toRPN(data.tokens);
        printTable(forRPNRead);
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
            Tokens TokenDataCopy = data.tokens;
            printTable(TokenDataCopy);
            if (!isAlphaString(name) && data.type == ExpressionType::RATIONAL_EXPR)
                { std::cout << "a variable can contain only letter" << std::endl; return; }
            while (!extractVariables(TokenDataCopy));
            if (data.type == ExpressionType::POLYNOMIAL_EXPR) {
                polynomeHandler(name, TokenDataCopy);
                return;
            }
            printTable(TokenDataCopy);
            //ReducePrintForm(TokenDataCopy);
            forRPNRead = Token::toRPN(TokenDataCopy);
            printTable(forRPNRead);
            Value val = evalRPN(forRPNRead);
            printFormat(val);
        } catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
            this->map.extract(str_toupper(name));
            return;
        }
    }
    if (data.type != ExpressionType::RESOLUTION_EXPR && data.type != ExpressionType::POLYNOMIAL_EXPR)
        insertInfosInMap(name, data);

}
void Computor::printMap(void) {
    for (auto it = this->map.begin(); it != this->map.end(); ++it) {
        std::cout << " {" << it->first << ", " << it->second.expr << "} " << std::endl;
    }
}