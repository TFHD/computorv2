#ifndef COMPUTOR_HPP
# define COMPUTOR_HPP

# include <iostream>
# include <string>
# include <map>
# include <algorithm>
# include <regex>
# include <vector>
# include <readline/readline.h>
# include <readline/history.h>
# include <iomanip>
# include <sstream>
# include "Token.hpp"
# include "Polynome_bonus.hpp"
# include "Utils.hpp"
# include "MathUtils.hpp"

class Computor {

    private :
        std::map<std::string, data> map;
    public :
        Computor(void);
        ~Computor(void);
        void parsingExpr(std::string &text);
        void printMap(void);

        bool writeVaraibleContent(std::string &text, unsigned long &it);
        bool notVariableHandler(data &data, std::string &name);
        bool variableHandler(data &data, std::string &name);
        Value evalRPN(const Tokens &rpn);

        void polynomeHandler(std::string &name, Tokens &tokens);
        void insertInfosInMap(std::string &name, data &data);
        bool functionHandler(std::string &name, std::string &expr, data &data);
        bool extractVariables(Tokens &tokens);
};

#endif