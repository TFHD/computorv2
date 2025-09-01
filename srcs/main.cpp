#include "Computor.hpp"
#include <cstdlib>

// bool    my_readline(std::string &str, std::string prompt) {

//     str.clear();
//     while (str.empty()) {
//         std::cout << prompt;
//         std::getline(std::cin, str);
//         if (std::cin.eof()) {
//             std::cout << std::endl;
//             return true;
//         }
//     }
//     return false;
// }

int main() {

    Computor computor;
    char *input;
    while ((input = readline("> ")) != nullptr) {
        if (std::string(input) == "clear") {
            system("clear");
            continue;
    }
        if (*input) {
            add_history(input);
        }
        if (std::string(input) == "exit")
            break;
        std::string text = input;
        computor.parsingExpr(text);
        computor.printMap();
        free(input);
    }
    rl_clear_history();
    return 0;
}