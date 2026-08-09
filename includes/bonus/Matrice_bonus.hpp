#ifndef MATRICE_BONUS_HPP
# define MATRICE_BONUS_HPP

# include "Complex_bonus.hpp"
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

        Matrice pow(int e) const {
            if (mat.empty() || mat.size() != mat[0].size())
                throw std::runtime_error("Matrix exponentiation: matrix must be square");
            if (e < 0)
                throw std::runtime_error("Matrix exponentiation: negative exponent forbidden");
            size_t n = mat.size();
            std::vector<std::vector<Complex>> id(n, std::vector<Complex>(n, Complex()));
            for (size_t i = 0; i < n; ++i)
                id[i][i] = Complex(1, 0);
            Matrice res(id);
            if (e == 0)
                return res;
            Matrice base = *this;
            while (e > 0) {
                if (e & 1)
                    res = res * base;
                base = base * base;
                e >>= 1;
            }
            return res;
        }

        bool isSquare() const {
            return !mat.empty() && mat.size() == mat[0].size();
        }

        Matrice transpose() const {
            if (mat.empty())
                return Matrice();
            size_t rows = mat.size();
            size_t cols = mat[0].size();
            std::vector<std::vector<Complex>> t(cols, std::vector<Complex>(rows));
            for (size_t i = 0; i < cols; ++i)
                for (size_t j = 0; j < rows; ++j)
                    t[i][j] = mat[j][i];
            return Matrice(t);
        }

        Matrice toMinor(size_t row, size_t col) const {
            if (mat.empty() || mat.size() < 2 || mat[0].size() < 2)
                throw std::runtime_error("Matrix minor: matrix too small");
            size_t rows = mat.size();
            size_t cols = mat[0].size();
            std::vector<std::vector<Complex>> minor(rows - 1, std::vector<Complex>(cols - 1));
            size_t minor_row = 0;

            for (size_t i = 0; i < rows; ++i) {
                size_t minor_col = 0;
                bool is_find = false;
                for (size_t j = 0; j < cols; ++j) {
                    if (i != row - 1 && j != col - 1) {
                        minor[minor_row][minor_col++] = mat[i][j];
                        is_find = true;
                    }
                }
                if (is_find)
                    ++minor_row;
            }
            return Matrice(minor);
        }

        Complex cofactor(size_t row, size_t col) const {
            Complex sign = ((row + col) % 2 == 0) ? Complex(1, 0) : Complex(-1, 0);
            return sign * toMinor(row, col).determinant();
        }

        Complex determinant() const {
            if (!isSquare())
                throw std::runtime_error("Matrix determinant: matrix must be square");
            size_t n = mat.size();
            if (n == 1)
                return mat[0][0];
            if (n == 2)
                return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
            Complex res;
            for (size_t i = 0; i < n; ++i)
                res = res + mat[0][i] * cofactor(1, i + 1);
            return res;
        }

        Matrice inverse() const {
            Complex det = determinant();
            if (Complex::snap(det.getRe()) == 0 && Complex::snap(det.getIm()) == 0)
                throw std::runtime_error("Inverse : Determinant is null");
            size_t n = mat.size();
            if (n == 1)
                return Matrice(std::vector<std::vector<Complex>>{{
                    (Complex(1, 0) / mat[0][0]).cleaned()
                }});
            std::vector<std::vector<Complex>> inv(n, std::vector<Complex>(n, Complex()));
            for (size_t i = 0; i < n; ++i)
                for (size_t j = 0; j < n; ++j)
                    inv[i][j] = cofactor(i + 1, j + 1);
            Matrice res = Matrice(inv).transpose() / det;
            for (auto &row : res.mat)
                for (auto &val : row)
                    val = val.cleaned();
            return res;
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
