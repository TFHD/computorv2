#ifndef MATHUTILS_HPP
# define MATHUTILS_HPP

struct Math {
    static double dabs(double x) {
        return (x < 0) ? -x : x; 
    }

    static float getMin(float x, float y) {
        return (x < y) ? x : y;
    }

    static float getMax(float x, float y) {
        return (x > y) ? x : y;
    }

    static double myAbs(double x, double y) {

        x = dabs(x);
        y = dabs(y);

        if (x > y)  return x - y;
        else        return y - x;
    }

    static double my_sqrt(double x) {

        if (x < 0)
            return -1;
        if (x == 0)
            return 0;
        double guess = x;
        double epsilon = 1e-10;
        int i = 10000;
        while (Math::dabs(guess * guess - x) > epsilon && i > 0) {
            guess = (guess + x / guess) / 2.0;
            i--;
        }
        return guess;
    }

    static double pow(double x, int e) {

        double res = 1;
        bool neg = e < 0;
        if (neg)
            e = -e;
        while (e-- > 0) {
            res *= x;
        }
        if (neg)
            return 1 / res;
        return res;
    }
};

#endif