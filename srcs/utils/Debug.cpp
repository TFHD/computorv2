#include "Computor.hpp"

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

template <typename T>
std::string printColor(T value, TokenType tokenType, int i) {

    (void)value;
    if (tokenType == TokenType::NUMBER) { if constexpr (std::is_same<T, double>::value) return "\e[32m"; }
    if (tokenType == TokenType::VARIABLE) { if constexpr (std::is_same<T, std::string>::value) return "\e[32m"; }
    if (tokenType == TokenType::OPERATOR) { if constexpr (std::is_same<T, char>::value) return "\e[32m"; }
    if ((tokenType == TokenType::PAREN_LEFT || tokenType == TokenType::PAREN_RIGHT) && i) { return "\e[36m"; }
    if ((tokenType == TokenType::COMPLEXS || tokenType == TokenType::MATRICE) && i) {return "\e[35m"; }

    return "\e[0m";

}

void printTable(const std::vector<Token>& tokens) {
    const int wValue = 11;
    const int wOp = 10;
    const int wType = 14;
    const int wVar = 18;

    const int totalWidth = wValue + wOp + wType + wVar + 5;
    (void)totalWidth;

    auto printSeparator = [&]() {
        std::cout << "\e[31m+" << std::string(wValue, '-') 
                  << "+" << std::string(wOp, '-') 
                  << "+" << std::string(wType, '-') 
                  << "+" << std::string(wVar, '-') 
                  << "+\e[0m" << std::endl;
    };

    printSeparator();
    std::cout << "\e[31m|\e[33;1m" << std::setw(wValue) << std::left << "   VALUE"
              << "\e[31m|\e[33;1m" << std::setw(wOp) << std::left << "    OP"
              << "\e[31m|\e[33;1m" << std::setw(wType) << std::left << "    TYPE"
              << "\e[31m|\e[33;1m" << std::setw(wVar) << std::left << "       VAR"
              << "\e[31m|\e[33;1m" << std::endl;
    printSeparator();

    for (const auto& t : tokens) {
        std::cout << "\e[31m|" << printColor(t.value, t.type, 0) << std::setw(wValue) << std::left << t.value
                  << "\e[31m|" << printColor(t.op, t.type, 0) << std::setw(wOp) << std::left << t.op
                  << "\e[31m|" << printColor(t.mat, t.type, 1) << std::setw(wType) << std::left << typeToString(t.type)
                  << "\e[31m|" << printColor(t.var, t.type, 0) << std::setw(wVar) << std::left << t.var
                  << "\e[31m|" << std::endl;
    }
    printSeparator();
}