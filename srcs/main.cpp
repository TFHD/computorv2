#include "../includes/Computor.hpp"

bool    my_readline(std::string &str, std::string prompt) {

    str.clear();
    while (str.empty()) {
        std::cout << prompt;
        std::getline(std::cin, str);
        if (std::cin.eof()) {
            std::cout << std::endl;
            return true;
        }
    }
    return false;
}

int main() {

    Computor computor;
    bool run = true;

    while (run) {
        std::string text;
        if (my_readline(text, "> "))
            return 1;
        computor.parsingExpr(text);
        computor.printMap();
    }

    return 0;
}