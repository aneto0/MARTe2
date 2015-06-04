/* Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http: //ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 express or implied.
 * See the Licence
 permissions and limitations under the Licence.
 *
 * $Id:$
 *
 **/

#include "GeneralDefinitions.h"
#include "ComplexTest.h"

//Return true if two double numbers are equal less than a given tolerance.
bool equalDouble(double a, double b, double tolerance) {

    return (a - b) < tolerance && (a - b) > (-tolerance);
}

bool ComplexTest::TestConstructors() {

    //Test empty constructor (real and imaginary parts equal to zero).
    Complex myComplex1;
    if (myComplex1.Real() != 0 || myComplex1.Imaginary() != 0) {
        return False;
    }

    //Test the constructor with only the real part as double argument.
    double doubleRealPart = 2.5;
    Complex myComplex2(doubleRealPart);
    if (myComplex2.Real() != doubleRealPart || myComplex2.Imaginary() != 0) {
        return False;
    }

    //Test the constructor with only the real part as int32 argument.
    int32 intRealPart = 2;
    Complex myComplex3(intRealPart);
    if (myComplex3.Real() != intRealPart || myComplex3.Imaginary() != 0) {
        return False;
    }

    //Test the constructor with doubles real and imaginary parts as argument.
    double doubleImaginaryPart = 3.6;
    Complex myComplex4(doubleRealPart, doubleImaginaryPart);
    if (myComplex4.Real() != doubleRealPart
            || myComplex4.Imaginary() != doubleImaginaryPart) {
        return False;
    }

    //Test the constructor with an array of two elements passed by argument.
    //If we pass NULL as argument it initialize real and imaginary to zero.
    Complex myFailComplex((double*) NULL);

    if (myFailComplex.Real() != 0 || myFailComplex.Imaginary() != 0) {
        return False;
    }

    double complexValues[1];
    complexValues[0] = doubleRealPart;
    complexValues[1] = doubleImaginaryPart;
    Complex myComplex5(complexValues);
    if (myComplex5.Real() != complexValues[0]
            || myComplex5.Imaginary() != complexValues[1]) {
        return False;
    }

    //Test the constructor that copies another complex
    Complex copyComplex(myComplex5);
    if (!(myComplex5 == copyComplex)) {
        return False;
    }

    return True;
}

bool ComplexTest::TestAddAndDifferenceOperators() {

    //Assign a complex using a double (assigns the real part) and test the equality operator with a double (checks the real part)
    Complex doubleRef = 6.8;
    if (!(doubleRef == 6.8)) {
        return False;
    }

    //Assigns an int number as the real part (the imaginary part becomes zero).
    Complex intRef = 6;
    if (!(intRef == 6) && intRef.Real() == 6 && intRef.Imaginary() == 0) {
        return False;
    }

    Complex zero = 0;

    //Create two equal complexes.
    Complex myComplex1(1, 1);
    Complex myComplex2 = myComplex1;

    //Test the equality operator between two complexes.
    if (!(myComplex1 == myComplex2)) {
        return False;
    }

    double sumOnRe1 = 5.5;
    double sumOnRe2 = 4.5;

    //Tests the sum operator double+complex and complex+double.
    Complex myComplex3 = myComplex1 + sumOnRe1;
    Complex myComplex4 = sumOnRe2 + myComplex2;

    Complex testMathComplex((sumOnRe1 + sumOnRe2 + myComplex1.Real() * 2), 2);

    //Tests the sum operator between two complexes.
    Complex sumComplex = myComplex3 + myComplex4;

    //Check if the result is correct.
    if (!(sumComplex == testMathComplex)) {
        return False;
    }

    //Tests the += operator with a double.
    myComplex1 += sumOnRe1;
    myComplex2 += sumOnRe2;

    //Check if the result is correct.
    sumComplex = myComplex1 + myComplex2;
    if (!(sumComplex == testMathComplex)) {
        return False;
    }

    //Tests the += operator between two complexes.
    myComplex1 += myComplex2;

    //Check if the result is correct.
    if (!(myComplex1 == testMathComplex)) {
        return False;
    }

    //Tests the - operator to change the sign of a complex.
    Complex zeroTest = myComplex2 + (-myComplex2);
    if (!(myComplex2 == (-(-myComplex2))) && zeroTest == zero) {
        return False;
    }

    //Tests the subtraction between two complexes and the -= operator.
    testMathComplex = myComplex1 - myComplex2;
    myComplex1 -= myComplex2;

    //Check if both give the same result.
    if (!(myComplex1 == testMathComplex)) {
        return False;
    }

    //Tests the subtraction complex-double and double-complex. 
    myComplex3 = myComplex1 - sumOnRe1;
    myComplex4 = sumOnRe2 - myComplex2;

    //Tests the -= operator between complex and double and the conjugate operator.
    myComplex1 -= sumOnRe1;
    myComplex2 -= sumOnRe2;
    myComplex2 = -myComplex2;

    //Check if the operations with these operators give the same result.
    if (!(myComplex1 == myComplex3) || !(myComplex2 == myComplex4)) {
        return False;
    }

    return True;
}

bool ComplexTest::TestProductOperators() {
    double rePart = 3.3;
    double imPart = 4.4;

    //Initialize a complex and check the *= operator with an int (or a double)
    Complex myComplex1(rePart, imPart);
    Complex myOppComplex1 = myComplex1 * (-1);

    if (!(myOppComplex1 == (-myComplex1))) {
        return False;
    }

    myComplex1 *= -1;
    if (!(myOppComplex1 == myComplex1)) {
        return False;
    }

    double productNum = 3.2;

    //Tests the * operator between a complex and a double.
    Complex myComplex2(myComplex1);
    myComplex1 = myComplex2 * (-productNum);
    if (myComplex1.Real() != (rePart * productNum)
            || myComplex1.Imaginary() != (imPart * productNum)) {
        return False;
    }

    //Tests the * operator between a double and a complex.
    Complex testMath = (-productNum) * myComplex2;
    if (!(testMath == myComplex1)) {
        return False;
    }

    double re1Now = myComplex1.Real();
    double re2Now = myComplex2.Real();
    double im1Now = myComplex1.Imaginary();
    double im2Now = myComplex2.Imaginary();

    //Tests the * operator between two complexes and chek if the result is mathematically correct.
    testMath = myComplex1 * myComplex2;

    double expectedRe = re1Now * re2Now - im1Now * im2Now;
    double expectedIm = re1Now * im2Now + re2Now * im1Now;

    if (testMath.Real() != expectedRe || testMath.Imaginary() != expectedIm) {
        return False;
    }

    //Tests the *= operator between two complexes and checks if the result is the same of the previous result.
    myComplex1 *= myComplex2;

    if (!(myComplex1 == testMath)) {
        return False;
    }

    return True;
}

bool ComplexTest::TestNormAndDivision() {
    double rePart = 3.3;
    double imPart = 4.4;
    Complex myComplex1(rePart, imPart);

    double divNumber = 1.2;
    Complex testMath = myComplex1;

    //Tests the /= operator (complex/double) and the / operator (complex double)
    //the two operations must returns the same result.
    myComplex1 /= divNumber;
    testMath = testMath / divNumber;

    if (myComplex1.Real() != (rePart / divNumber)
            || myComplex1.Imaginary() != (imPart / divNumber)
            || !(testMath == myComplex1)) {
        return False;
    }

    //Checks if the division of a complex with itself it's the real one.
    Complex unity = 1.0;

    testMath /= testMath;

    if (!(testMath == unity)) {
        return False;
    }

    //Calulates the norm of myComplex1. 
    double re1Now = myComplex1.Real();
    double im1Now = myComplex1.Imaginary();

    double testNorm2 = re1Now * re1Now + im1Now * im1Now;
    double norm = myComplex1.Norma();

    //Checks if the functions Norma() and Norma2() are correct.
    if (!equalDouble((norm * norm), testNorm2, 1e-6)
            || !equalDouble(myComplex1.Norma2(), testNorm2, 1e-6)) {
        return False;
    }

    //Tests the division between a double and a complex.
    testMath = myComplex1;
    Complex myComplex2 = divNumber / myComplex1;

    //Calculate the result using the mathematic formula.
    testMath = divNumber * (~testMath) / testMath.Norma2();

    //Checks if the division operation returns the right result.
    if (!(testMath == myComplex2)) {
        return False;
    }

    double newRePart = 5.7;
    double newImPart = 0.6;

    //Tests the division between two complexes.
    Complex newComplex(newRePart, newImPart);

    //Calculate the result with the mathematic formula.
    testMath = newComplex * (~myComplex1) / myComplex1.Norma2();
    myComplex2 = newComplex / myComplex1;

    //Checks if the result is correct
    if (!(testMath == myComplex2)) {
        return False;
    }

    //Tests the < and > operators which can give a sorting of complexes by their norma.
    if (myComplex1 > newComplex) {
        if (myComplex1.Norma2() < newComplex.Norma2()) {
            return False;
        }
        if (newComplex > myComplex1) {
            return False;
        }
    }
    else if (myComplex1 < newComplex) {
        if (myComplex1.Norma2() > newComplex.Norma2()) {
            return False;
        }
        if (newComplex < myComplex1) {
            return False;
        }
    }

    //Checks if a real positive number has a null phase.
    if (!equalDouble(unity.Arg(), 0, 1e-4)) {
        return False;
    }

    //Checks if a real negative number has a phase equal to pi
    if (!equalDouble((-unity).Arg(), pi, 1e-4)) {
        return False;
    }

    //Checks if a pure imaginary positive number has a phase equal to pi/2.
    newComplex -= newComplex.Real();
    if (!equalDouble(newComplex.Arg(), (pi / 2.0), 1e-4)) {
        return False;
    }

    double rePartUp = 0.0;
    double imPartUp = 1.0;
    double rePartDown = 0.0;
    double imPartDown = -1.0;
    double step = 1e-3;

    //Checks if the phase for complexes in the second and third quadrants is in the correct range.
    while (imPartUp > 0 && rePartUp > -1) {
        Complex testArgComplexUp(rePartUp, imPartUp);
        Complex testArgComplexDown(rePartDown, imPartDown);
        if ((testArgComplexUp.Arg() < (pi / 2 - 1e-3)
                && testArgComplexUp.Arg() > (pi + 1e-3))
                || (testArgComplexDown.Arg() > (-pi / 2 + 1e-3)
                        && testArgComplexDown.Arg() < (-pi - 1e-3))) {
            return False;
        }
        rePartUp -= step;
        imPartUp -= step;
        rePartDown -= step;
        imPartDown += step;
    }

    return True;

}

bool ComplexTest::TestMathOperations() {
    double rePart = 5.8;
    double imPart = 6.7;

    //Initialize a complex.
    Complex testComplex(rePart, imPart);

    //Save the expected result for a sine operation.
    double expectedRe = -188.723044;
    double expectedIm = 359.7;

    Complex result = sin(testComplex);

    //Check if the sine operation returns the expected result.
    if (!equalDouble(result.Real(), expectedRe, 1e-4)
            || !equalDouble(result.Imaginary(), expectedIm, 1e-4)) {
        return False;
    }

    //Check the result for the cosine operation.
    expectedRe = 359.7011519;
    expectedIm = 188.722472;

    result = cos(testComplex);
    if (!equalDouble(result.Real(), expectedRe, 1e-4)
            || !equalDouble(result.Imaginary(), expectedIm, 1e-4)) {
        return False;
    }

    //Check the result for the exponential operation.
    expectedRe = 302.02035;
    expectedIm = 133.72175;

    result = exp(testComplex);
    if (!equalDouble(result.Real(), expectedRe, 1e-4)
            || !equalDouble(result.Imaginary(), expectedIm, 1e-4)) {
        return False;
    }

    //Check the result for the logarithm operation.
    expectedRe = 2.18174;
    expectedIm = 0.857274;

    result = log(testComplex);
    if (!equalDouble(result.Real(), expectedRe, 1e-4)
            || !equalDouble(result.Imaginary(), expectedIm, 1e-4)) {
        return False;
    }

    //Check the result for the square root operation.
    expectedRe = 2.7076;
    expectedIm = 1.2373;

    result = sqrt(testComplex);
    if (!equalDouble(result.Real(), expectedRe, 1e-4)
            || !equalDouble(result.Imaginary(), expectedIm, 1e-4)) {
        return False;
    }

    //Check the result for the square operation.
    expectedRe = rePart * rePart - imPart * imPart;
    expectedIm = rePart * imPart * 2;
    result = sqr(testComplex);
    if (!equalDouble(result.Real(), expectedRe, 1e-4)
            || !equalDouble(result.Imaginary(), expectedIm, 1e-4)) {
        return False;
    }

    //Test the logarithm of a negative number.
    double negativeNumber = -(e * e * e);
    expectedRe = 3.0;
    expectedIm = pi;
    result = Clog(negativeNumber);
    if (!equalDouble(result.Real(), expectedRe, 1e-4)
            || !equalDouble(result.Imaginary(), expectedIm, 1e-4)) {
        return False;
    }

    //Test if it works also for positive numbers.
    double positiveNumber = -negativeNumber;
    expectedRe = 3.0;
    expectedIm = 0;
    result = Clog(positiveNumber);
    if (!equalDouble(result.Real(), expectedRe, 1e-4)
            || !equalDouble(result.Imaginary(), expectedIm, 1e-4)) {
        return False;
    }

    //Test the square root of a negative number.
    expectedRe = 0;
    result = Csqrt(negativeNumber);

    if (!equalDouble(result.Imaginary() * result.Imaginary(), -negativeNumber,
                     1e-4) || !equalDouble(result.Real(), expectedRe, 1e-4)) {
        return False;
    }

    //Test if it works also for positive numbers.
    expectedIm = 0;
    result = Csqrt(positiveNumber);

    if (!equalDouble(result.Real() * result.Real(), positiveNumber, 1e-4)
            || !equalDouble(result.Imaginary(), expectedIm, 1e-4)) {
        return False;
    }

    return True;

}

