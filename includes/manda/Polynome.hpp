#ifndef POLYNOME_HPP
# define POLYNOME_HPP

# include <string>
# include <iostream>
# include <cstdlib>
# include <algorithm>
# include <map>
# include <sstream>
# include <regex>

class Polynome {

    private:
        int polynome_degree;
        std::map<std::string, double> numbers;
        std::map<std::string, double> numbers_after;

        void searchPositions(std::string expr, std::map<std::string, double> &map);
        void convertCharToPolynome(std::string &expr);
        void reducePolynome(std::string &expr);
        int pgcd(int a, int b);
        float fabs(float x);
        double my_sqrt(double x);
        int abs(int x);
        double pow(double x, int e);
        void printValuePositive(double value, std::string str, std::string before, bool space);
        void setDegrees(std::string str);
        int parsingExpr(std::string expr);
        int regexExpr(std::string expr);
        void adaptExpr(std::string &expr);

    public:
        Polynome(std::string expr);
        ~Polynome(void);

        double getA(void);
        double getB(void);
        double getC(void);
        int getPolynomeDegree(void);
        void printReducedForme(void);
        void printSolutions(void);
};

#endif