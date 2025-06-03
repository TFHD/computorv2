#ifndef COMPLEX_HPP
# define COMPLEX_HPP

#include <iostream>

class Complex {

    private:
        double re;
        double im;
    
    public :
        Complex(double r = 0, double i = 0) : re(r), im(i) {}
        
        Complex operator+(const Complex &cplx) {
            return Complex(this->re + this->re, cplx.im + cplx.im);
        }

        Complex operator-(const Complex &cplx) {
            return Complex(this->re - cplx.re, this->im - cplx.im);
        }

        Complex operator*(const Complex &cplx) {
            return Complex(this->re * cplx.re - this->im * cplx.im, this->re * cplx.im + this->im * cplx.re);
        }

        Complex operator/(const Complex &cplx) {
            double denom = cplx.re * cplx.re + cplx.im * cplx.im;
            if (!denom)
                throw std::runtime_error("Error : division by 0 is forbidden");
            return Complex(
                (this->re * cplx.re + this->im * cplx.im) / denom,
                (this->im * cplx.re - this->re * cplx.im) / denom
            );
        }   

        double getRe(void) const {
            return this->re;
        }

        double getIm(void) const {
            return this->im;
        }

        void setRe(double &re) {
            this->re = re;
        }

        void setIm(double &im) {
            this->im = im;
        }

};

// std::ostream &operator<<(std::ostream &o, const Complex &cplx) {
//     if (cplx.getIm() == 0)
//         o << cplx.getRe();
//     else if (cplx.getIm() < 0)
//         o << cplx.getRe() << " - " << -cplx.getIm() << "i";
//     else if (cplx.getIm() > 0)
//         o << cplx.getRe() << " + " << cplx.getIm() << "i";
//     return (o);
// }

#endif