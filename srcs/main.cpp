#include "Computor.hpp"
#include <cstdlib>

int main() {

    Computor computor;
    char *input;
    using_history();
    while ((input = readline("> ")) != nullptr) {
        if (std::string(input) == "clear") {
            system("clear");
            free(input);
            continue;
        }
        if (std::string(input) == "printmap") {
            computor.printMap();
            free(input);
            continue;
        }
        if (*input)
            add_history(input);
        if (std::string(input) == "exit") {
            free(input);
            break;
        }
        std::string text = input;
        computor.parsingExpr(text);
        free(input);
    }
    rl_clear_history();
    return 0;
}