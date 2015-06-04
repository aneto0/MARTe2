/*
 * Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 express or implied.
 * See the Licence
 permissions and limitations under the Licence.
 *
 * $Id: $
 *
 **/

/** 
 * @file
 * @brief An implementation of the complex numerical type as 2 doubles
 */
#ifndef COMPLEX_H_
#define COMPLEX_H_

/** @brief Implements the main operation with complex numbers.
  * 
  * The attributes of this class which represents a complex number are simply two
  * double numbers for real and positive parts.
  *
  * These functions implement also overloading operators, the main operations 
  * on complex numbers considering also the particular case of real numbers (imaginary part 
  * equal to zero), including sine, cosine, log, square root and norm. 
  *
  * For the mathematical operations most of the implementation is delegated to math.h. */




#include <math.h>

class Complex {

    /** The real part. */
    double real;

    /** The imaginary part. */
    double imaginary;

public:
    /** @brief Constructor. */
    inline Complex() {
        real = 0;
        imaginary = 0;
    }

    /** 
     * @brief Constructor from a double real value.
     * @param x the real value to set. */
    inline Complex(double x) {
        real = x;
        imaginary = 0;
    }

    /** 
     * @brief Constructor from an integer real value.
     * @param x the real value to set. */
    inline Complex(int x) {
        real = x;
        imaginary = 0;
    }

    /** 
     * @brief Constructor from a double real value and a double imaginary value.
     * @param r the real value to set. 
     * @param i the imaginary value to set. */
    inline Complex(double r, double i) {
        real = r;
        imaginary = i;
    }

    /** 
     * @brief Constructor from an array.
     * The first index contains the real value and the second index the 
     * imaginary value.
     * @param r the real and complex value. */
    inline Complex(double r[2]) {
        if (r != NULL && (r + 1) != NULL) {
            real = r[0];
            imaginary = r[1];
        }
        else {
            real = 0;
            imaginary = 0;
        }
    }

    /**
     * @brief Copy constructor. 
     * This complex well get the real and imaginary parts from the argument.
     * @param x the complex value to be copied. */
    inline Complex(const Complex &x) {
        real = x.real;
        imaginary = x.imaginary;
    }

    /** 
     * @brief Get the real part.	
     * @return the real part of the complex. */
    inline double Real() {
        return real;
    }

    /** 
     * @brief Get the imaginary part.
     * @return the imaginary part of the complex. */
    inline double Imaginary() {
        return imaginary;
    }

    //For RTAI not to crash, must return Complex&
    /** 
     * @brief = operator. It will copy both the real and imaginary parts from the argument.
     * @param x the Complex object to be copied. */
    inline Complex& operator=(const Complex &x) {
        real = x.real;
        imaginary = x.imaginary;
        return *this;
    }

    /** 
     * @brief = operator. 
     * The real part will be equal to the argument and the imaginary part will be 0.
     * @param x the real part to be assigned. */
    inline Complex& operator=(double x) {
        real = x;
        imaginary = 0;
        return *this;
    }

    /** 
     * @brief = operator. 
     * The real part will be equal to the argument and the imaginary part will be 0.
     * @param x the real part to be assigned. */
    inline Complex& operator=(int x) {
        real = x;
        imaginary = 0;
        return *this;
    }

    /** 
     * @brief Performs the complex conjugate operation.
     * @param x is the complex to conjugate. */
    inline friend const Complex operator~(const Complex &x) {
        Complex ret;
        ret.real = x.real;
        ret.imaginary = -x.imaginary;
        return ret;
    }

    /**
     * @brief Performs the opposite operation.
     * @param x is the complex number. */
    inline friend const Complex operator-(const Complex &x) {
        Complex ret;
        ret.real = -x.real;
        ret.imaginary = -x.imaginary;
        return ret;
    }

    /**
     * @brief Sum between complexes.
     * @param a is the first complex number.
     * @param b is the second complex number.
     * @return the sum. */
    inline friend const Complex operator+(const Complex &a, const Complex &b) {
        Complex ret(a.real + b.real, a.imaginary + b.imaginary);
        return ret;
    }

    /**
     * @brief Sum considering only the real part.
     * @param a is the complex number.
     * @param b is the value to be summed to the real part of a.
     * @return the result of the sum. */
    inline friend const Complex operator+(const Complex &a, double b) {
        Complex ret(a.real + b, a.imaginary);
        return ret;
    }

    /**
     * @brief Sum the real part.
     * @param a is the value to be summed to the real part of a.
     * @param b is the complex number.
     * @return the result of the sum. */
    inline friend const Complex operator+(double a, const Complex &b) {
        Complex ret(a + b.real, b.imaginary);
        return ret;
    }

    /**
     * @brief Sum between complexes.
     * @param a is the complex number to sum to this. */
    inline void operator +=(const Complex &a) {
        real += a.real;
        imaginary += a.imaginary;
    }

    /**
     * @brief Sum considering only the real part.
     * @param a is the value to sum to the real part of this. */
    inline void operator +=(double a) {
        real += a;
    }

    /**
     * @brief Subtraction between complexes.
     * @param a is the first complex number.
     * @param b is the second complex number.
     * @return the result of the subtraction. */ 
    inline friend const Complex operator-(const Complex &a, const Complex &b) {
        Complex ret(a.real - b.real, a.imaginary - b.imaginary);
        return ret;
    }

    /**
     * @brief Subtract between a real and a complex.
     * @param a is value from which the real part of b must bu subtracted.
     * @param b is the complex number.
     * @return a complex with the same imaginary part of b and the difference between a and the real part of b as real part. */ 
    inline friend const Complex operator-(double a, const Complex &b) {
        Complex ret(a - b.real, - b.imaginary);
        return ret;
    }

    /**
     * @brief Subtract between a complex and a real.
     * @param a is the complex number.
     * @param b is value to subtract to the real part of a.
     * @return a complex with the same imaginary part of b and the difference the real part of b and a as real part. */ 
    inline friend const Complex operator-(const Complex &a, double b) {
        Complex ret(a.real - b, a.imaginary);
        return ret;
    }

    /**
     * @brief Subtraction between complexes.
     * @param a is the complex number to subtract to this. */
    inline void operator -=(const Complex &a) {
        real -= a.real;
        imaginary -= a.imaginary;
    }

    /**
     * @brief Subtract between a complex and a real.
     * @param a is value to subtract to the real part of this. */ 
    inline void operator -=(double a) {
        real -= a;
    }

    /**
     * @brief Performs the product operation between complexes.
     * @param a is the first complex number.
     * @param b is the second complex number.
     * @return the product of them. */     
    inline friend const Complex operator*(const Complex &a, const Complex &b) {
        Complex ret(a.real * b.real - a.imaginary * b.imaginary,
                    a.real * b.imaginary + b.real * a.imaginary);
        return ret;
    }

    /**
     * @brief Performs the product operation between a complex and a real.
     * @param a is the first complex number.
     * @param b is the real number.
     * @return the product of them. */  
    inline friend const Complex operator*(const Complex &a, double b) {
        Complex ret(a.real * b, a.imaginary * b);
        return ret;
    }

    /**
     * @brief Performs the product operation between a real and a complex.
     * @param a is the real number.
     * @param b is the complex number.
     * @return the product of them. */
    inline friend const Complex operator*(double a, const Complex &b) {
        Complex ret(b.real * a, b.imaginary * a);
        return ret;
    }

    /**
     * @brief Performs the product operation between complexes.
     * @param a is the complex number to multiply to this. */
    inline void operator *=(const Complex &a) {
        *this = *this * a;
    }

    /**
     * @brief Performs the product operation between a complex and a real.
     * @param a is the real number to multiply to this. */
    inline void operator *=(double a) {
        real *= a;
        imaginary *= a;
    }

    /**
     * @brief Performs the division operation between complexes.
     * @param a is the first complex number.
     * @param b is the second complex number.
     * @return the division result. */
    inline friend const Complex operator/(const Complex &a, const Complex &b) {
        Complex ret(a);
        ret /= b.Norma2();
        ret *= ~b;
        return ret;
    }

    /**
     * @brief Performs the division operation between a complex and a real.
     * @param a is the complex number.
     * @param b is the real number.
     * @return the division result. */
    inline friend const Complex operator/(const Complex &a, double b) {
        Complex ret = a;
        ret /= b;
        return ret;
    }

    /**
     * @brief Performs the division operation between a real and a complex.
     * @param a is the real number.
     * @param b is complex number.
     * @return the division result. */
    inline friend const Complex operator/(double a, const Complex &b) {
        Complex ret = a;
        ret /= b.Norma2();
        ret *= ~b;
        return ret;
    }

    /**
     * @brief Performs the division operation between complexes.
     * @param a is the complex number which will divide this.
     * @return the division result. */
    inline void operator /=(const Complex &a) {
        *this = *this / a;
    }

    /**
     * @brief Performs the division operation between complex and real.
     * @param a is the real number which will divide this.
     * @return the division result. */
    inline void operator /=(double a) {
        real /= a;
        imaginary /= a;
    }

    /**
     * @brief Compares using the Norma2() (Square of the norm).
     * @param x is the complex to be compared with this.
     * @return true if this Norma2() is greater.   */
    inline bool operator <(const Complex &x) {
        return (Norma2() < x.Norma2());
    }

    /**
     * @brief Compares using the Norma2() (Square of the norm).
     * @param x is the complex to be compared with this.
     * @return true if this Norma2() is minor.   */
    inline bool operator >(const Complex &x) {
        return (Norma2() > x.Norma2());
    }

    /**
     * @brief Equality operator.
     * @param x is the complex to be compared with this.
     * @return true if it is equal to this. */
    inline bool operator ==(const Complex &x) {
        return ((real == x.real) && (imaginary == x.imaginary));
    }

    /**
     * @brief Equality operator with a real number.
     * @param x is the real number to be compared with this.
     * @return true if it is equal to this. */
    inline bool operator ==(double x) {
        return ((real == x) && (imaginary == 0));
    }

    /**
     * @brief Get the square of the norm.
     * @return the square of the norm. */
    inline double Norma2() const {
        return real * real + imaginary * imaginary;
    }

    /**
     * @brief Get the norm.
     * @return the norm. */
    inline double Norma() const {
        return sqrt(Norma2());
    }

    /**
     * @brief Computes the complex argument (or phase).
     * @return the complex argument. */
    inline double Arg() const {
        if (real == 0) {
            if (imaginary > 0) {
                return 1.570704;
            }
            return -1.570704;
        }
        double arg = atan(imaginary / real);
        if (real > 0) {
            return arg;
        }
        else if (imaginary >= 0) {
            return 3.1415 + arg;
        }
        else {
            return -3.1415 + arg;
        }

    }

    /**   
     * @brief Compute the sine of a complex number.
     * @param x is the complex argument.
     * @return the sine result. */
    inline friend const Complex sin(const Complex &x) {
        return Complex(sin(x.real) * cosh(x.imaginary),
                       cos(x.real) * sinh(x.imaginary));
    }

    /**   
     * @brief Compute the cosine of a complex number.
     * @param x is the complex argument.
     * @return the cosine result. */
    inline friend const Complex cos(const Complex &x) {
        return Complex(cos(x.real) * cosh(x.imaginary),
                       -sin(x.real) * sinh(x.imaginary));
    }

    /**   
     * @brief Compute the exponential of a complex number.
     * @param x is the complex argument.
     * @return the exponential operation result. */
    inline friend const Complex exp(const Complex &x) {
        return Complex(exp(x.real) * cos(x.imaginary),
                       exp(x.real) * sin(x.imaginary));
    }

    /**   
     * @brief Compute the logarithm of a complex number.
     * @param x is the complex argument.
     * @return the logarithm result. */
    inline friend const Complex log(const Complex &x) {
        return Complex(log(x.Norma()), x.Arg());
    }

    /**   
     * @brief Compute the square root of a complex number.
     * @param x is the complex argument.
     * @return the square root result. */
    inline friend const Complex sqrt(const Complex &x) {
        double norma = sqrt(x.Norma());
        double arg = x.Arg() / 2;

        return Complex(norma * cos(arg), norma * sin(arg));
    }

    /**   
     * @brief Compute the square of a complex number.
     * @param x is the complex argument.
     * @return the square result. */
    inline friend const Complex sqr(const Complex &x) {
        return Complex(x.real * x.real - x.imaginary * x.imaginary,
                       x.real * x.imaginary + x.real * x.imaginary);
    }
};

extern "C" {

/** @brief Compute the logarithm of a real giving a complex as result.
  * This allows to compute logarithm of negative real numbers.
  * @return the logarithm result. */
inline const Complex Clog(double x) {
    if (x > 0)
        return Complex(log(x), 0);
    else
        return Complex(log(-x), 3.1415);
}

/** @brief Compute the square root of a real giving a complex as result.
  * This allows to compute square root of negative real numbers.
  * @return the square root result. */
inline const Complex Csqrt(double x) {
    if (x > 0)
        return Complex(sqrt(x), 0);
    return Complex(0, sqrt(-x));
}
}

#endif

