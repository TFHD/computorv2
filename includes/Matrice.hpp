#ifndef MATRICE_HPP
# define MATRICE_HPP

# include <vector>

class Matrice {
    private:
        std::vector<std::vector<double>> mat;
    public:
        Matrice(void) {}
        Matrice(const std::vector<std::vector<double>>& m) : mat(m) {}

        double getElem(int x, int y) const { return mat[y][x]; }
        void setElem(int x, int y, double elem) { mat[y][x] = elem; }

        const std::vector<std::vector<double>>& getMat() const { return mat; }
        std::vector<std::vector<double>>& getMat() { return mat; }

        Matrice operator+(const Matrice& b) const {
            if (mat.size() != b.mat.size() || mat.at(0).size() != b.mat.at(0).size())
                throw std::runtime_error("Matrix addition: incompatible size");
            Matrice r = *this;
            for (size_t i=0; i<mat.size(); ++i)
                for (size_t j=0; j<mat[0].size(); ++j)
                    r.mat[i][j] += b.mat[i][j];
            return r;
        }
        Matrice operator-(const Matrice& b) const {
            if (mat.size() != b.mat.size() || mat.at(0).size() != b.mat.at(0).size())
                throw std::runtime_error("Matrix subtraction: incompatible size");
            Matrice r = *this;
            for (size_t i=0; i<mat.size(); ++i)
                for (size_t j=0; j<mat[0].size(); ++j)
                    r.mat[i][j] -= b.mat[i][j];
            return r;
        }
        Matrice operator*(const Matrice& b) const {
            if (mat[0].size() != b.mat.size())
                throw std::runtime_error("Matrix multiplication: incompatible size");
            std::vector<std::vector<double>> res(mat.size(), std::vector<double>(b.mat[0].size(), 0.0));
            for (size_t i=0; i<mat.size(); ++i)
                for (size_t j=0; j<b.mat[0].size(); ++j)
                    for (size_t k=0; k<mat[0].size(); ++k)
                        res[i][j] += mat[i][k] * b.mat[k][j];
            return Matrice(res);
        }
        Matrice operator*(double d) const {
            Matrice r = *this;
            for (auto& row : r.mat) for (auto& val : row) val *= d;
            return r;
        }
        Matrice operator/(double d) const {
            Matrice r = *this;
            for (auto& row : r.mat) for (auto& val : row) val /= d;
            return r;
        }

        void printMatrice() const {
            for (size_t i = 0; i < mat.size(); i++) {
                std::cout << "[ ";
                for (size_t j = 0; j < mat[i].size(); j++) {
                    std::cout << mat[i][j];
                    if (j + 1 < mat[i].size())
                        std::cout << " , ";
                }
                std::cout << " ]" << std::endl;
            }
        }

        friend Matrice operator*(double d, const Matrice& m) { return m * d; }
};

#endif