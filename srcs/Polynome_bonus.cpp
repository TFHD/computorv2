#include "Polynome_bonus.hpp"

Polynome::Polynome(std::string expr) : polynome_degree(0)
{
    if (parsingExpr(expr))
        throw std::runtime_error("Error : invalid value(s) !");
    this->numbers.insert({"*X^0", 0.0});
    this->numbers.insert({"*X^1", 0.0});
    this->numbers.insert({"*X^2", 0.0});
    unsigned long it = expr.find('=');
    if (it == std::string::npos)
        throw std::runtime_error("Error : invalid value(s) !");
    std::string before = expr.substr(0, it);
    convertCharToPolynome(before);
    if (it != std::string::npos) {
        std::string after = expr.substr(it + 1, expr.length());
        reducePolynome(after);
        setDegrees(after);
    }
    setDegrees(before);
}
Polynome::~Polynome(void) {}

void Polynome::adaptExpr(std::string &expr)
{
    for (int i = 0; i < (int)expr.length(); i++) {
        unsigned long pos = expr.find("X", i);
        if (pos == std::string::npos)
            continue;
        if (pos + 1 >= expr.length() || expr[pos + 1] != '^') {
            expr.insert(pos + 1, 1, '^');
            expr.insert(pos + 2, 1, '1');
        }
        pos = expr.find("X", i);
        if (expr[pos - 1] != '*') {
            expr.insert(pos, 1, '*');
            if (!std::isdigit(expr[pos - 1]))
                expr.insert(pos, 1, '1');
        }
    }

    for (int i = 0; i < (int)expr.length(); i++) {
        if (std::isdigit(expr[i])) {
            if (i - 1 >= 0 && expr[i - 1] != '+' && !std::isdigit(expr[i - 1]))
                continue;
            if (i + 1 < (int)expr.length() && (expr[i + 1] == '*' || std::isdigit(expr[i + 1])))
                continue;
            expr.insert(i + 1, "*X^0", 0, 4);
        }
    }
}

int Polynome::regexExpr(std::string expr)
{
    std::regex long_word_regex("^([-+][0-9.]+\\*X\\^[0-9.]+)+$");
        std::string new_s = std::regex_replace(expr, long_word_regex, "[$&]");
        if (new_s == expr)
            return 1;
    return 0;
}

void Polynome::convertCharToPolynome(std::string &expr)
{
    expr.erase(remove(expr.begin(), expr.end(), ' '), expr.end());
    if (strtol(expr.c_str(), 0, 0) >= 0 && expr[0] != '+')
        expr.insert(0, 1, '+');
    adaptExpr(expr);
    if (regexExpr(expr))
        throw std::runtime_error("Error : invalid syntax !");
    searchPositions(expr, this->numbers);
}

int Polynome::parsingExpr(std::string expr)
{
    for (unsigned int i = 0; i < expr.length(); i++)
        if (expr[i] != 'X' && expr[i] != '^' && expr[i] != '+' && expr[i] != '-' && expr[i] != '*' && expr[i] != '=' && expr[i] != ' ' && expr[i] != '.' && !std::isdigit(expr[i]))
            return 1;
    return 0;
}

void Polynome::reducePolynome(std::string &expr)
{
    expr.erase(remove(expr.begin(), expr.end(), ' '), expr.end());
    if (strtol(expr.c_str(), 0, 0) >= 0 && expr[0] != '+')
        expr.insert(0, 1, '+');
    adaptExpr(expr);
    if (regexExpr(expr))
        throw std::runtime_error("Error : invalid syntax !");
    searchPositions(expr, this->numbers_after);
    for (auto &pair : this->numbers_after) {
        auto it = this->numbers.find(pair.first);
        if (it != numbers.end())
            it->second -= pair.second;
        else
            this->numbers.insert({pair.first, -pair.second});
    }
}

int Polynome::pgcd(int a, int b) {
    if (b == 0)
        return a;
    return pgcd(b, a % b);
}

int Polynome::abs(int x) {
    return (x < 0) ? -x : x; 
}

float Polynome::fabs(float x) {
    return (x < 0) ? -x : x; 
}

double Polynome::pow(double x, int e) {

    int res = 1;
    while (e-- > 0) {
        res *= x;
    }
    return res;
}

double Polynome::my_sqrt(double x) {
    if (x < 0)
        return -1;
    if (x == 0)
        return 0;
    double guess = x;
    double epsilon = 1e-10;
    int i = 10000;
    while (std::abs(guess * guess - x) > epsilon && i > 0) {
        guess = (guess + x / guess) / 2.0;
        i--;
    }
    return guess;
}

void Polynome::printSolutions(void)
{  
    if (getPolynomeDegree() > 0)
    if (getPolynomeDegree() > 2) {
        std::cout << "The polynomial degree is strictly greater than 2, I can't solve." << std::endl;
        return;
    }
    if (!getA() && !getB() && !getC()) {
        std::cout << "Any real number is a solution." << std::endl;
        return ;
    }
    if (this->polynome_degree <= 1) {
        if (!getB()) {
            std::cout << "No solution." << std::endl;
            return;
        }
        std::cout << "The solution is:" << std::endl;
        double x1 = -getC() / getB();
        std::cout << x1 << std::endl;
        return;
    }
    double determinant = getB() * getB() - 4 * getA() * getC();
    if (determinant > 0) {
        if (!getA()) {
            std::cout << "No solution." << std::endl;
            return;
        }
        std::cout << "Discriminant is strictly positive, the two solutions are:" << std::endl;
        double x1 = (-getB() - my_sqrt(determinant)) / (2 * getA());
        double x2 = (-getB() + my_sqrt(determinant)) / (2 * getA());
        std::cout << x1 << std::endl << x2 << std::endl;
    }
    else if (!determinant) {
        std::cout << "The solution is:" << std::endl;
        double x1 = -getB() / (2 * getA());
        std::cout << x1 << std::endl;
    }
    else {
        std::cout << "Discriminant is strictly negative, the two complex solutions are:" << std::endl;
        int pgcds = 1;
        int pgcd1 = fabs(pgcd(getB(), 2 * getA()));
        int pgcd2 = fabs(pgcd(my_sqrt(-determinant), 2 * getA()));
        std::cout << pgcd1 << " " << pgcd2 << std::endl;
        int sqrt_determinant = my_sqrt(-determinant);
        if (pgcd1 == pgcd2 && (int)pow(my_sqrt(getB()), 2) == (int)getB() && (int)pow(sqrt_determinant, 2) == (int)-determinant)
            pgcds = pgcd1;
        if ((int)pow(sqrt_determinant, 2) == (int)-determinant) {
            if (-getB())
                std::cout << -getB() / pgcds << "/" << 2 * getA() / pgcds;
            else
                std::cout << -getB();
            std::cout << " + i" << my_sqrt(-determinant) / pgcds << "/" << 2 * getA() / pgcds << std::endl;
            if (-getB())
                std::cout << -getB() / pgcds << "/" << 2 * getA() / pgcds;
            else
                std::cout << -getB();
            std::cout << " - i" << my_sqrt(-determinant) / pgcds << "/" << 2 * getA() / pgcds << std::endl;
        }
        else {
            int pgcd3 = 1;
            if (pgcd1 == pgcd2) {
                pgcds = pgcd1;
                for (int i = 1; i < pgcds; i++) {
                    if (pgcds % i == 0 && (int)-determinant % (int)pow(i, 2) == 0)
                        pgcd3 = i;
                }
                determinant /= pow(pgcd3, 2);
            }
            if (-getB())
                std::cout << -getB() << "/" << 2 * getA() / pgcd3;
            else
                std::cout << -getB();
            std::cout << " + i√(" << -determinant << ")/" << 2 * getA() / pgcd3 << std::endl;
            if (-getB())
                std::cout << -getB() << "/" << 2 * getA() / pgcd3;
            else
                std::cout << -getB();
            std::cout << " - i√(" << -determinant << ")/" << 2 * getA() / pgcd3 << std::endl; 
        }
    }
}

void Polynome::setDegrees(std::string str)
{
    unsigned int i = str.find("X^");
    while (i < str.length()) {
        int nb = strtol(str.c_str() + i + 2, 0, 0);
        if (this->polynome_degree < nb && this->numbers.find("*X^" + std::to_string(nb))->second)
            this->polynome_degree = nb;
        str = str.substr(i + 2, str.length());
        i = str.find("X^");
    }
}

void Polynome::searchPositions(std::string expr, std::map<std::string, double> &map)
{
    unsigned int i = expr.find("*X^");
    while (i < expr.length()) {
        if (i + 3 >= expr.length())
            return ;
        int exp = strtol(expr.c_str() + i + 3, 0, 0);
        std::string result = "*X^" + std::to_string(exp);
        unsigned long pos = expr.find(result);
        if (pos == std::string::npos) {
            expr = expr.substr(i + result.length(), expr.length());
            i = expr.find("*X^");
            continue;
        }
        while (--pos > 0 && (std::isdigit(expr[pos]) || expr[pos] == '.'));
        auto it = map.find(result);
        if (it != map.end())
            it->second += std::strtod(expr.c_str() + pos, 0);
        else
            map.insert({result, std::strtod(expr.c_str() + pos, 0)});
        expr = expr.substr(i + result.length(), expr.length());
        i = expr.find("*X^");
    }
}

void Polynome::printValuePositive(double value, std::string str, std::string before, bool space)
{
    if (str == " * X^0" || str == " * X^1") {
        if (str == " * X^0")
            str = "";
        else
            str = " * X";
    }
    if (value >= 0)
        std::cout << before << value << str;
    else if (value < 0 && space)
        std::cout << " - " << -value << str;
    else
        std::cout << "-" << -value << str;
}

void Polynome::printReducedForme(void)
{
    std::cout << "Reduced form: ";

    for (int i = 0; i < __INT_MAX__; i++) {
        std::string result = "*X^" + std::to_string(i); 
        auto it = this->numbers.find(result);
        if (it != this->numbers.end()) {
            result.insert(0, 1, ' ');
            result.insert(2, 1, ' ');
            if (result == " * X^0" && i <= getPolynomeDegree())
                printValuePositive(it->second, result, "", false);
            else if (result != " * X^0" && i <= getPolynomeDegree())
                printValuePositive(it->second, result, " + ", true);
        }
        else
            break;
    }
    std::cout << " = 0" << std::endl;
}

double Polynome::getA(void) {return this->numbers.find("*X^2")->second; };
double Polynome::getB(void) {return this->numbers.find("*X^1")->second; };
double Polynome::getC(void) {return this->numbers.find("*X^0")->second; };
int Polynome::getPolynomeDegree(void) { return this->polynome_degree; };