#ifndef MATRICE_HPP
# define MATRICE_HPP

# include "Complex.hpp"
# include <vector>
# include <stdexcept>
# include <iostream>
# include <iomanip>
# include <sstream>
# include <string>
# include <algorithm>

class Matrice {
    private:
        std::vector<std::vector<Complex>> mat;
    public:
        Matrice(void) {}
        Matrice(const std::vector<std::vector<Complex>>& m) : mat(m) {}

        Complex getElem(int x, int y) const { return mat[y][x]; }
        void setElem(int x, int y, const Complex &elem) { mat[y][x] = elem; }

        const std::vector<std::vector<Complex>>& getMat() const { return mat; }
        std::vector<std::vector<Complex>>& getMat() { return mat; }

        std::string getMatString() const {
            std::ostringstream oss;
            oss << "[";
            for (size_t i = 0; i < mat.size(); i++) {
                oss << "[";
                for (size_t j = 0; j < mat[i].size(); j++) {
                    oss << mat[i][j];
                    if (j + 1 < mat[i].size())
                        oss << ", ";
                }
                oss << "]";
                if (i + 1 < mat.size())
                    oss << ";";
            }
            oss << "]";
            return oss.str();
        }

        Matrice operator+(const Matrice& b) const {
            if (mat.size() != b.mat.size() || mat.at(0).size() != b.mat.at(0).size())
                throw std::runtime_error("Matrix addition: incompatible size");
            Matrice r = *this;
            for (size_t i=0; i<mat.size(); ++i)
                for (size_t j=0; j<mat[0].size(); ++j)
                    r.mat[i][j] = r.mat[i][j] + b.mat[i][j];
            return r;
        }
        Matrice operator-(const Matrice& b) const {
            if (mat.size() != b.mat.size() || mat.at(0).size() != b.mat.at(0).size())
                throw std::runtime_error("Matrix subtraction: incompatible size");
            Matrice r = *this;
            for (size_t i=0; i<mat.size(); ++i)
                for (size_t j=0; j<mat[0].size(); ++j)
                    r.mat[i][j] = r.mat[i][j] - b.mat[i][j];
            return r;
        }
        Matrice operator*(const Matrice& b) const {
            if (mat[0].size() != b.mat.size())
                throw std::runtime_error("Matrix multiplication: incompatible size");
            std::vector<std::vector<Complex>> res(mat.size(), std::vector<Complex>(b.mat[0].size(), Complex()));
            for (size_t i=0; i<mat.size(); ++i)
                for (size_t j=0; j<b.mat[0].size(); ++j)
                    for (size_t k=0; k<mat[0].size(); ++k)
                        res[i][j] = res[i][j] + mat[i][k] * b.mat[k][j];
            return Matrice(res);
        }
        Matrice operator*(double d) const {
            Matrice r = *this;
            for (auto& row : r.mat)
                for (auto& val : row)
                    val = val * d;
            return r;
        }
        Matrice operator*(const Complex &c) const {
            Matrice r = *this;
            for (auto& row : r.mat)
                for (auto& val : row)
                    val = val * c;
            return r;
        }
        Matrice operator/(double d) const {
            if (!d)
                throw std::runtime_error("Error : division by 0 is forbidden");
            Matrice r = *this;
            for (auto& row : r.mat)
                for (auto& val : row)
                    val = val / d;
            return r;
        }
        Matrice operator/(const Complex &c) const {
            Matrice r = *this;
            for (auto& row : r.mat)
                for (auto& val : row)
                    val = val / c;
            return r;
        }

        void printMatrice() const {
            if (mat.empty())
                return;

            size_t cols = 0;
            for (const auto& row : mat)
                cols = std::max(cols, row.size());

            std::vector<size_t> widths(cols, 0);
            for (const auto& row : mat) {
                for (size_t j = 0; j < row.size(); ++j) {
                    std::ostringstream oss;
                    oss << row[j];
                    widths[j] = std::max(widths[j], oss.str().size());
                }
            }

            for (size_t i = 0; i < mat.size(); ++i) {
                std::cout << "[ ";
                for (size_t j = 0; j < mat[i].size(); ++j) {
                    std::ostringstream oss;
                    oss << mat[i][j];
                    std::cout << std::setw(static_cast<int>(widths[j])) << oss.str();
                    if (j + 1 < mat[i].size())
                        std::cout << " , ";
                }
                std::cout << " ]" << std::endl;
            }
        }

        friend Matrice operator*(double d, const Matrice& m) { return m * d; }
        friend Matrice operator*(const Complex &c, const Matrice& m) { return m * c; }
};

#endif
