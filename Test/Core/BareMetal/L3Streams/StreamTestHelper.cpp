/**
 * @file StreamTestHelper.cpp
 * @brief Source file for class StreamTestHelper
 * @date 16/10/2015
 * @author Andre' Neto
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class StreamTestHelper (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "StreamTestHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

static inline uint8 *SaveNumber(uint8 number) {
    static uint8 numberx[numberOfIntegers];
    static int32 index = -1;

    for (int32 i = index; i >= 0; i--) {
        if (numberx[i] == number) {
            return &numberx[i];
        }
    }

    //the number is not already saved
    index++;
    numberx[index] = number;
    return &numberx[index];
}
static inline uint16 *SaveNumber(uint16 number) {
    static uint16 numberx[numberOfIntegers];
    static int32 index = -1;

    for (int32 i = index; i >= 0; i--) {
        if (numberx[i] == number) {
            return &numberx[i];
        }
    }

    //the number is not already saved
    index++;
    numberx[index] = number;
    return &numberx[index];
}
static inline uint32 *SaveNumber(uint32 number) {
    static uint32 numberx[numberOfIntegers];
    static int32 index = -1;

    for (int32 i = index; i >= 0; i--) {
        if (numberx[i] == number) {
            return &numberx[i];
        }
    }

    //the number is not already saved
    index++;
    numberx[index] = number;
    return &numberx[index];
}
static inline uint64 *SaveNumber(uint64 number) {
    static uint64 numberx[numberOfIntegers];
    static int32 index = -1;

    for (int32 i = index; i >= 0; i--) {
        if (numberx[i] == number) {
            return &numberx[i];
        }
    }

    //the number is not already saved
    index++;
    numberx[index] = number;
    return &numberx[index];
}
static inline int8 *SaveNumber(int8 number) {
    static int8 numberx[numberOfIntegers];
    static int32 index = -1;

    for (int32 i = index; i >= 0; i--) {
        if (numberx[i] == number) {
            return &numberx[i];
        }
    }

    //the number is not already saved
    index++;
    numberx[index] = number;
    return &numberx[index];
}
static inline int16 *SaveNumber(int16 number) {
    static int16 numberx[numberOfIntegers];
    static int32 index = -1;

    for (int32 i = index; i >= 0; i--) {
        if (numberx[i] == number) {
            return &numberx[i];
        }
    }

    //the number is not already saved
    index++;
    numberx[index] = number;
    return &numberx[index];
}
static inline int32 *SaveNumber(int32 number) {
    static int32 numberx[numberOfIntegers];
    static int32 index = -1;

    for (int32 i = index; i >= 0; i--) {
        if (numberx[i] == number) {
            return &numberx[i];
        }
    }

    //the number is not already saved
    index++;
    numberx[index] = number;
    return &numberx[index];
}
static inline int64 *SaveNumber(int64 number) {
    static int64 numberx[numberOfIntegers];
    static int32 index = -1;

    for (int32 i = index; i >= 0; i--) {
        if (numberx[i] == number) {
            return &numberx[i];
        }
    }

    //the number is not already saved
    index++;
    numberx[index] = number;
    return &numberx[index];
}

static inline float32 *SaveNumber(float32 number) {
    static float32 numberx[numberOfFloats];
    static int32 index = -1;

    for (int32 i = index; i >= 0; i--) {
        if (numberx[i] == number) {
            return &numberx[i];
        }
    }

    //the number is not already saved
    index++;
    numberx[index] = number;
    return &numberx[index];
}
static inline float64 *SaveNumber(float64 number) {
    static float64 numberx[numberOfFloats];
    static int32 index = -1;

    for (int32 i = index; i >= 0; i--) {
        if (numberx[i] == number) {
            return &numberx[i];
        }
    }

    //the number is not already saved
    index++;
    numberx[index] = number;
    return &numberx[index];
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
const PrintfNode *GeneratePrintFormattedDecimalTable(){
    static const PrintfNode printfDecTable[] = {
        //integers
        //decimal notations
        PrintfNode("%d", "255",     *SaveNumber((uint8) 255)),
        PrintfNode("%s", "255",     *SaveNumber((uint8) 255)),
        PrintfNode("%?", "Unsigned Integer",     *SaveNumber((uint8) 255)),
        PrintfNode("%5d", "255",    *SaveNumber((uint8) 255)),
        PrintfNode("% 5d", "  255", *SaveNumber((uint8) 255)),
        PrintfNode("%-5d", "255  ", *SaveNumber((uint8) 255)),
        PrintfNode("%d", "-127", *SaveNumber((int8) -127)),
        PrintfNode("%f", "-127", *SaveNumber((int8) -127)),
        PrintfNode("%-6i", "-127  ", *SaveNumber((int8) -127)),
        PrintfNode("%d", "-128", *SaveNumber((int8) 0x80)),
        PrintfNode("%u", "12345", *SaveNumber((uint16) 12345)),
        PrintfNode("%-#8d", "12345   ", *SaveNumber((uint16) 12345)),
        PrintfNode("%d", "-12345", *SaveNumber((int16) -12345)),
        PrintfNode("% 7?", "Signed ", *SaveNumber((int16) -12345)),
        PrintfNode("%i", "-32768", *SaveNumber((int16) 0x8000)),
        PrintfNode("%d", "123456789", *SaveNumber((uint32) 123456789)),
        PrintfNode("% #11u", "  123456789", *SaveNumber((uint32) 123456789)),
        PrintfNode("%i", "-123456789", *SaveNumber((int32) -123456789)),
        PrintfNode("%i", "-2147483648", *SaveNumber((int32) 0x80000000)),
        PrintfNode("%u", "12345678912345678", *SaveNumber((uint64) 12345678912345678)),
        PrintfNode("%-18?", "Signed Integer    ", *SaveNumber((int16) -12345)),
        PrintfNode("%i", "-12345678912345678", *SaveNumber((int64) -12345678912345678)),
        PrintfNode("%i", "-9223372036854775808", *SaveNumber((int64) 0x8000000000000000)),
        PrintfNode("% #11d", "          ?", *SaveNumber((int64) 0x8000000000000000)),
        PrintfNode("", "")
    };

    return printfDecTable;
}

const PrintfNode *GeneratePrintFormattedHexadecimalTable(){
    static const PrintfNode printfHexTable[] = {
        //hexadecimal notation
        PrintfNode("%x", "EA", *SaveNumber((uint8) 0xea)),
        PrintfNode("%x", "EA", *SaveNumber((int8) 0xea)),
        PrintfNode("%5x", "EA", *SaveNumber((int8) 0xea)),
        PrintfNode("%#0x", "0xEA", *SaveNumber((int8) 0xea)),
        PrintfNode("%0x", "0F", *SaveNumber((uint8) 0xf)),
        PrintfNode("%-#07x", "0xABCD ", *SaveNumber((uint16) 0xabcd)),
        PrintfNode("%#05x", "0x0CD", *SaveNumber((uint16) 0xcd)),
        PrintfNode("% #12x", "  0xABCDEF78", *SaveNumber((uint32) 0xabcdef78)),
        PrintfNode("% #011x", " 0x00ABCD0F", *SaveNumber((uint32) 0x00abcd0f)),
        PrintfNode("% 11x", "     ABCD0F", *SaveNumber((uint32) 0x00abcd0f)),
        PrintfNode("%120x", "89ABCDEF0123FFF0", *SaveNumber((uint64) 0x89abcdef0123fff0)),
        PrintfNode("% 5x", "    ?", *SaveNumber((uint64) 0x123fff0)),
        PrintfNode("%-07x", "000FFF0", *SaveNumber((int64) 0xfff0)),
        PrintfNode("", "")
    };
    return printfHexTable;
}

const PrintfNode *GeneratePrintFormattedOctalTable(){
    static const PrintfNode printfOctTable[] = {
        //hexadecimal notation
        PrintfNode("%o", "352", *SaveNumber((uint8) 0xea)),
        PrintfNode("%o", "352", *SaveNumber((int8) -22)),
        PrintfNode("%5o", "352", *SaveNumber((int8) -22)),
        PrintfNode("%#0o", "0o352", *SaveNumber((int8) -22)),
        PrintfNode("%0o", "017", *SaveNumber((uint8) 0xf)),
        PrintfNode("%-#09o", "0o075715 ",*SaveNumber((uint16) 0x7bcd)),
        PrintfNode("%#07o", "0o00315", *SaveNumber((uint16) 0xcd)),
        PrintfNode("% #15o", "  0o25363367570", *SaveNumber((uint32) 0xabcdef78)),
        PrintfNode("% #015o", "  0o00000146417", *SaveNumber((uint32) 0xcd0f)),
        PrintfNode("% 15o", "         146417", *SaveNumber((uint32) 0xcd0f)),
        PrintfNode("%120o", "1046536336740111000000", *SaveNumber((uint64) 0x89abcdef01240000)),
        PrintfNode("% 5o", "    ?", *SaveNumber((int64) 0x123fff0)),
        PrintfNode("%-07o", "0177760", *SaveNumber((int64) 0xfff0)),
        PrintfNode("", "")
    };
    return printfOctTable;
}

const PrintfNode *GeneratePrintFormattedBinaryTable(){
    static const PrintfNode printfBinTable[] = {
        PrintfNode("%b", "11101010", *SaveNumber((uint8) 0xea)),
        PrintfNode("%b", "11101010", *SaveNumber((int8) -22)),
        PrintfNode("%50b", "11101010", *SaveNumber((int8) -22)),
        PrintfNode("%#0b", "0b11101010", *SaveNumber((int8) -22)),
        PrintfNode("%0b", "00001111", *SaveNumber((uint8) 0xf)),
        PrintfNode("%-#019b", "0b0111101111001101 ", *SaveNumber((uint16) 0x7bcd)),
        PrintfNode("%#012b", "0b0011001101", *SaveNumber((int16) 0xcd)),
        PrintfNode("% #36b", "  0b10101011110011011110111101111000", *SaveNumber((uint32) 0xabcdef78)),
        PrintfNode("% 034b", "  00000000000000001100110100001111", *SaveNumber((uint32) 0xcd0f)),
        PrintfNode("% 34b", "                  1100110100001111", *SaveNumber((uint32) 0xcd0f)),
        PrintfNode("%120b", "1000000000000000000000000000000000000000000000000000000000000001", *SaveNumber((uint64) 0x8000000000000001)),
        PrintfNode("% 5b", "    ?", *SaveNumber((uint64) 0x123fff0)),
        PrintfNode("%-014b", "00111111110000", *SaveNumber((int64) 0xff0)),
        PrintfNode("", "")
    };
    return printfBinTable;
}


#define Windows 2
#define Linux 1

const PrintfNode *GeneratePrintFormattedFloatFPTable(){
    static const PrintfNode printfFloatFPTable[] = {
        PrintfNode("%f", "-1.123457", *SaveNumber(-1.1234567f)),
        PrintfNode("%s", "-1.123457", *SaveNumber(-1.1234567f)),
        PrintfNode("%- 12.7f", "-1.1234567  ", *SaveNumber(-1.1234567f)),
        PrintfNode("%- 12.3f", "-1.123      ", *SaveNumber(-1.1234567f)),
        PrintfNode("% 12.2f", "   112345.67", *SaveNumber(112345.67f)),
        PrintfNode("% 3.2f", "  0", *SaveNumber((float32) 0)),
        PrintfNode("% 1.2f", "0", *SaveNumber(0.09f)),
        PrintfNode("% 10.3f", " 12346.000", *SaveNumber(12345.9999)),
        PrintfNode("% 10.0f", "     12346", *SaveNumber(12345.9999)),
        PrintfNode("% 17.9f", " 999999.999960000", *SaveNumber(999999.99996)),
        PrintfNode("% 6.4f", "     ?", *SaveNumber(999999.99996)),
        PrintfNode("%-8.4f", "1000000 ", *SaveNumber(999999.99996)),
        PrintfNode("%-9.4f", "1000000.0", *SaveNumber(999999.99996)),
        PrintfNode("%7.4f", "-222223", *SaveNumber(-222222.5255)),
        PrintfNode("% 8.4f", " -222223", *SaveNumber(-222222.5255)),
        PrintfNode("% 9.4f", "-222222.5", *SaveNumber(-222222.5255)),
        PrintfNode("% 10.4f", "-222222.53", *SaveNumber(-222222.5255)),
        PrintfNode("% 5.4f", "    ?", *SaveNumber(-222222.5255)),
        PrintfNode("% 5.2f", " 0.01", *SaveNumber(0.009)),
        PrintfNode("% 12.4f", "           0", *SaveNumber(1e-12)),
        PrintfNode("% 1.10f", "0", *SaveNumber(-1e-12)),
#if ENVIRONMENT == Linux
        PrintfNode("%10.10f", "+Inf", *SaveNumber((float64) (1.0 / 0.0))),
        PrintfNode("% 10.10f", "      -Inf", *SaveNumber((float64) (-1.0 / 0.0))),
        PrintfNode("%-1.10f", "?", *SaveNumber((float32) (-1.0 / 0.0))),
        PrintfNode("%-10.10f", "NaN       ", *SaveNumber((float64) (0.0 / 0.0))),
        PrintfNode("% 0.4f", "NaN", *SaveNumber((float32) (0.0 / 0.0))),
        PrintfNode("% 2.10f", " ?", *SaveNumber((float64) (0.0 / 0.0))),
#endif
        PrintfNode("", "")
    };
    return printfFloatFPTable;
}



const PrintfNode *GeneratePrintFormattedFloatFPRTable(){
    static const PrintfNode printfFloatFPRTable[] = {
        PrintfNode("%-12.8F", "-1.1234567  ", *SaveNumber(-1.1234567f)),
        PrintfNode("%-12F", "-1.123457   ", *SaveNumber(-1.1234567f)),
        PrintfNode("%-12.0F", "0           ", *SaveNumber(-1.1234567f)),
        PrintfNode("%-12.4F", "-1.123      ", *SaveNumber(-1.1234567f)),
        PrintfNode("% 12.8F", "   112345.67", *SaveNumber(112345.67f)),
        PrintfNode("% 12.8?", "       Float", *SaveNumber(112345.67f)),
        PrintfNode("% 10.8F", " 12346.000", *SaveNumber(12345.9999)),
        PrintfNode("% 10.3F", "     12300", *SaveNumber(12345.9999)),
        PrintfNode("% 17.15F", " 999999.999960000", *SaveNumber(999999.99996)),
        PrintfNode("% 6.10F", "     ?", *SaveNumber(999999.99996)),
        PrintfNode("%-8.10F", "1000000 ", *SaveNumber(999999.99996)),
        PrintfNode("%-9.10F", "1000000.0", *SaveNumber(999999.99996)),
        PrintfNode("%7.10F", "-222223", *SaveNumber(-222222.5255)),
        PrintfNode("% 8.10F", " -222223", *SaveNumber(-222222.5255)),
        PrintfNode("% 9.10F", "-222222.5", *SaveNumber(-222222.5255)),
        PrintfNode("%-9?", "Float    ", *SaveNumber(-222222.5255)),
        PrintfNode("% 10.10F", "-222222.53", *SaveNumber(-222222.5255)),
        PrintfNode("% 5.10F", "    ?", *SaveNumber(-222222.5255)),
        PrintfNode("% 12.12F","           0", *SaveNumber(1e-12)),
#if ENVIRONMENT == Linux
        PrintfNode("%10.10F", "+Inf", *SaveNumber((float32) 1.0 / 0.0)),
        PrintfNode("% 10.10F", "      -Inf", *SaveNumber((float64) -1.0 / 0.0)),
        PrintfNode("%-1.10F", "?", *SaveNumber((float64) -1.0 / 0.0)),
        PrintfNode("%-10.10F", "NaN       ", *SaveNumber((float64) 0.0 / 0.0)),
        PrintfNode("% 0.10F", "NaN", *SaveNumber((float64) 0.0 / 0.0)),
        PrintfNode("% 2.10F", " ?", *SaveNumber((float64) -1.0 / 0.0)),
#endif
        PrintfNode("", "")
    };
    return printfFloatFPRTable;
}

const PrintfNode *GeneratePrintFormattedFloatExpTable(){
    static const PrintfNode printfFloatExpTable[] = {
        PrintfNode("%-15.8e", "-1.1234567E+1  ", *SaveNumber(-11.234567f)),
        PrintfNode("%-9.4e", "-1.123E+1", *SaveNumber(-11.234567f)),
        PrintfNode("% 10.8e", "1.12346E+5", *SaveNumber(112345.67f)),
        PrintfNode("% 21.e", " -1.23459999000000E+4", *SaveNumber(-12345.9999)),
        PrintfNode("% 13.8e", "-1.2346000E+4", *SaveNumber(-12345.9999)),
        PrintfNode("%.6e", "1.00000E+6", *SaveNumber(999999.55556)),
        PrintfNode("%-5.6e", "1E+6 ", *SaveNumber(999999.55556)),
        PrintfNode("%-3.2e", "?  ", *SaveNumber(999999.55556)),
        PrintfNode("%-4.2e", "1E-9", *SaveNumber(9.5e-10)),
        PrintfNode("%-4.2e", "?   ", *SaveNumber(9.4999e-10)),
        // quad float
        //
        //          PrintfNode("%-6.2e","1E-100",*SaveNumber(9.9999e-101)),
        //
        //          PrintfNode("%-7.2e","1E+1000",*SaveNumber(9.9999e+999)),
        PrintfNode("", "")
    };
    return printfFloatExpTable;
}

const PrintfNode *GeneratePrintFormattedFloatEngTable(){
    static const PrintfNode printfFloatEngTable[] = {
        PrintfNode("%.9E", "-11.2345671", *SaveNumber(-11.234567f)),
        PrintfNode("%-10.4E", "-1.123E+3 ", *SaveNumber(-1123.4567f)),
        PrintfNode("% 10.8E", "112.346E+3", *SaveNumber(112345.67f)),
        PrintfNode("% 10.9E", "12.3460E+3", *SaveNumber(12345.9999)),
        PrintfNode("% 3.3E", " -1", *SaveNumber(-0.9999)),
        PrintfNode("% 3.5E", " -1", *SaveNumber(-0.9999)),
        PrintfNode("% 20.10E", "      1.000000000E+6", *SaveNumber(999999.99999)),
        PrintfNode("% 19E", "999.999999990000E+3", *SaveNumber(999999.9999900000000000000000000000)),
        PrintfNode("%-7.3E", "-10E+9 ", *SaveNumber(-9.99999e9)),
        PrintfNode("% 6.4E", "     ?", *SaveNumber(-99.9999e9)),
        PrintfNode("% 6.6E", "     ?", *SaveNumber(-99.9999e9)),
        PrintfNode("%5.2E", "?", *SaveNumber(-999.999e9)),
        PrintfNode("% 4.10E", "1E+3", *SaveNumber(999.9)),
        PrintfNode("% 3.10E", "  ?", *SaveNumber(999.9)),
        PrintfNode("% 4.10E", "1E+6", *SaveNumber(999.9e3)),
        PrintfNode("% 4.10E", "1E-3", *SaveNumber(950.0e-6)),
        PrintfNode("% 4.10E", "   ?", *SaveNumber(949.9e-6)),
        PrintfNode("", "")
    };
    return printfFloatEngTable;
}

const PrintfNode *GeneratePrintFormattedFloatSmartTable(){
    static const PrintfNode printfFloatSmartTable[] = {
        PrintfNode("%.9g", "-11.2345671", *SaveNumber(-11.234567f)),
        PrintfNode("%- 10.4g", "-1.123K   ", *SaveNumber(-1123.4567f)),
        PrintfNode("% 8.6g", "112.346K", *SaveNumber(112345.67f)),
        PrintfNode("% 10.6g", "  12.3460K", *SaveNumber(12345.9999)),
        PrintfNode("% 3.3g", " -1", *SaveNumber(-0.9999)),
        PrintfNode("% 20.10g", "        1.000000000M", *SaveNumber(999999.99999)),
        PrintfNode("%-7.3g", "-10.0G ", *SaveNumber(-9.99999e9)),
        PrintfNode("%-4.4g", "?   ", *SaveNumber(-99.9999e9)),
        PrintfNode("% 4.5g", " -1T", *SaveNumber(-999.999e9)),
        PrintfNode("% 4.10g", "1.0K", *SaveNumber(999.9)),
        PrintfNode("% 3.10g", " 1M", *SaveNumber(999.9e3)),
        PrintfNode("%-7.10g", "90E+27 ", *SaveNumber(9e28)),
        PrintfNode("% 9.10g", " -990E-30", *SaveNumber(-9.9e-28)),
        PrintfNode("", "")
    };
    return printfFloatSmartTable;
}

const PrintfNode *GeneratePrintFormattedFloatCompactTable(){
    static const PrintfNode printfFloatCompactTable[] = {
        PrintfNode("%.9G", "-11.2345671", *SaveNumber(-11.234567f)),
        PrintfNode("%-10.4G", "-1.123K   ", *SaveNumber(-1123.4567f)),
        PrintfNode("% 8.6G", "112.346K", *SaveNumber(112345.67f)),
        PrintfNode("% 10.6G", "  12.3460K", *SaveNumber(12345.9999)),
        PrintfNode("% 3.3G", " -1", *SaveNumber(-0.9999)),
        PrintfNode("% 20.10G", "        1.000000000M", *SaveNumber(999999.99999)),
        PrintfNode("%-7.3G", "-10.0G ", *SaveNumber(-9.99999e9)),
        PrintfNode("%-4.4G", "?   ", *SaveNumber(-99.9999e9)),
        PrintfNode("% 4.54G", " -1T", *SaveNumber(-999.999e9)),
        PrintfNode("% 4.10G", "1.0K", *SaveNumber(999.9)),
        PrintfNode("% 3.10G", " 1M", *SaveNumber(999.9e3)),
        PrintfNode("%-7.10G", "9.0E+28", *SaveNumber(9e28)),
        PrintfNode("% 9.10G", "-9.90E-28", *SaveNumber(-9.9e-28)),
        PrintfNode("", "")
    };
    return printfFloatCompactTable;
}

const PrintfNode *GeneratePrintFormattedGenericTable(){
    static const PrintfNode printfGenericTable[] = {
        PrintfNode("%.9G %5x%u :) %s ", "-11.2345671 EA12345 :) Hello! ", *SaveNumber(-11.234567f), *SaveNumber((int8) 0xea), *SaveNumber((uint16) 12345), "Hello!"),
        PrintfNode("%.9G %5x%u :) %s Very very very long long long long striiiiiiiiiiiiiinnnnnggggggg!", "-11.2345671 EA12345 :) Hello! Very very very long long long long striiiiiiiiiiiiiinnnnnggggggg!", *SaveNumber(-11.234567f), *SaveNumber((int8) 0xea), *SaveNumber((uint16) 12345), "Hello!"),
        PrintfNode("", "")
    };
    return printfGenericTable;
}

const char8* printfCStringTable[][3] = {
            { "string:%s", "HelloWorld", "string:HelloWorld" },
            { "string:%d", "HelloWorld", "string:HelloWorld" },
            { "string:%5s", "HelloWorld", "string:Hello" },
            { "string:% 11s", "HelloWorld", "string: HelloWorld" },
            { "string:%-11s", "HelloWorld", "string:HelloWorld " },
            { "string:%-13?", "HelloWorld", "string:Char String  " },
            { "string:% 13?", "HelloWorld", "string:  Char String" },
            { "string:% #5s", "HelloWorld", "string:\"Hel\"" },
            { "string:%#-11s", "Stream", "string:\"Stream\"   " },
            { "string:% #11s", "Stream", "string:   \"Stream\"" },
            { "% 5s", "  ", "     " },
            { "%-5s", "  ", "     " },
            { "%s", (const char8*) NULL, "" },
            { NULL, NULL, NULL }
    };

const char8* printfStreamTable[][3] = {
            { "string:%s", "HelloWorld", "string:HelloWorld" },
            { "string:%d", "HelloWorld", "string:HelloWorld" },
            { "string:%5s", "HelloWorld", "string:Hello" },
            { "string:% 11s", "HelloWorld", "string: HelloWorld" },
            { "string:%-11s", "HelloWorld", "string:HelloWorld " },
            { "string:%-11?", "HelloWorld", "string:Stream     " },
            { "string:% 11?", "HelloWorld", "string:     Stream" },
            { "string:% #11?", "HelloWorld", "string:   \"Stream\"" },
            { "string:%#s", "HelloWorld", "string:\"HelloWorld\"" },
            { "string:%#-11?", "HelloWorld", "string:\"Stream\"   " },
            { "string:% #5s", "HelloWorld", "string:\"Hel\"" },
            { "% 5s", "  ", "     " },
            { "%-5s", "  ", "     " },
            { "%s", (const char8*) NULL, "" },
            { NULL, NULL, NULL }
    };



const BitSetToBitSetTableRow *GeneratedBitSetToBitSetTable(){
    static const uint32 source[] = { 0xffffffb5, 0x00001111, 0x22223333, 0x0 };
    static const BitSetToBitSetTableRow bitSetToBitSetTable[]={
            {0, 0, 32, 32, (uint32*)source, 0, 0xffffffb5, true, true},
            {32, 0, 32, 32, (uint32*)source, 1, 0xffffffb5, true, true},
            {64, 16, 32, 32, (uint32*)source, 2, 0x1111ffff, true, true},
            //Take only 16 bit size. Sign padding should work.
            {64, 16, 32, 16, (uint32*)source, 2, 0xffffffff, true, true},
            //Take only 3 bit size with shift=1. No sign padding
            {64, 1, 32, 3, (uint32*)source, 2, 0x2, true, true},
            //destSize < sourceSize, saturation.
            {96, 0, 5, 16, (uint32*)source, 3, 0x10, true, true},
            //saturation for unsigned source and signed destination.
            {104, 16, 5, 16, (uint32*)source, 3, 0xf10, false, true},
            //saturation to zero for negative source and unsigned destination.
            {104, 16, 5, 16, (uint32*)source, 3, 0x10, true, false},
            //8 bit pointer
            {104, 64, 8, 3, (uint32*)source, 3, 0x310, false, false},
            //16 bit pointer. Not padding because source is unsigned.
            {96, 80, 16, 10, (uint32*)source, 3, 0x222, false, false},
            //16 bit pointer to 32. Sign padding.
            {104, 77, 16, 5, (uint32*)source, 3, 0xfff122, true, true},
            //64 bit pointer to 128. Padding
            {32, 48, 32, 30, (uint32*) source, 1, 0xf3330000, true, true},
            {0, 0, 0, 0, (uint32*)NULL, 0, 0, true, true},

    };

    return bitSetToBitSetTable;
}







