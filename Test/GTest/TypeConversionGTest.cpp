/**
 * @file TypeConversionGTest.cpp
 * @brief Source file for class TypeConversionGTest
 * @date 12/11/2015
 * @author Giuseppe Ferrò
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
 * the class TypeConversionGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "TypeConversionTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(TypeConversionGTest,TestCCStringToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,int8> table[]={
            {"123",123, true},
            {"123s",123, true},
            {"a123s5",0, true},
            {"0xFF",-1, true},
            {"0xFF1",-1, true},
            {"0xABCD",0xab, true},
            {"0o12",10, true},
            {"0x7F",127, true},
            {"0x7F1",127, true},
            {"0x80",-128, true},
            {"0o377",-1, true},
            {"0o577",47, true},
            {"0b11111111",-1, true},
            {"0b111111111",-1, true},
            {"0b01111111",127, true},
            {"0b011111111",127, true},
            {"127",127, true},
            {"128",12, true},
            {"-128",-128, true},
            {"-129",-12, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestCCStringToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,uint8> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFF",255, true},
            {"0xFF1",255, true},
            {"0xABCD",0xab, true},
            {"0o12",10, true},
            {"0x7F",127, true},
            {"0x80",128, true},
            {"0o377",255, true},
            {"0o577",47, true},
            {"0b11111111",255, true},
            {"0b111111111",255, true},
            {"0b01111111",127, true},
            {"0b011111111",127, true},
            {"255",255, true},
            {"256",25, true},
            {"-1",0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestCCStringToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,int16> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFF",-1, true},
            {"0xFFFF1",-1, true},
            {"0x7FFF",32767, true},
            {"0o177777",-1, true},
            {"0o77777",32767, true},
            {"0o277777",12287, true},
            {"0b1111111111111111",-1, true},
            {"0b11111111111111111",-1, true},
            {"0b0111111111111111",32767, true},
            {"0b01111111111111111",32767, true},
            {"32767",32767, true},
            {"32768",3276, true},
            {"-32768",-32768, true},
            {"-32769",-3276, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestCCStringToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,uint16> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFF",65535, true},
            {"0xFFFF1",65535, true},
            {"0x7FFF",32767, true},
            {"0o177777",65535, true},
            {"0o77777",32767, true},
            {"0o277777",12287, true},
            {"0b1111111111111111",65535, true},
            {"0b11111111111111111",65535, true},
            {"32767",32767, true},
            {"65536",6553, true},
            {"-1",0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestCCStringToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,int32> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFFFFFF",-1, true},
            {"0xFFFFFFFF1",-1, true},
            {"0x7FFFFFFF",2147483647, true},
            {"0o37777777777",-1, true},
            {"0o17777777777",2147483647, true},
            {"0o47777777777",671088639, true},
            {"2147483647",2147483647, true},
            {"2147483648",214748364, true},
            {"0b11111111111111111111111111111111",-1, true},
            {"0b111111111111111111111111111111111",-1, true},
            {"0b01111111111111111111111111111111",2147483647, true},
            {"0b011111111111111111111111111111111",2147483647, true},
            {"-2147483648",-2147483648, true},
            {"-2147483649",-214748364, true},
            {0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestCCStringToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,uint32> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFFFFFF",4294967295, true},
            {"0xFFFFFFFF1",4294967295, true},
            {"0x7FFFFFFF",2147483647, true},
            {"0o37777777777",4294967295, true},
            {"0o17777777777",2147483647, true},
            {"0o47777777777",671088639, true},
            {"0b11111111111111111111111111111111",4294967295, true},
            {"0b111111111111111111111111111111111",4294967295, true},
            {"4294967295",4294967295, true},
            {"4294967296",429496729, true},
            {"-1",0, true},
            {0,0, 0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestCCStringToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,int64> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFFFFFFFFFFFFFF",-1, true},
            {"0xFFFFFFFFFFFFFFFF1",-1, true},
            {"0x7FFFFFFFFFFFFFFF",9223372036854775807, true},
            {"0o1777777777777777777777",-1, true},
            {"0o777777777777777777777",9223372036854775807, true},
            {"0o2777777777777777777777",3458764513820540927, true},
            {"0b1111111111111111111111111111111111111111111111111111111111111111",-1, true},
            {"0b11111111111111111111111111111111111111111111111111111111111111111",-1, true},
            {"0b0111111111111111111111111111111111111111111111111111111111111111",9223372036854775807, true},
            {"0b01111111111111111111111111111111111111111111111111111111111111111",9223372036854775807, true},
            {"9223372036854775807",9223372036854775807, true},
            {"9223372036854775808",922337203685477580, true},
            {"-9223372036854775808",-9223372036854775808, true},
            {"-9223372036854775809",-922337203685477580, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestCCStringToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,uint64> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFFFFFFFFFFFFFF",18446744073709551615, true},
            {"0xFFFFFFFFFFFFFFFF1",18446744073709551615, true},
            {"0x7FFFFFFFFFFFFFFF",9223372036854775807, true},
            {"0o1777777777777777777777",18446744073709551615, true},
            {"0o777777777777777777777",9223372036854775807, true},
            {"0o2777777777777777777777",3458764513820540927, true},
            {"0b1111111111111111111111111111111111111111111111111111111111111111",18446744073709551615, true},
            {"0b11111111111111111111111111111111111111111111111111111111111111111",18446744073709551615, true},
            {"0b0111111111111111111111111111111111111111111111111111111111111111",9223372036854775807, true},
            {"0b01111111111111111111111111111111111111111111111111111111111111111",9223372036854775807, true},
            {"18446744073709551615",18446744073709551615, true},
            {"18446744073709551616",1844674407370955161, true},
            {"-1",0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestCCStringToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,float32> table[]={
            {"123.5",123.5, true},
            {"123.12345",123.12345, true},
            {"1E+2",100, true},
            {"10E+2",1000, true},
            {"12345.678E2",1234567.8, true},
            {"1E-2",0.01, true},
            {"0.1E-2",0.001, true},
            {"16E-2",0.16, true},
            // see the precision is not the same!
            {"12345.678E-2",123.456772, true},
            {"1E+20",1E+20, true},
            // this is overflow!
            {"123E+42",123, true},
            {"1E-25",1E-25, true},
            {"0.000000001",1E-9, true},
            //Overflow!
     //       {"340282346638528859811704183484516999999",340282346638528859811704183484516925440},
     //       {"340282346638528859811704183484516925440",340282346638528859811704183484516925440},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestCCStringToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,float64> table[]={
            {"123.5",123.5, true},
            {"123.12345",123.12345, true},
            {"1E+2",100, true},
            {"10E+2",1000, true},
            {"12345.678E2",1234567.8, true},
            {"1E-2",0.01, true},
            {"0.1E-2",0.001, true},
            {"16E-2",0.16, true},
            // see the precision now is the same!
            {"12345.678E-2",123.45678, true},
            {"1E+20",1E+20, true},
            {"123E+63",123E+63, true},
            {"123E+83",123E+83, true},
            // this is overflow!
            {"123E+510",123, true},
            {"1E-32",1E-32, true},
            {"0.000000001",1E-9, true},
            //Overflow!
     //       {"179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368",179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368},
     //       {"179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368",179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestSStringToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<String,int8> table[]={
            {"123",123, true},
            {"123s",123, true},
            {"a123s5",0, true},
            {"0xFF",-1, true},
            {"0xFF1",-1, true},
            {"0xABCD",0xab, true},
            {"0o12",10, true},
            {"0x7F",127, true},
            {"0x7F1",127, true},
            {"0x80",-128, true},
            {"0o377",-1, true},
            {"0o577",47, true},
            {"0b11111111",-1, true},
            {"0b111111111",-1, true},
            {"0b01111111",127, true},
            {"0b011111111",127, true},
            {"127",127, true},
            {"128",12, true},
            {"-128",-128, true},
            {"-129",-12, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestSStringToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<String,uint8> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFF",255, true},
            {"0xFF1",255, true},
            {"0xABCD",0xab, true},
            {"0o12",10, true},
            {"0x7F",127, true},
            {"0x80",128, true},
            {"0o377",255, true},
            {"0o577",47, true},
            {"0b11111111",255, true},
            {"0b111111111",255, true},
            {"0b01111111",127, true},
            {"0b011111111",127, true},
            {"255",255, true},
            {"256",25, true},
            {"-1",0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestSStringToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<String,int16> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFF",-1, true},
            {"0xFFFF1",-1, true},
            {"0x7FFF",32767, true},
            {"0o177777",-1, true},
            {"0o77777",32767, true},
            {"0o277777",12287, true},
            {"0b1111111111111111",-1, true},
            {"0b11111111111111111",-1, true},
            {"0b0111111111111111",32767, true},
            {"0b01111111111111111",32767, true},
            {"32767",32767, true},
            {"32768",3276, true},
            {"-32768",-32768, true},
            {"-32769",-3276, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestSStringToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<String,uint16> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFF",65535, true},
            {"0xFFFF1",65535, true},
            {"0x7FFF",32767, true},
            {"0o177777",65535, true},
            {"0o77777",32767, true},
            {"0o277777",12287, true},
            {"0b1111111111111111",65535, true},
            {"0b11111111111111111",65535, true},
            {"32767",32767, true},
            {"65536",6553, true},
            {"-1",0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestSStringToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<String,int32> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFFFFFF",-1, true},
            {"0xFFFFFFFF1",-1, true},
            {"0x7FFFFFFF",2147483647, true},
            {"0o37777777777",-1, true},
            {"0o17777777777",2147483647, true},
            {"0o47777777777",671088639, true},
            {"2147483647",2147483647, true},
            {"2147483648",214748364, true},
            {"0b11111111111111111111111111111111",-1, true},
            {"0b111111111111111111111111111111111",-1, true},
            {"0b01111111111111111111111111111111",2147483647, true},
            {"0b011111111111111111111111111111111",2147483647, true},
            {"-2147483648",-2147483648, true},
            {"-2147483649",-214748364, true},
            {0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestSStringToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<String,uint32> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFFFFFF",4294967295, true},
            {"0xFFFFFFFF1",4294967295, true},
            {"0x7FFFFFFF",2147483647, true},
            {"0o37777777777",4294967295, true},
            {"0o17777777777",2147483647, true},
            {"0o47777777777",671088639, true},
            {"0b11111111111111111111111111111111",4294967295, true},
            {"0b111111111111111111111111111111111",4294967295, true},
            {"4294967295",4294967295, true},
            {"4294967296",429496729, true},
            {"-1",0, true},
            {0,0, 0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestSStringToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<String,int64> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFFFFFFFFFFFFFF",-1, true},
            {"0xFFFFFFFFFFFFFFFF1",-1, true},
            {"0x7FFFFFFFFFFFFFFF",9223372036854775807, true},
            {"0o1777777777777777777777",-1, true},
            {"0o777777777777777777777",9223372036854775807, true},
            {"0o2777777777777777777777",3458764513820540927, true},
            {"0b1111111111111111111111111111111111111111111111111111111111111111",-1, true},
            {"0b11111111111111111111111111111111111111111111111111111111111111111",-1, true},
            {"0b0111111111111111111111111111111111111111111111111111111111111111",9223372036854775807, true},
            {"0b01111111111111111111111111111111111111111111111111111111111111111",9223372036854775807, true},
            {"9223372036854775807",9223372036854775807, true},
            {"9223372036854775808",922337203685477580, true},
            {"-9223372036854775808",-9223372036854775808, true},
            {"-9223372036854775809",-922337203685477580, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestSStringToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<String,uint64> table[]={
            {"123",123, true},
            {"123s1",123, true},
            {"a123s5",0, true},
            {"0xFFFFFFFFFFFFFFFF",18446744073709551615, true},
            {"0xFFFFFFFFFFFFFFFF1",18446744073709551615, true},
            {"0x7FFFFFFFFFFFFFFF",9223372036854775807, true},
            {"0o1777777777777777777777",18446744073709551615, true},
            {"0o777777777777777777777",9223372036854775807, true},
            {"0o2777777777777777777777",3458764513820540927, true},
            {"0b1111111111111111111111111111111111111111111111111111111111111111",18446744073709551615, true},
            {"0b11111111111111111111111111111111111111111111111111111111111111111",18446744073709551615, true},
            {"0b0111111111111111111111111111111111111111111111111111111111111111",9223372036854775807, true},
            {"0b01111111111111111111111111111111111111111111111111111111111111111",9223372036854775807, true},
            {"18446744073709551615",18446744073709551615, true},
            {"18446744073709551616",1844674407370955161, true},
            {"-1",0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestSStringToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<String,float32> table[]={
            {"123.5",123.5, true},
            {"123.12345",123.12345, true},
            {"1E+2",100, true},
            {"10E+2",1000, true},
            {"12345.678E2",1234567.8, true},
            {"1E-2",0.01, true},
            {"0.1E-2",0.001, true},
            {"16E-2",0.16, true},
            // see the precision is not the same!
            {"12345.678E-2",123.456772, true},
            {"1E+20",1E+20, true},
            // this is overflow!
            {"123E+42",123, true},
            {"1E-25",1E-25, true},
            {"0.000000001",1E-9, true},
            //Overflow!
     //       {"340282346638528859811704183484516999999",340282346638528859811704183484516925440},
     //       {"340282346638528859811704183484516925440",340282346638528859811704183484516925440},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestSStringToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<String,float64> table[]={
            {"123.5",123.5, true},
            {"123.12345",123.12345, true},
            {"1E+2",100, true},
            {"10E+2",1000, true},
            {"12345.678E2",1234567.8, true},
            {"1E-2",0.01, true},
            {"0.1E-2",0.001, true},
            {"16E-2",0.16, true},
            // see the precision now is the same!
            {"12345.678E-2",123.45678, true},
            {"1E+20",1E+20, true},
            {"123E+63",123E+63, true},
            {"123E+83",123E+83, true},
            // this is overflow!
            {"123E+510",123, true},
            {"1E-32",1E-32, true},
            {"0.000000001",1E-9, true},
            //Overflow!
     //       {"179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368",179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368},
     //       {"179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368",179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

///////////////////////////////////////////////////////////////////////////////////////////////

TEST(TypeConversionGTest,TestInt8ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,int8> table[]={
            {1,1, true},
            {-1,-1,true},
            {127,127,true},
            {-128, -128, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(TypeConversionGTest,TestInt8ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,uint8> table[]={
            {1,1, true},
            {-1,0,true},
            {127,127,true},
            {-128, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt8ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,int16> table[]={
            {1,1, true},
            {-1,-1,true},
            {127,127,true},
            {-128, -128, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt8ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,uint16> table[]={
            {1,1, true},
            {-1,0,true},
            {127,127,true},
            {-128, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestInt8ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,int32> table[]={
            {1,1, true},
            {-1,-1,true},
            {127,127,true},
            {-128, -128, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt8ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,uint32> table[]={
            {1,1, true},
            {-1,0,true},
            {127,127,true},
            {-128, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt8ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,int64> table[]={
            {1,1, true},
            {-1,-1,true},
            {127,127,true},
            {-128, -128, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt8ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,uint64> table[]={
            {1,1, true},
            {-1,0,true},
            {127,127,true},
            {-128, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

//////////////////////////////////////////////////////////////////////////////////////////




TEST(TypeConversionGTest,TestUInt8ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,int8> table[]={
            {1,1, true},
            {128,127,true},
            {255,127,true},
            {127,127,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(TypeConversionGTest,TestUInt8ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,uint8> table[]={
            {1,1, true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt8ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,int16> table[]={
            {1,1, true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt8ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,uint16> table[]={
            {1,1, true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestUInt8ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,int32> table[]={
            {1,1, true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt8ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,uint32> table[]={
            {1,1, true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt8ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,int64> table[]={
            {1,1, true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt8ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,uint64> table[]={
            {1,1, true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

//////////////////////////////////////////////////////////////



TEST(TypeConversionGTest,TestInt16ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,int8> table[]={
            {1,1, true},
            {-1,-1,true},
            {127,127,true},
            {128, 127, true},
            {-128, -128, true},
            {-129, -128, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(TypeConversionGTest,TestInt16ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,uint8> table[]={
            {1,1, true},
            {-1,0,true},
            {255,255,true},
            {256, 255, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt16ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,int16> table[]={
            {1,1, true},
            {-1,-1,true},
            {32767,32767,true},
            {-32768, -32768, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt16ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,uint16> table[]={
            {1,1, true},
            {32767,32767,true},
            {-1, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestInt16ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,int32> table[]={
            {1,1, true},
            {-1,-1,true},
            {32767,32767,true},
            {-32768, -32768, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt16ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,uint32> table[]={
            {1,1, true},
            {32767,32767,true},
            {-1, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt16ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,int64> table[]={
            {1,1, true},
            {-1,-1,true},
            {32767,32767,true},
            {-32768, -32768, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt16ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,uint64> table[]={
            {1,1, true},
            {32767,32767,true},
            {-1, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

//////////////////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestUInt16ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,int8> table[]={
            {1,1, true},
            {127,127,true},
            {128,127,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(TypeConversionGTest,TestUInt16ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,uint8> table[]={
            {1,1, true},
            {256,255,true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt16ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,int16> table[]={
            {1,1, true},
            {32768,32767,true},
            {32767, 32767, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt16ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,uint16> table[]={
            {1,1, true},
            {65535,65535,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestUInt16ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,int32> table[]={
            {1,1, true},
            {65535,65535,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt16ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,uint32> table[]={
            {1,1, true},
            {65535,65535,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt16ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,int64> table[]={
            {1,1, true},
            {65535,65535,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt16ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,uint64> table[]={
            {1,1, true},
            {65535,65535,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(TypeConversionGTest,TestInt32ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,int8> table[]={
            {1,1, true},
            {-1,-1,true},
            {-128, -128, true},
            {127,127,true},
            {128,127,true},
            {-129,-128, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(TypeConversionGTest,TestInt32ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,uint8> table[]={
            {1,1, true},
            {-1,0,true},
            {255,255,true},
            {256, 255, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt32ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,int16> table[]={
            {1,1, true},
            {-1,-1,true},
            {32767,32767,true},
            {-32768, -32768, true},
            {32768,32767,true},
            {-32769, -32768, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt32ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,uint16> table[]={
            {1,1, true},
            {65535,65535,true},
            {65536,65535,true},
            {-1, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestInt32ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,int32> table[]={
            {1,1, true},
            {-1,-1,true},
            {2147483647,2147483647,true},
            {-2147483648, -2147483648, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt32ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,uint32> table[]={
            {1,1, true},
            {2147483647,2147483647,true},
            {-1, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt32ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,int64> table[]={
            {1,1, true},
            {-1,-1,true},
            {2147483647,2147483647,true},
            {-2147483648, -2147483648, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt32ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,uint64> table[]={
            {1,1, true},
            {2147483647,2147483647,true},
            {-1, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


///////////////////////////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestUInt32ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,int8> table[]={
            {1,1, true},
            {127,127,true},
            {128,127,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(TypeConversionGTest,TestUInt32ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,uint8> table[]={
            {1,1, true},
            {256,255,true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt32ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,int16> table[]={
            {1,1, true},
            {32768,32767,true},
            {32767, 32767, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt32ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,uint16> table[]={
            {1,1, true},
            {65535,65535,true},
            {65536,65535,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestUInt32ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,int32> table[]={
            {1,1, true},
            {2147483647,2147483647,true},
            {2147483648,2147483647,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt32ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,uint32> table[]={
            {1,1, true},
            {4294967295,4294967295,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt32ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,int64> table[]={
            {1,1, true},
            {4294967295,4294967295,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt32ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,uint64> table[]={
            {1,1, true},
            {4294967295,4294967295,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


////////////////////////////////////////////////////////////////////////////////////



TEST(TypeConversionGTest,TestInt64ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,int8> table[]={
            {1,1, true},
            {-1,-1,true},
            {-128, -128, true},
            {127,127,true},
            {128,127,true},
            {-129,-128, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(TypeConversionGTest,TestInt64ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,uint8> table[]={
            {1,1, true},
            {-1,0,true},
            {255,255,true},
            {256, 255, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt64ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,int16> table[]={
            {1,1, true},
            {-1,-1,true},
            {32767,32767,true},
            {-32768, -32768, true},
            {32768,32767,true},
            {-32769, -32768, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt64ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,uint16> table[]={
            {1,1, true},
            {65535,65535,true},
            {65536,65535,true},
            {-1, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestInt64ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,int32> table[]={
            {1,1, true},
            {-1,-1,true},
            {2147483647,2147483647,true},
            {-2147483648, -2147483648, true},
            {2147483648,2147483647,true},
            {-2147483649, -2147483648, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt64ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,uint32> table[]={
            {1,1, true},
            {4294967295,4294967295,true},
            {4294967296,4294967295,true},
            {-1, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt64ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,int64> table[]={
            {1,1, true},
            {-1,-1,true},
            {9223372036854775807,9223372036854775807,true},
            {-9223372036854775808, -9223372036854775808, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestInt64ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,uint64> table[]={
            {1,1, true},
            {9223372036854775807,9223372036854775807,true},
            {-1, 0, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


///////////////////////////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestUInt64ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,int8> table[]={
            {1,1, true},
            {127,127,true},
            {128,127,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(TypeConversionGTest,TestUInt64ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,uint8> table[]={
            {1,1, true},
            {256,255,true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt64ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,int16> table[]={
            {1,1, true},
            {32768,32767,true},
            {32767, 32767, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt64ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,uint16> table[]={
            {1,1, true},
            {65535,65535,true},
            {65536,65535,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(TypeConversionGTest,TestUInt64ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,int32> table[]={
            {1,1, true},
            {2147483647,2147483647,true},
            {2147483648,2147483647,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt64ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,uint32> table[]={
            {1,1, true},
            {4294967295,4294967295,true},
            {4294967296,4294967295,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt64ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,int64> table[]={
            {1,1, true},
            {9223372036854775807,9223372036854775807,true},
            {9223372036854775808,9223372036854775807,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(TypeConversionGTest,TestUInt64ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,uint64> table[]={
            {1,1, true},
            {18446744073709551615,18446744073709551615,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


///////////////////////////////////////////////////////////////////////////////////////////

TEST(TypeConversionGTest,TestInt8ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,float32> table[]={
            {1,1, true},
            {-1,-1,true},
            {-128,-128,true},
            {127,127,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt8ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,float64> table[]={
            {1,1, true},
            {-1,-1,true},
            {-128,-128,true},
            {127,127,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


///////////////////////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestUInt8ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,float32> table[]={
            {1,1, true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt8ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,float64> table[]={
            {1,1, true},
            {255,255,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////

TEST(TypeConversionGTest,TestInt16ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,float32> table[]={
            {1,1, true},
            {-1,-1,true},
            {32767,32767,true},
            {-32768,-32768,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt16ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,float64> table[]={
            {1,1, true},
            {-1,-1,true},
            {32767,32767,true},
            {-32768,-32768,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


///////////////////////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestUInt16ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,float32> table[]={
            {1,1, true},
            {65535,65535,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt16ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,float64> table[]={
            {1,1, true},
            {65535,65535,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////

TEST(TypeConversionGTest,TestInt32ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,float32> table[]={
            {1,1, true},
            {-1,-1,true},
            {2147483647,2147483647,true},
            {-2147483648,-2147483648,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt32ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,float64> table[]={
            {1,1, true},
            {-1,-1,true},
            {2147483647,2147483647,true},
            {-2147483648,-2147483648,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


///////////////////////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestUInt32ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,float32> table[]={
            {1,1, true},
            {4294967295,4294967295,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt32ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,float64> table[]={
            {1,1, true},
            {4294967295,4294967295,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestInt64ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,float32> table[]={
            {1,1, true},
            {-1,-1,true},
            {9223372036854775807,9223372036854775807,true},
            {-9223372036854775808,-9223372036854775808.0,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestInt64ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,float64> table[]={
            {1,1, true},
            {-1,-1,true},
            {9223372036854775807,9223372036854775807,true},
            {-9223372036854775808,-9223372036854775808.0,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


///////////////////////////////////////////////////////////////////////////////////


TEST(TypeConversionGTest,TestUInt64ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,float32> table[]={
            {1,1, true},
            {18446744073709551615,18446744073709551615,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(TypeConversionGTest,TestUInt64ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,float64> table[]={
            {1,1, true},
            {18446744073709551615,18446744073709551615,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////
