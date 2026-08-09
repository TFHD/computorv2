#ifndef COMPLEX_HPP
# define COMPLEX_HPP

#include <iostream>

class Complex {

    private:
        double re;
        double im;
    
    public :
        Complex(double r = 0, double i = 0) : re(r), im(i) {}
        
        Complex operator+(const Complex &cplx) const {
            return Complex(this->re + cplx.re, this->im + cplx.im);
        }

        Complex operator-(const Complex &cplx) const {
            return Complex(this->re - cplx.re, this->im - cplx.im);
        }

        Complex operator*(const Complex &cplx) const {
            return Complex(this->re * cplx.re - this->im * cplx.im, this->re * cplx.im + this->im * cplx.re);
        }

        Complex operator/(const Complex &cplx) const {
            double denom = cplx.re * cplx.re + cplx.im * cplx.im;
            if (!denom)
                throw std::runtime_error("Error : division by 0 is forbidden");
            return Complex(
                (this->re * cplx.re + this->im * cplx.im) / denom,
                (this->im * cplx.re - this->re * cplx.im) / denom
            );
        }


        Complex operator/(double d) const {
            if (!d)
                throw std::runtime_error("Error : division by 0 is forbidden");
            return Complex(this->re / d, this->im / d);
        }


        Complex operator*(double d) const { return Complex(this->re * d, this->im * d); }
        Complex operator+(double d) const { return Complex(this->re + d, this->im); }
        Complex operator-(double d) const { return Complex(this->re - d, this->im); }

        Complex pow(int e) const {
            if (e == 0)
                return Complex(1, 0);
            bool neg = e < 0;
            if (neg)
                e = -e;
            Complex res(1, 0);
            Complex base = *this;
            while (e > 0) {
                if (e & 1)
                    res = res * base;
                base = base * base;
                e >>= 1;
            }
            if (neg)
                return 1.0 / res;
            return res;
        }

        friend Complex operator*(double d, const Complex& cplx) { return cplx * d; }
        friend Complex operator+(double d, const Complex& cplx) { return cplx + d; }
        friend Complex operator-(double d, const Complex& cplx) { return Complex(d - cplx.re, -cplx.im); }
        friend Complex operator/(double d, const Complex& cplx) {
            double denom = cplx.re * cplx.re + cplx.im * cplx.im;
            if (!denom)
                throw std::runtime_error("Error : division by 0 is forbidden");
            return Complex(
                (d * cplx.re) / denom,
                (-d * cplx.im) / denom
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

        static constexpr double EPSILON = 1e-10;

        static double snap(double x) {
            return (x > -EPSILON && x < EPSILON) ? 0.0 : x;
        }

        Complex cleaned() const {
            return Complex(snap(re), snap(im));
        }

        friend std::ostream &operator<<(std::ostream &o, const Complex &cplx) {
            Complex c = cplx.cleaned();
            if (c.getIm() == 0)
                o << c.getRe();
            else if (c.getIm() < 0)
                o << c.getRe() << " - " << -c.getIm() << "i";
            else
                o << c.getRe() << " + " << c.getIm() << "i";
            return (o);
        }
};


#endif