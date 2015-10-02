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
#include "IntegerToStreamTest.h"
#include "StringHelper.h"
#include "StreamTestHelper.h"
using namespace MARTe;

bool IntegerToStreamTest::TestDecimalMagnitude() {
    uint8 bit8 = 255;

    if (GetOrderOfMagnitude(bit8) != 2) {
        return false;
    }

    uint16 bit16 = 12345;

    if (GetOrderOfMagnitude(bit16) != 4) {
        return false;
    }

    uint32 bit32 = 123456789;
    if (GetOrderOfMagnitude(bit32) != 8) {
        return false;
    }

    uint64 bit64 = 12345678912345678;
    if (GetOrderOfMagnitude(bit64) != 16) {
        return false;
    }

    return true;
}

bool IntegerToStreamTest::TestHexadecimalMagnitude() {
    uint8 bit8 = 0x81;

    if (GetNumberOfDigitsHexNotation(bit8) != 2) {
        return false;
    }

    uint16 bit16 = 0xf123;

    if (GetNumberOfDigitsHexNotation(bit16) != 4) {
        return false;
    }

    uint32 bit32 = 0xaf010a31;
    if (GetNumberOfDigitsHexNotation(bit32) != 8) {
        return false;
    }

    uint64 bit64 = 0xedab12348976aef2;
    if (GetNumberOfDigitsHexNotation(bit64) != 16) {
        return false;
    }

    return true;
}

bool IntegerToStreamTest::TestOctalMagnitude() {
    uint8 bit8 = 0x81;

    if (GetNumberOfDigitsOctalNotation(bit8) != 3) {
        return false;
    }

    uint16 bit16 = 0xf123;

    if (GetNumberOfDigitsOctalNotation(bit16) != 6) {
        return false;
    }

    uint32 bit32 = 0xff010a31;
    if (GetNumberOfDigitsOctalNotation(bit32) != 11) {
        return false;
    }

    uint64 bit64 = 0xadab12348976aef2;
    if (GetNumberOfDigitsOctalNotation(bit64) != 22) {
        return false;
    }

    bit64 = 0x7dab12348976aef2;

    if (GetNumberOfDigitsOctalNotation(bit64) != 21) {
        return false;
    }
    return true;
}

bool IntegerToStreamTest::TestBinaryMagnitude() {
    uint8 bit8 = 0x81;

    if (GetNumberOfDigitsBinaryNotation(bit8) != 8) {
        return false;
    }

    bit8 = 0x71;

    if (GetNumberOfDigitsBinaryNotation(bit8) != 7) {
        return false;
    }

    uint16 bit16 = 0xf123;

    if (GetNumberOfDigitsBinaryNotation(bit16) != 16) {
        return false;
    }

    uint32 bit32 = 0xff010a31;
    if (GetNumberOfDigitsBinaryNotation(bit32) != 32) {
        return false;
    }

    uint64 bit64 = 0xedab12348976aef2;
    if (GetNumberOfDigitsBinaryNotation(bit64) != 64) {
        return false;
    }

    return true;
}

bool IntegerToStreamTest::TestDecimalStream() {

    MyStream myStream;
    uint8 ubit8 = 255;

    IntegerToStreamDecimalNotation(*(myStream.GetMyOutputBuffer()), ubit8);

    myStream.PutC(0);
    if (StringHelper::Compare("255", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();

    //LeftAligned and not padded (nothing happen)
    IntegerToStreamDecimalNotation(*(myStream.GetMyOutputBuffer()), ubit8, 5,
                                   false, true);

    myStream.PutC(0);
    if (StringHelper::Compare("255", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();
    int8 sbit8 = -127;
    IntegerToStreamDecimalNotation(*(myStream.GetMyOutputBuffer()), sbit8);

    myStream.PutC(0);
    if (StringHelper::Compare("-127", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();

    //LeftAligned and padded "...  "
    IntegerToStreamDecimalNotation(*(myStream.GetMyOutputBuffer()), sbit8, 6,
                                   true, true);
    myStream.PutC(0);
    if (StringHelper::Compare("-127  ", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();

    //Max int8
    sbit8 = 0x80;
    IntegerToStreamDecimalNotation(*(myStream.GetMyOutputBuffer()), sbit8);

    myStream.PutC(0);
    if (StringHelper::Compare("-128", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();

    uint16 ubit16 = 12345;
    IntegerToStreamDecimalNotation(*(myStream.GetMyOutputBuffer()), ubit16);

    myStream.PutC(0);
    if (StringHelper::Compare("12345", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();

    //LeftAligned and padded with positive sign "+...  "
    IntegerToStreamDecimalNotation(*(myStream.GetMyOutputBuffer()), ubit16, 8,
                                   true, true, true);

    myStream.PutC(0);
    if (StringHelper::Compare("+12345  ", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();

    int16 sbit16 = -12345;
    IntegerToStreamDecimalNotation(*(myStream.GetMyOutputBuffer()), sbit16);

    myStream.PutC(0);
    if (StringHelper::Compare("-12345", myStream.Buffer())!=0) {

        return false;
    }
    myStream.Clear();

    //Max int16
    sbit16 = 0x8000;
    IntegerToStreamDecimalNotation(*(myStream.GetMyOutputBuffer()), sbit16);

    myStream.PutC(0);
    if (StringHelper::Compare("-32768", myStream.Buffer())!=0) {

        return false;
    }

    myStream.Clear();

    uint32 ubit32 = 123456789;
    IntegerToStreamDecimalNotation(*(myStream.GetMyOutputBuffer()), ubit32);

    myStream.PutC(0);
    if (StringHelper::Compare("123456789", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();

    //RightAligned and padded with sign " +..."
    IntegerToStreamDecimalNotation(*(myStream.GetMyOutputBuffer()), ubit32, 11,
                                   true, false, true);
    myStream.PutC(0);
    if (StringHelper::Compare(" +123456789", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();

    int32 sbit32 = -123456789;

    IntegerToStreamDecimalNotation(*(myStream.GetMyOutputBuffer()), sbit32);

    myStream.PutC(0);
    if (StringHelper::Compare("-123456789", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();

    //Max int32
    sbit32 = 0x80000000;

    IntegerToStreamDecimalNotation(*(myStream.GetMyOutputBuffer()), sbit32);

    myStream.PutC(0);
    if (StringHelper::Compare("-2147483648", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();

    uint64 ubit64 = 12345678912345678;
    IntegerToStreamDecimalNotation(*(myStream.GetMyOutputBuffer()), ubit64);
    myStream.PutC(0);
    if (StringHelper::Compare("12345678912345678", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();

    int64 sbit64 = -12345678912345678;
    IntegerToStreamDecimalNotation(*(myStream.GetMyOutputBuffer()), sbit64);

    myStream.PutC(0);
    if (StringHelper::Compare("-12345678912345678", myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();

    //Max int64
    sbit64 = 0x8000000000000000;
    IntegerToStreamDecimalNotation(*(myStream.GetMyOutputBuffer()), sbit64);

    myStream.PutC(0);
    if (StringHelper::Compare("-9223372036854775808", myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();

    //? if the maxSize is incorrect	
    IntegerToStreamDecimalNotation(*(myStream.GetMyOutputBuffer()), sbit64, 11,
                                   true, false, true);

    myStream.PutC(0);
    if (StringHelper::Compare("          ?", myStream.Buffer())!=0) {
        return false;
    }

    return true;

}

bool IntegerToStreamTest::TestHexadecimalStream() {

    MyStream myStream;
    uint8 ubit8 = 0xea;

    IntegerToStreamExadecimalNotation(*(myStream.GetMyOutputBuffer()), ubit8);
    myStream.PutC(0);
    if (StringHelper::Compare("EA", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();

    //Check if works also for negative signed numbers.
    int8 sbit8 = 0xea;

    IntegerToStreamExadecimalNotation(*(myStream.GetMyOutputBuffer()), sbit8);
    myStream.PutC(0);
    if (StringHelper::Compare("EA", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();

    //LeftAligned and not padded (nothing happen)
    IntegerToStreamExadecimalNotation(*(myStream.GetMyOutputBuffer()), ubit8, 5,
                                      false, true);
    myStream.PutC(0);
    if (StringHelper::Compare("EA", myStream.Buffer())!=0) {
        return false;
    }

    //Add Header and trailing zeros
    myStream.Clear();
    IntegerToStreamExadecimalNotation(*(myStream.GetMyOutputBuffer()), ubit8, 0,
                                      false, false, true, true);

    myStream.PutC(0);
    if (StringHelper::Compare("0xEA", myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();

    ubit8 = 0xf;
    //Add only trailing zeros
    myStream.Clear();
    IntegerToStreamExadecimalNotation(*(myStream.GetMyOutputBuffer()), ubit8, 0,
                                      false, false, sizeof(ubit8) * 8, false);
    myStream.PutC(0);
    if (StringHelper::Compare("0F", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();

    //All true with a space more... it must print only a space after.
    uint16 ubit16 = 0xabcd;
    IntegerToStreamExadecimalNotation(*(myStream.GetMyOutputBuffer()), ubit16,
                                      7, true, true, sizeof(ubit16) * 8, true);

    myStream.PutC(0);
    if (StringHelper::Compare("0xABCD ", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();

    ubit16 = 0xcd;
    //With zeros and 3 as number of digits (without header).
    IntegerToStreamExadecimalNotation(*(myStream.GetMyOutputBuffer()), ubit16,
                                      5, true, true, sizeof(ubit16) * 8, true);
    myStream.PutC(0);
    if (StringHelper::Compare("0x0CD", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();

    //Only right aligned with header
    uint32 ubit32 = 0xabcdef78;
    IntegerToStreamExadecimalNotation(*(myStream.GetMyOutputBuffer()), ubit32,
                                      12, true, false, 0, true);
    myStream.PutC(0);
    if (StringHelper::Compare("  0xABCDEF78", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();

    ubit32 = 0x00abcd0f;
    //Right align with zero and header
    IntegerToStreamExadecimalNotation(*(myStream.GetMyOutputBuffer()), ubit32,
                                      11, true, false, sizeof(ubit32) * 8,
                                      true);
    myStream.PutC(0);
    if (StringHelper::Compare(" 0x00ABCD0F", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();

    //Right align without zeros and header
    IntegerToStreamExadecimalNotation(*(myStream.GetMyOutputBuffer()), ubit32,
                                      11, true, false, 0, false);
    myStream.PutC(0);
    if (StringHelper::Compare("     ABCD0F", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();

    //padded=false
    uint64 ubit64 = 0x89abcdef0123fff0;
    IntegerToStreamExadecimalNotation(*(myStream.GetMyOutputBuffer()), ubit64,
                                      120, false);
    myStream.PutC(0);
    if (StringHelper::Compare("89ABCDEF0123FFF0", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();
    //?
    ubit64 = 0x123fff0;
    IntegerToStreamExadecimalNotation(*(myStream.GetMyOutputBuffer()), ubit64,
                                      5, true);
    myStream.PutC(0);
    if (StringHelper::Compare("    ?", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();

    ubit64 = 0xfff0;
    IntegerToStreamExadecimalNotation(*(myStream.GetMyOutputBuffer()), ubit64,
                                      7, true, true, sizeof(ubit64) * 8);
    myStream.PutC(0);
    if (StringHelper::Compare("000FFF0", myStream.Buffer())!=0) {
        return false;
    }

    return true;

}

bool IntegerToStreamTest::TestOctalStream() {

    MyStream myStream;
    uint8 ubit8 = 0xea;

    IntegerToStreamOctalNotation(*(myStream.GetMyOutputBuffer()), ubit8);
    myStream.PutC(0);
    if (StringHelper::Compare("352", myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();

    //Test if works also for negative signed numbers.
    int8 sbit8 = -22;

    IntegerToStreamOctalNotation(*(myStream.GetMyOutputBuffer()), sbit8);
    myStream.PutC(0);
    if (StringHelper::Compare("352", myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();

    //LeftAligned and not padded (nothing happen)
    IntegerToStreamOctalNotation(*(myStream.GetMyOutputBuffer()), ubit8, 5,
                                 false, true);
    myStream.PutC(0);
    if (StringHelper::Compare("352", myStream.Buffer())!=0) {
        return false;
    }

    //Add Header and trailing zeros
    myStream.Clear();
    IntegerToStreamOctalNotation(*(myStream.GetMyOutputBuffer()), ubit8, 0,
                                 false, false, sizeof(ubit8) * 8, true);
    myStream.PutC(0);
    if (StringHelper::Compare("0o352", myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();

    ubit8 = 0xf;
    //Add only trailing zeros
    myStream.Clear();
    IntegerToStreamOctalNotation(*(myStream.GetMyOutputBuffer()), ubit8, 0,
                                 false, false, sizeof(ubit8) * 8, false);
    myStream.PutC(0);
    if (StringHelper::Compare("017", myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();

    //All true with a space more... it must print a space after.
    uint16 ubit16 = 0x7bcd; //6 is the maximum now are 5
    IntegerToStreamOctalNotation(*(myStream.GetMyOutputBuffer()), ubit16, 9,
                                 true, true, sizeof(ubit16) * 8, true);
    myStream.PutC(0);
    if (StringHelper::Compare("0o075715 ", myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();

    ubit16 = 0xcd;
    //With zeros and 5 as number of digits (without header).
    IntegerToStreamOctalNotation(*(myStream.GetMyOutputBuffer()), ubit16, 7,
                                 true, true, sizeof(ubit16) * 8, true);
    myStream.PutC(0);
    if (StringHelper::Compare("0o00315", myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();

    //Only right aligned with header
    uint32 ubit32 = 0xabcdef78;
    IntegerToStreamOctalNotation(*(myStream.GetMyOutputBuffer()), ubit32, 15,
                                 true, false, 0, true);
    myStream.PutC(0);
    if (StringHelper::Compare("  0o25363367570", myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();

    ubit32 = 0xcd0f;
    //Right align with zero and header
    IntegerToStreamOctalNotation(*(myStream.GetMyOutputBuffer()), ubit32, 15,
                                 true, false, sizeof(ubit32) * 8, true);
    myStream.PutC(0);
    if (StringHelper::Compare("  0o00000146417", myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();

    //Right align without zeros and header
    IntegerToStreamOctalNotation(*(myStream.GetMyOutputBuffer()), ubit32, 15,
                                 true, false, 0, false);
    myStream.PutC(0);
    if (StringHelper::Compare("         146417", myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();

    //padded=false
    uint64 ubit64 = 0x89abcdef01240000;
    IntegerToStreamOctalNotation(*(myStream.GetMyOutputBuffer()), ubit64, 120,
                                 false);
    myStream.PutC(0);
    if (StringHelper::Compare("1046536336740111000000",
                                   myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();
    //?
    ubit64 = 0x123fff0;
    IntegerToStreamOctalNotation(*(myStream.GetMyOutputBuffer()), ubit64, 5,
                                 true);
    myStream.PutC(0);
    if (StringHelper::Compare("    ?", myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();

    ubit64 = 0xfff0;
    IntegerToStreamOctalNotation(*(myStream.GetMyOutputBuffer()), ubit64, 7,
                                 true, true, sizeof(ubit64) * 8);
    myStream.PutC(0);
    if (StringHelper::Compare("0177760", myStream.Buffer())!=0) {
        return false;
    }

    return true;

}

bool IntegerToStreamTest::TestBinaryStream() {

    MyStream myStream;
    uint8 ubit8 = 0xea;

    IntegerToStreamBinaryNotation(*(myStream.GetMyOutputBuffer()), ubit8);
    myStream.PutC(0);
    if (StringHelper::Compare("11101010", myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();

    //Test if works also for negative signed numbers
    int8 sbit8 = 0xea;

    IntegerToStreamBinaryNotation(*(myStream.GetMyOutputBuffer()), sbit8);
    myStream.PutC(0);
    if (StringHelper::Compare("11101010", myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();
    //LeftAligned and not padded (nothing happen)
    IntegerToStreamBinaryNotation(*(myStream.GetMyOutputBuffer()), ubit8, 50,
                                  false, true);
    myStream.PutC(0);
    if (StringHelper::Compare("11101010", myStream.Buffer())!=0) {
        return false;
    }

    //Add trailing zeros
    myStream.Clear();
    IntegerToStreamBinaryNotation(*(myStream.GetMyOutputBuffer()), ubit8, 0,
                                  false, false, sizeof(ubit8) * 8, true);
    myStream.PutC(0);
    if (StringHelper::Compare("0b11101010", myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();

    ubit8 = 0xf;
    //Add only trailing zeros
    myStream.Clear();
    IntegerToStreamBinaryNotation(*(myStream.GetMyOutputBuffer()), ubit8, 0,
                                  false, false, sizeof(ubit8) * 8, false);
    myStream.PutC(0);
    if (StringHelper::Compare("00001111", myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();

    //All true with a space more... it must print a space after.
    uint16 ubit16 = 0x7bcd; //
    IntegerToStreamBinaryNotation(*(myStream.GetMyOutputBuffer()), ubit16, 19,
                                  true, true, sizeof(ubit16) * 8, true);
    myStream.PutC(0);
    if (StringHelper::Compare("0b0111101111001101 ", myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();

    ubit16 = 0xcd;
    //With zeros and 10 as number of digits (without header).
    IntegerToStreamBinaryNotation(*(myStream.GetMyOutputBuffer()), ubit16, 12,
                                  true, true, sizeof(ubit16) * 8, true);
    myStream.PutC(0);
    if (StringHelper::Compare("0b0011001101", myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();

    //Only right aligned with header
    uint32 ubit32 = 0xabcdef78;
    IntegerToStreamBinaryNotation(*(myStream.GetMyOutputBuffer()), ubit32, 34,
                                  true);
    myStream.PutC(0);
    if (StringHelper::Compare("  10101011110011011110111101111000",
                                   myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();

    ubit32 = 0xcd0f;
    //Right align with zero and header
    IntegerToStreamBinaryNotation(*(myStream.GetMyOutputBuffer()), ubit32, 34,
                                  true, false, sizeof(ubit32) * 8);
    myStream.PutC(0);
    if (StringHelper::Compare("  00000000000000001100110100001111",
                                   myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();

    //Right align without zeros and header
    IntegerToStreamBinaryNotation(*(myStream.GetMyOutputBuffer()), ubit32, 34,
                                  true, false, 0);
    myStream.PutC(0);
    if (StringHelper::Compare("                  1100110100001111",
                                   myStream.Buffer())!=0) {
        return false;
    }

    myStream.Clear();

    //padded=false
    uint64 ubit64 = 0x8000000000000001;
    IntegerToStreamBinaryNotation(*(myStream.GetMyOutputBuffer()), ubit64, 120,
                                  false);
    myStream.PutC(0);
    if (StringHelper::Compare(
            "1000000000000000000000000000000000000000000000000000000000000001",
            myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();
    //?
    ubit64 = 0x123fff0;
    IntegerToStreamBinaryNotation(*(myStream.GetMyOutputBuffer()), ubit64, 5,
                                  true);
    myStream.PutC(0);
    if (StringHelper::Compare("    ?", myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();

    ubit64 = 0xff0;
    IntegerToStreamBinaryNotation(*(myStream.GetMyOutputBuffer()), ubit64, 14,
                                  true, true, sizeof(ubit64) * 8);
    myStream.PutC(0);
    if (StringHelper::Compare("00111111110000", myStream.Buffer())!=0) {
        return false;
    }
    return true;

}

bool IntegerToStreamTest::TestIntegerToStream() {
    int8 sbit8 = 0xea;
    FormatDescriptor format;
    const char *pFormat;
    MyStream myStream;

    pFormat = "- i";
    format.InitialiseFromString(pFormat);
    IntegerToStream(*(myStream.GetMyOutputBuffer()), sbit8, format);
    myStream.PutC(0);
    if (StringHelper::Compare("-22", myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();

    int64 sbit64 = 0xf0;
    pFormat = " #0x";
    format.InitialiseFromString(pFormat);
    IntegerToStream(*(myStream.GetMyOutputBuffer()), sbit64, format);
    myStream.PutC(0);
    if (StringHelper::Compare("0x00000000000000F0", myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();
    int32 sbit32 = 0x18;

    pFormat = "- #0o";
    format.InitialiseFromString(pFormat);
    IntegerToStream(*(myStream.GetMyOutputBuffer()), sbit32, format);
    myStream.PutC(0);
    if (StringHelper::Compare("0o00000000030", myStream.Buffer())!=0) {
        return false;
    }
    myStream.Clear();

    int16 sbit16 = 0x71;
    pFormat = " #b";
    format.InitialiseFromString(pFormat);
    IntegerToStream(*(myStream.GetMyOutputBuffer()), sbit16, format);
    myStream.PutC(0);
    if (StringHelper::Compare("0b1110001", myStream.Buffer())!=0) {
        return false;
    }
    return true;
}

bool IntegerToStreamTest::TestBitSetToStream() {
    uint64 data[5] = { 0x13579BDF02468ACE, 0x13579BDF02468ACE,
            0x123456789ABCDEF0, 0xDEADBABEBAB00111 };
    const char streamString[] =
            "DEADBABEBAB00111123456789ABCDEF013579BDF02468ACE13579BDF02468ACE";
    int32 sizeStr = 63;
    int32 dataBitSize = 256;
    MyStream myStream;

    FormatDescriptor format;
    const char *pFormat;
    pFormat = "0x";
    format.InitialiseFromString(pFormat);
    uint32* source = (uint32*) data;

    //from size =1 to size = 64
    for (int size = 4; size < 64; size += 4) {
        int32 beg = 0;
        int32 end = 0;

        for (int myShift = 0; myShift < dataBitSize; myShift += size) {
            //source and shift are automatically modified by the function.
            BitSetToStream(*(myStream.GetMyOutputBuffer()), source, myShift,
                           size, false, format);
            char buffer[128];

            end = sizeStr - myShift / 4;
            beg = end - size / 4 + 1;
            StringHelper::Substr(beg, end, streamString, buffer);
//			printf("\n%s",buffer);

            myStream.PutC(0);
            if (StringHelper::Compare(buffer, myStream.Buffer())!=0) {
                return false;
            }
            myStream.Clear();

            //Avoids to print shit. (it remains less than size)
            if ((dataBitSize - myShift) < (2 * size))
                break;
        }
    }

    return true;
}
