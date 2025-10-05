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
    for(size_t i = 0; i < str.size(); i++)
        new_str.push_back(std::toupper(str[i]));
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

std::string printFormat(Value &val) {
    std::ostringstream oss;
    if (val.type == SYMBOLIC) {
        bool first = true;
        for (auto& [var, coeff] : val.symbolic.coeffs) {
            if (coeff == 0) continue;
            // Handle sign and separator
            if (!first) oss << (coeff > 0 ? " + " : " - ");
            else if (coeff < 0) oss << "-";
            first = false;
            // Print coefficient (skip if 1 or -1)
            double abs_coeff = std::abs(coeff);
            if (abs_coeff != 1) {
                // If int, no decimals; else, 1 decimal max
                if (std::floor(abs_coeff) == abs_coeff)
                    oss << static_cast<int>(abs_coeff);
                else
                    oss << std::fixed << std::setprecision(2) << abs_coeff;
                oss << " * ";  // Ajouter le * entre le coeff et la variable
            }
            oss << var;
        }
        // Affichage de la constante
        if (val.symbolic.constant != 0 || first) {
            if (!first) oss << (val.symbolic.constant > 0 ? " + " : " - ");
            else if (val.symbolic.constant < 0) oss << "-";
            if (std::floor(std::abs(val.symbolic.constant)) == std::abs(val.symbolic.constant))
                oss << static_cast<int>(std::abs(val.symbolic.constant));
            else
                oss << std::fixed << std::setprecision(2) << std::abs(val.symbolic.constant);
        }
        return oss.str();
    }
    if (val.type == ValueType::MATRIX)
        val.matrix.printMatrice();
    else if (val.type == ValueType::SCALAR) { oss << val.scalar; return oss.str(); }
    else if (val.type == ValueType::COMPLEX) { oss << val.cplx; return oss.str(); }

    return "";
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

std::string &NoSpace(std::string str) {
    std::string &res = str;
    res.erase(remove(res.begin(), res.end(), ' '), res.end());
    return res;
}
