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
#include "FloatToStreamTest.h"
#include "StringTestHelper.h"
#include "StreamTestHelper.h"
#include "stdio.h"

bool FloatToStreamTest::TestFixedPoint() {
    MyStream thisStream;
    float sbit32 = -1.1234567;
    FormatDescriptor format;
    const char* pformat;

    //Left padded, negative number, exact precision.
    pformat = "- 12.7f";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-1.1234567  ", thisStream.Buffer())) {
        return False;
    }

    thisStream.Clear();

    //6 decimal numbers by default.
    pformat = "f";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-1.123457", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Less precision.
    pformat = "- 12.3f";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-1.123      ", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Right aligned padding.
    sbit32 = 112345.67; //112345,...
    pformat = " 12.2f";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("   112345.67", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Print 0
    sbit32 = 0; //112345,...
    pformat = " 3.2f";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("  0", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Underflow
    sbit32 = 0.09; //112345,...
    pformat = " 1.2f";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("0", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Round up correction.
    double sbit64 = 12345.9999;
    pformat = " 10.3f";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare(" 12346.000", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Get the integer part.
    pformat = " 10.0f";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("     12346", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Over precision
    sbit64 = 999999.99996;
    pformat = " 17.9f"; //
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare(" 999999.999960000", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();
    pformat = " 6.4f"; //

    //Overflow and not enough space, because the rounding up we need at least seven digits.
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("     ?", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Point removing and overflow.
    pformat = "- 8.4f"; //
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);

    thisStream.PutC(0);
    if (!StringTestHelper::Compare("1000000 ", thisStream.Buffer())) {
        return False;
    }

    //Enough space for the point and a decimal number.
    thisStream.Clear();
    pformat = "- 9.4f"; //
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("1000000.0", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //No space for the fraction part.
    sbit64 = -222222.5255;
    pformat = "7.4f";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);

    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-222223", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //No space for the fraction part again.
    pformat = " 8.4f";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare(" -222223", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Space enouh for one decimal number. Approximation at the second digit after .
    pformat = " 9.4f";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);

    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-222222.5", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Space enouh for two decimal numbers. Approximation at the third digit after .
    pformat = " 10.4f";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);

    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-222222.53", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Not enough space for the integer part.
    pformat = " 5.4f";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);

    thisStream.PutC(0);
    if (!StringTestHelper::Compare("    ?", thisStream.Buffer())) {
        return False;
    }

    thisStream.Clear();

    //Negative exponent.
    sbit64 = 0.009;
    pformat = " 5.2f";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);

    thisStream.PutC(0);
    if (!StringTestHelper::Compare(" 0.01", thisStream.Buffer())) {
        return False;
    }

    thisStream.Clear();

    //Not enough space (it must be at least 12+1(zero)+1(point)=14)
    __float128 sbit128 = 1e-12Q;
    pformat = " 12.10f";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("           0", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Not enough space with 1 as maxSize and for negative number (it must be at least 12+1(zero)+1(point)=14)
    sbit128 = -1e-12Q;
    pformat = " 1.10f";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("0", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Positive inf.
    float inf = 1.0 / 0.0;
    pformat = "10.10f";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), inf, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("+Inf", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Negative inf
    double ninf = -1.0 / 0.0;
    pformat = " 10.10f";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), ninf, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("      -Inf", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Not enough space to print -Inf
    pformat = "- 1.10f";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), ninf, format);

    thisStream.PutC(0);
    if (!StringTestHelper::Compare("?", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //NaN
    __float128 nan = 0.0 / 0.0;
    pformat = "- 10.10f";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), nan, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("NaN       ", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Nan with automatic size
    pformat = " 0.10f";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), nan, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("NaN", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Not enough space to print Nan
    pformat = " 2.10f";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), nan, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare(" ?", thisStream.Buffer())) {
        return False;
    }

    return True;

}

bool FloatToStreamTest::TestFixedRelativePoint() {
    MyStream thisStream;
    float sbit32 = -1.1234567;
    FormatDescriptor format;
    const char* pformat;

    //Left padded, negative number, exact precision
    pformat = "- 12.8F";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-1.1234567  ", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Automatic precision set to 7 for 32 bit numbers.
    pformat = "- 12.F";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-1.123457   ", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Precision 0.
    pformat = "- 12.0F";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("0           ", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Less precision.
    pformat = "- 12.4F";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-1.123      ", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Right aligned padding
    sbit32 = 112345.67; //112345,...
    pformat = " 12.8F";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("   112345.67", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Round up correction
    double sbit64 = 12345.9999;
    pformat = " 10.8F";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare(" 12346.000", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Less precision than the size of integer part.
    pformat = " 10.3F";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("     12300", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Over precision.
    sbit64 = 999999.99996;
    pformat = " 17.15F"; //
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare(" 999999.999960000", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Overflow and not enough space because the round up.
    pformat = " 6.10F"; //
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("     ?", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Overflow, no space for decimals.
    pformat = "- 8.10F"; //
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("1000000 ", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Enough space for the point and a decimal number.
    pformat = "- 9.10F"; //
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("1000000.0", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //No space for decimal numbers 
    sbit64 = -222222.5255;
    pformat = "7.10F"; //
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-222223", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //No space for decimal numbers again.
    pformat = " 8.10F";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare(" -222223", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Space enough for one decimal number. Approximation at the second digit after .
    pformat = " 9.10F";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-222222.5", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Space enouh for two numbers. Approximation at the third digit after .
    pformat = " 10.10F";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-222222.53", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Not enough space for the integer part
    pformat = " 5.10F";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("    ?", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Not enough space (it must be at least 12+1(zero)+1(point)=14)
    __float128 sbit128 = 1e-12Q;
    pformat = " 12.12F";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("           0", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Poisitive inf.
    float inf = 1.0 / 0.0;
    pformat = "10.10F";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), inf, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("+Inf", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Negative inf
    double ninf = -1.0 / 0.0;
    pformat = " 10.10F";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), ninf, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("      -Inf", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Not enough space to print -Inf
    pformat = "- 1.10F";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), ninf, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("?", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //NaN
    __float128 nan = 0.0 / 0.0;
    pformat = "- 10.10F";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), nan, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("NaN       ", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Nan with automatic size
    pformat = " 0.10F";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), nan, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("NaN", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Not enough space to print Nan
    pformat = " 2.10F";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), nan, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare(" ?", thisStream.Buffer())) {
        return False;
    }

    return True;

}

bool FloatToStreamTest::TestExponential() {
    MyStream thisStream;
    float sbit32 = -11.234567;
    FormatDescriptor format;
    const char* pformat;

    //Left alignment padded	
    pformat = "- 15.8e";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-1.1234567E+1  ", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Left alignment, clip the number without rounding (the next is 4).
    pformat = "- 9.4e";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-1.123E+1", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Right Alignment with round up because the size (the next number is 6 because precision became 10(size)-3(exp)-1(point)=6)
    sbit32 = 112345.67; //112345,...
    pformat = " 10.8e";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);

    thisStream.PutC(0);
    if (!StringTestHelper::Compare("1.12346E+5", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Automatic precision=15 for 64 bit.
    double sbit64 = -12345.9999;
    pformat = " 21.e";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare(" -1.23459999000000E+4",
                                   thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Round up and zero added because the chosen precision.
    sbit64 = -12345.9999;
    pformat = " 13.8e";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-1.2346000E+4", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //In case of round up overflow.
    __float128 sbit128 = 999999.55556Q;
    pformat = ".6e"; //
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("1.00000E+6", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Clip the precision because the size.
    pformat = "- 5.6e";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("1E+6 ", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Too few space for number + exponent.
    pformat = "- 3.2e";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("?  ", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //The number initially does not fit but later yes thanks to the approximation.
    sbit128 = 9.5e-10Q;
    pformat = "- 4.2e";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("1E-9", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //The approximation is not sufficient now.
    sbit128 = 9.4999e-10Q;
    pformat = "- 4.2e";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("?   ", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Support exponent with 3 digits
    sbit128 = 9.9999e-101Q;
    pformat = "- 6.2e";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("1E-100", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Support exponent with 4 digits
    sbit128 = 9.9999e+999Q;
    pformat = "- 7.2e";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("1E+1000", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    return True;

}

bool FloatToStreamTest::TestEngeneering() {
    MyStream thisStream;
    FormatDescriptor format;
    const char* pformat;

    //Precision lost (with float this happens frequently)!
    float sbit32 = -11.234567;
    pformat = ".9E";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-11.2345671", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Left Align without correction (the next number is 4)	
    sbit32 *= 100;
    pformat = "- 10.4E";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-1.123E+3 ", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Right align padded with round up correction (the next number is 5)
    sbit32 = 112345.67;
    pformat = " 10.8E";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("112.346E+3", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Round up and zero added because the precision clip caused by the size (precision become 6).
    double sbit64 = 12345.9999;
    pformat = " 10.9E";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("12.3460E+3", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Without rounding up it can't print (999e-3) but with approximation it can.
    sbit64 = -0.9999;
    pformat = " 3.3E";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);

    thisStream.PutC(0);
    if (!StringTestHelper::Compare(" -1", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //If precision is too much we don't have approximation, then the size is not sufficient.
    sbit64 = -0.9999;
    pformat = " 3.5E";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare(" -1", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Round up and right aligned padd.
    __float128 sbit128 = 999999.99999Q;
    pformat = " 20.10E"; //
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("      1.000000000E+6",
                                   thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Automatic precision=34 for 128 bit number (clipped).
    sbit128 = 999999.9999900000000000000000000000Q;
    pformat = " 19E"; //
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("999.999999990000E+3",
                                   thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();
    //Not enough space for decimal numbers.
    sbit128 = -9.99999e9Q;
    pformat = "- 7.3E"; //
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-10E+9 ", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Not enough space to print the approximated number -100E+9. 
    sbit128 *= 10; //-99e9
    pformat = " 6.4E";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("     ?", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Also increasing the precision, it is clipped because the size then with the overflow the size is not sufficient.
    pformat = " 6.6E";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("     ?", thisStream.Buffer())) {
        return False;
    }

    //? the size is less than the minimum required
    sbit128 *= 10; //-1E+12 min_size=6
    thisStream.Clear();
    pformat = "5.2E";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("?", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //The number enter as 999 not approximated, and then approximation enter again in the size 
    sbit128 = 999.9Q;
    pformat = " 4.10E";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("1E+3", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //The number enter as 999 not approximated, but with approximation exceed the size then ?
    pformat = " 3.10E";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("  ?", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //The number does not enter as 999E+3 but after approximation enter in the size as 1E+6
    sbit128 = 999.9e3Q;
    pformat = " 4.10E";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("1E+6", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //The number initially does not fit, but it fit later thanks to approximation. 
    sbit128 = 950.0e-6Q;
    pformat = " 4.10E";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("1E-3", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //The approximation is not enough for the rounding.
    sbit128 = 949.9e-6Q;
    pformat = " 4.10E";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("   ?", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    return True;
}

bool FloatToStreamTest::TestSmart() {
    MyStream thisStream;
    float sbit32 = -11.234567;
    FormatDescriptor format;
    const char* pformat;

    pformat = ".9g";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);
    //Precision lost (with float this happens frequently)!
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-11.2345671", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Left Align without correction (the next is 4)	
    sbit32 *= 100; //1123.4567	
    pformat = "- 10.4g";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-1.123K   ", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Right align padded with round up correction (the next is 5)
    sbit32 = 112345.67; //112345,...
    pformat = " 8.6g";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("112.346K", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Round up and zero added because the chosen precision.
    double sbit64 = 12345.9999;
    pformat = " 10.6g";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("  12.3460K", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    sbit64 = -0.9999;
    pformat = " 3.3g";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare(" -1", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Round up and right aligned padd.
    __float128 sbit128 = 999999.99999Q;

    pformat = " 20.10g"; //

    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);

    thisStream.PutC(0);
    if (!StringTestHelper::Compare("        1.000000000M",
                                   thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Automatic size definition
    sbit128 = -9.99999e9Q;

    pformat = "- 7.3g"; //

    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-10.0G ", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Clip the precision because the size
    sbit128 *= 10; //-9.9e10
    pformat = "- 4.4g";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("?   ", thisStream.Buffer())) {
        return False;
    }

    //? the size is less than the minimum required
    sbit128 *= 10; //-999E+9 min_size=7
    thisStream.Clear();
    pformat = " 4.5g";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare(" -1T", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //The number enter as 999 not approximated, and then approximation enter again in the size 
    sbit128 = 999.9Q;
    pformat = " 4.10g";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("1.0K", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //The number does not enter as 999K but after approximation enter in the size as 1M
    sbit128 = 999.9e3Q;
    pformat = " 3.10g";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare(" 1M", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //print as engineering for exponents >24 and <-24
    sbit128 = 9e28Q;
    pformat = "- 7.10g";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("90E+27 ", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    sbit64 = -9.9e-28;
    pformat = " 9.10g";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare(" -990E-30", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    return True;
}

bool FloatToStreamTest::TestCompact() {
    MyStream thisStream;
    float sbit32 = -11.234567;
    FormatDescriptor format;
    const char* pformat;

    pformat = ".9G";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);
    //Precision lost (with float this happens frequently)!
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-11.2345671", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Left Align without correction (the next is 4)	
    sbit32 *= 100; //1123.4567	
    pformat = "- 10.4G";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-1.123K   ", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Right align padded with round up correction (the next is 5)
    sbit32 = 112345.67; //112345,...
    pformat = " 8.6G";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit32, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("112.346K", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Round up and zero added because the chosen precision.
    double sbit64 = 12345.9999;
    pformat = " 10.6G";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("  12.3460K", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    sbit64 = -0.9999;
    pformat = " 3.3G";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare(" -1", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Round up and right aligned padd.
    __float128 sbit128 = 999999.99999Q;

    pformat = " 20.10G"; //

    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);

    thisStream.PutC(0);
    if (!StringTestHelper::Compare("        1.000000000M",
                                   thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Automatic size definition
    sbit128 = -9.99999e9Q;

    pformat = "- 7.3G"; //

    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-10.0G ", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //Clip the precision because the size
    sbit128 *= 10; //-9.9e10
    pformat = "- 4.4G";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("?   ", thisStream.Buffer())) {
        return False;
    }

    //? the size is less than the minimum required
    sbit128 *= 10; //-999E+9 min_size=7
    thisStream.Clear();
    pformat = " 4.5G";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare(" -1T", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //The number enter as 999 not approximated, and then approximation enter again in the size 
    sbit128 = 999.9Q;
    pformat = " 4.10G";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("1.0K", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //The number does not enter as 999K but after approximation enter in the size as 1M
    sbit128 = 999.9e3Q;
    pformat = " 3.10G";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare(" 1M", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    //print as engineering for exponents >24 and <-24
    sbit128 = 9e28Q;
    pformat = "- 7.10G";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit128, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("9.0E+28", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    sbit64 = -9.9e-28;
    pformat = " 9.10G";
    format.InitialiseFromString(pformat);
    FloatToStream(*(thisStream.GetMyOutputBuffer()), sbit64, format);
    thisStream.PutC(0);
    if (!StringTestHelper::Compare("-9.90E-28", thisStream.Buffer())) {
        return False;
    }
    thisStream.Clear();

    return True;
}
