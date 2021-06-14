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


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "TypeConversionTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

#define Linux 1
#define Windows 2
#define FreeRTOS 3

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,int8> table[]={
            {"+123",123, true,true},
            {"123s",123, true,false},
            {"a123s5",0, true,false},
            {"0xFF",-1, true,true},
            {"0xFF1",-1, true,false},
            {"0xABCD",(int8)0xab, true,false},
            {"0xFG",15, true,false},
            {"0o12",10, true,true},
            {"0x7F",127, true,true},
            {"0x7F1",127, true,false},
            {"0x80",-128, true,true},
            {"0o377",-1, true,true},
            {"0o577",47, true,false},
            {"0o3777",-1, true,false},
            {"0o38",3, true,false},
            {"0b11111111",-1, true,true},
            {"0b111111111",-1, true,false},
            {"0b01111111",127, true,true},
            {"0b011111111",127, true,false},
            {"0b01121111",3, true,false},
            {"127",127, true,true},
            {"128",12, true,false},
            {"-128",-128, true,true},
            {"-129",-12, true,false},
            {"x",0,true, false},
            {0,0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,uint8> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFF",255, true, true},
            {"0xFF1",255, true, false},
            {"0xABcD",0xab, true, false},
            {"0o12",10, true, true},
            {"0x7F",127, true, true},
            {"0x80",128, true, true},
            {"0o377",255, true, true},
            {"0o577",47, true, false},
            {"0b11111111",255, true, true},
            {"0b111111111",255, true, false},
            {"0b01111111",127, true, true},
            {"0b011111111",127, true, false},
            {"255",255, true, true},
            {"256",25, true, false},
            {"-1",0, true, false},
            {0,0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,int16> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFF",-1, true, true},
            {"0xFFFF1",-1, true, false},
            {"0x7FFF",32767, true, true},
            {"0o177777",-1, true, true},
            {"0o77777",32767, true, true},
            {"0o277777",12287, true, false},
            {"0b1111111111111111",-1, true, true},
            {"0b11111111111111111",-1, true, false},
            {"0b0111111111111111",32767, true, true},
            {"0b01111111111111111",32767, true, false},
            {"32767",32767, true, true},
            {"32768",3276, true, false},
            {"-32768",-32768, true, true},
            {"-32769",-3276, true, false},
            {0,0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,uint16> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFF",65535, true, true},
            {"0xFFFF1",65535, true, false},
            {"0x7FFF",32767, true, true},
            {"0o177777",65535, true, true},
            {"0o77777",32767, true, true},
            {"0o277777",12287, true, false},
            {"0b1111111111111111",65535, true, true},
            {"0b11111111111111111",65535, true, false},
            {"32767",32767, true, true},
            {"65536",6553, true, false},
            {"-1",0, true, false},
            {0,0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,int32> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFFFFFF",-1, true, true},
            {"0xFFFFFFFF1",-1, true, false},
            {"0x7FFFFFFF",2147483647, true, true},
            {"0o37777777777",-1, true, true},
            {"0o17777777777",2147483647, true, true},
            {"0o47777777777",671088639, true, false},
            {"2147483647",2147483647, true, true},
            {"2147483648",214748364, true, false},
            {"0b11111111111111111111111111111111",-1, true, true},
            {"0b111111111111111111111111111111111",-1, true, false},
            {"0b01111111111111111111111111111111",2147483647, true, true},
            {"0b011111111111111111111111111111111",2147483647, true, false},
            {"-2147483648",-2147483648, true, true},
            {"-2147483649",-214748364, true, false},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,uint32> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFFFFFF",4294967295, true, true},
            {"0xFFFFFFFF1",4294967295, true, false},
            {"0x7FFFFFFF",2147483647, true, true},
            {"0o37777777777",4294967295, true, true},
            {"0o17777777777",2147483647, true, true},
            {"0o47777777777",671088639, true, false},
            {"0b11111111111111111111111111111111",4294967295, true, true},
            {"0b111111111111111111111111111111111",4294967295, true, false},
            {"4294967295",4294967295, true, true},
            {"4294967296",429496729, true, false},
            {"10000000000",1000000000, true, false},
            {"-1",0, true, false},
            {0,0, 0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,int64> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFFFFFFFFFFFFFF",-1, true, true},
            {"0xFFFFFFFFFFFFFFFF1",-1, true, false},
            {"0x7FFFFFFFFFFFFFFF",9223372036854775807ul, true, true},
            {"0o1777777777777777777777",-1, true, true},
            {"0o777777777777777777777",9223372036854775807ul, true, true},
            {"0o2777777777777777777777",3458764513820540927ul, true, false},
            {"0b1111111111111111111111111111111111111111111111111111111111111111",-1, true, true},
            {"0b11111111111111111111111111111111111111111111111111111111111111111",-1, true, false},
            {"0b0111111111111111111111111111111111111111111111111111111111111111",9223372036854775807ul, true, true},
            {"0b01111111111111111111111111111111111111111111111111111111111111111",9223372036854775807ul, true, false},
            {"9223372036854775807",9223372036854775807ul, true, true},
            {"9223372036854775808",922337203685477580ul, true, false},
            {"-9223372036854775808",(int64)-9223372036854775808ul, true, true},
            {"-9223372036854775809",(int64)-922337203685477580ul, true, false},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,uint64> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFFFFFFFFFFFFFF",18446744073709551615ul, true, true},
            {"0xFFFFFFFFFFFFFFFF1",18446744073709551615ul, true, false},
            {"0x7FFFFFFFFFFFFFFF",9223372036854775807ul, true, true},
            {"0o1777777777777777777777",18446744073709551615ul, true, true},
            {"0o777777777777777777777",9223372036854775807ul, true, true},
            {"0o2777777777777777777777",3458764513820540927ul, true, false},
            {"0b1111111111111111111111111111111111111111111111111111111111111111",18446744073709551615ul, true, true},
            {"0b11111111111111111111111111111111111111111111111111111111111111111",18446744073709551615ul, true, false},
            {"0b0111111111111111111111111111111111111111111111111111111111111111",9223372036854775807ul, true, true},
            {"0b01111111111111111111111111111111111111111111111111111111111111111",9223372036854775807ul, true, false},
            {"18446744073709551615",18446744073709551615ul, true, true},
            {"18446744073709551616",1844674407370955161ul, true, false},
            {"-1",0, true, false},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,float32> table[]={
            {"-123.5",-123.5, true, true},
            {"123.12345",123.12345, true, true},
            {"1E+2",100, true,true},
            {"10E+2",1000, true,true},
            {"12345.678E2",1234567.8, true,true},
            {"-1E-2",-0.01, true,true},
            {"0.1E-2",0.001, true,true},
            {"16E-2",0.16, true,true},
            // see the precision is not the same!
            {"12345.678E-2",123.456772, true,true},
            {"1E+20",1E+20, true,true},
            // this is overflow!
            {"123E+42",123, true,false},
            {"1E-25",1E-25, true,true},
            {"0.000000001",1E-9, true,true},
            // invalid token
            {"123a", 123.0, true, false},
            {"+123.5o",123.5, true, false},
            {"-123E-x",-123.0, true, false},
            //Overflow!
            {"340282346638528859811704183484516999999",340282346638528859811704183484516925440.0, true, true},
            {"340282346638528859811704183484516925440",340282346638528859811704183484516925440.0, true, true},
            {"440282346638528859811704183484516925440",44028234663852885981170418348451692544.0, true, false},
            {"0.000000000000000000000000000000000000000000001",0.0, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,float64> table[]={
            {"123.5",123.5, true, true},
            {"123.12345",123.12345, true, true},
            {"1E+2",100, true, true},
            {"10E+2",1000, true, true},
            {"+12345.678E2",1234567.8, true, true},
            {"1E-2",0.01, true, true},
            {"0.1E-2",0.001, true, true},
            {"16E-2",0.16, true, true},
            // see the precision now is the same!
            {"12345.678E-2",123.45678, true, true},
            {"1E+20",1E+20, true, true},
            {"123E+63",123E+63, true, true},
            {"123E+83",123E+83, true, true},
            // this is overflow!
            {"123E+510",123, true, false},
            {"1E-32",1E-32, true, true},
            {"0.000000001",1E-9, true, true},
            // invalid token
            {"123a", 123.0, true, false},
            {"+123.5o",123.5, true, false},
            {"-123E-x",-123.0, true, false},
            //Overflow!
#if ENVIRONMENT != Windows
            {"17976931348623157081452742373170435679807056752584499659891747680315726078002853876058955863276687817154045895351438246423432132688946418276846754670353751698604991057655128207624549009038932894407586850845513394230458323690322294816580855933212334827479782620414472316873817718091929988125040402618412485836",179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0},
            {"17976931348623157081452742373170435679807056752584499659891747680315726078002853876058955863276687817154045895351438246423432132688946418276846754670353751698604991057655128207624549009038932894407586850845513394230458323690322294816580855933212334827479782620414472316873817718091929988125040402618412485836",179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0},
            {"27976931348623157081452742373170435679807056752584499659891747680315726078002853876058955863276687817154045895351438246423432132688946418276846754670353751698604991057655128207624549009038932894407586850845513394230458323690322294816580855933212334827479782620414472316873817718091929988125040402618412485836",179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0},
#endif
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,StreamString> table[]={
            {"Hello","Hello", true, true},
            {"123.12345","123.12345", true, true},
            {"","", true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<StreamString,StreamString> table[]={
            {"Hello","Hello", true, true},
            {"123.12345","123.12345", true, true},
            {"","", true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToCCString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<StreamString,const char8*> table[]={
            {"Hello","Hello", true, true},
            {"123.12345","123.12345", true, true},
            {"","", true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCCString(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToCCString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8* ,const char8*> table[]={
            {"Hello","Hello", true, true},
            {"123.12345","123.12345", true, true},
            {"","", true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCCString(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<StreamString,int8> table[]={
            {"123",123, true,true},
            {"123s",123, true,false},
            {"a123s5",0, true,false},
            {"0xFF",-1, true,true},
            {"0xFF1",-1, true,false},
            {"0xABCD",(int8)0xab, true,false},
            {"0o12",10, true,true},
            {"0x7F",127, true,true},
            {"0x7F1",127, true,false},
            {"0x80",-128, true,true},
            {"0o377",-1, true,true},
            {"0o577",47, true,false},
            {"0b11111111",-1, true,true},
            {"0b111111111",-1, true,false},
            {"0b01111111",127, true,true},
            {"0b011111111",127, true,false},
            {"127",127, true,true},
            {"128",12, true,false},
            {"-128",-128, true,true},
            {"-129",-12, true,false},
            {0,0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<StreamString,uint8> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFF",255, true, true},
            {"0xFF1",255, true, false},
            {"0xABCD",0xab, true, false},
            {"0o12",10, true, true},
            {"0x7F",127, true, true},
            {"0x80",128, true, true},
            {"0o377",255, true, true},
            {"0o577",47, true, false},
            {"0b11111111",255, true, true},
            {"0b111111111",255, true, false},
            {"0b01111111",127, true, true},
            {"0b011111111",127, true, false},
            {"255",255, true, true},
            {"256",25, true, false},
            {"-1",0, true, false},
            {0,0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<StreamString,int16> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFF",-1, true, true},
            {"0xFFFF1",-1, true, false},
            {"0x7FFF",32767, true, true},
            {"0o177777",-1, true, true},
            {"0o77777",32767, true, true},
            {"0o277777",12287, true, false},
            {"0b1111111111111111",-1, true, true},
            {"0b11111111111111111",-1, true, false},
            {"0b0111111111111111",32767, true, true},
            {"0b01111111111111111",32767, true, false},
            {"32767",32767, true, true},
            {"32768",3276, true, false},
            {"-32768",-32768, true, true},
            {"-32769",-3276, true, false},
            {0,0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<StreamString,uint16> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFF",65535, true, true},
            {"0xFFFF1",65535, true, false},
            {"0x7FFF",32767, true, true},
            {"0o177777",65535, true, true},
            {"0o77777",32767, true, true},
            {"0o277777",12287, true, false},
            {"0b1111111111111111",65535, true, true},
            {"0b11111111111111111",65535, true, false},
            {"32767",32767, true, true},
            {"65536",6553, true, false},
            {"-1",0, true, false},
            {0,0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<StreamString,int32> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFFFFFF",-1, true, true},
            {"0xFFFFFFFF1",-1, true, false},
            {"0x7FFFFFFF",2147483647, true, true},
            {"0o37777777777",-1, true, true},
            {"0o17777777777",2147483647, true, true},
            {"0o47777777777",671088639, true, false},
            {"2147483647",2147483647, true, true},
            {"2147483648",214748364, true, false},
            {"0b11111111111111111111111111111111",-1, true, true},
            {"0b111111111111111111111111111111111",-1, true, false},
            {"0b01111111111111111111111111111111",2147483647, true, true},
            {"0b011111111111111111111111111111111",2147483647, true, false},
            {"-2147483648",-2147483648, true, true},
            {"-2147483649",-214748364, true, false},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<StreamString,uint32> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFFFFFF",4294967295, true, true},
            {"0xFFFFFFFF1",4294967295, true, false},
            {"0x7FFFFFFF",2147483647, true, true},
            {"0o37777777777",4294967295, true, true},
            {"0o17777777777",2147483647, true, true},
            {"0o47777777777",671088639, true, false},
            {"0b11111111111111111111111111111111",4294967295, true, true},
            {"0b111111111111111111111111111111111",4294967295, true, false},
            {"4294967295",4294967295, true, true},
            {"4294967296",429496729, true, false},
            {"-1",0, true, false},
            {0,0, 0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<StreamString,int64> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFFFFFFFFFFFFFF",-1, true, true},
            {"0xFFFFFFFFFFFFFFFF1",-1, true, false},
            {"0x7FFFFFFFFFFFFFFF",9223372036854775807, true, true},
            {"0o1777777777777777777777",-1, true, true},
            {"0o777777777777777777777",9223372036854775807, true, true},
            {"0o2777777777777777777777",3458764513820540927, true, false},
            {"0b1111111111111111111111111111111111111111111111111111111111111111",-1, true, true},
            {"0b11111111111111111111111111111111111111111111111111111111111111111",-1, true, false},
            {"0b0111111111111111111111111111111111111111111111111111111111111111",9223372036854775807, true, true},
            {"0b01111111111111111111111111111111111111111111111111111111111111111",9223372036854775807, true, false},
            {"9223372036854775807",9223372036854775807, true, true},
            {"9223372036854775808",922337203685477580, true, false},
            {"-9223372036854775808",(int64)-9223372036854775808ul, true, true},
            {"-9223372036854775809",(int64)-922337203685477580, true, false},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<StreamString,uint64> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFFFFFFFFFFFFFF",18446744073709551615ul, true, true},
            {"0xFFFFFFFFFFFFFFFF1",18446744073709551615ul, true, false},
            {"0x7FFFFFFFFFFFFFFF",9223372036854775807ul, true, true},
            {"0o1777777777777777777777",18446744073709551615ul, true, true},
            {"0o777777777777777777777",9223372036854775807ul, true, true},
            {"0o2777777777777777777777",3458764513820540927ul, true, false},
            {"0b1111111111111111111111111111111111111111111111111111111111111111",18446744073709551615ul, true, true},
            {"0b11111111111111111111111111111111111111111111111111111111111111111",18446744073709551615ul, true, false},
            {"0b0111111111111111111111111111111111111111111111111111111111111111",9223372036854775807ul, true, true},
            {"0b01111111111111111111111111111111111111111111111111111111111111111",9223372036854775807ul, true, false},
            {"18446744073709551615",18446744073709551615ul, true, true},
            {"18446744073709551616",1844674407370955161ul, true, false},
            {"-1",0, true, false},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<StreamString,float32> table[]={
            {"123.5",123.5, true, true},
            {"123.12345",123.12345, true, true},
            {"1E+2",100, true,true},
            {"10E+2",1000, true,true},
            {"12345.678E2",1234567.8, true,true},
            {"1E-2",0.01, true,true},
            {"0.1E-2",0.001, true,true},
            {"16E-2",0.16, true,true},
            // see the precision is not the same!
            {"12345.678E-2",123.456772, true,true},
            {"1E+20",1E+20, true,true},
            // this is overflow!
            {"123E+42",123, true,false},
            {"1E-25",1E-25, true,true},
            {"0.000000001",1E-9, true,true},
            //Overflow!
     //       {"340282346638528859811704183484516999999",340282346638528859811704183484516925440},
     //       {"340282346638528859811704183484516925440",340282346638528859811704183484516925440},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<StreamString,float64> table[]={
            {"123.5",123.5, true, true},
            {"123.12345",123.12345, true, true},
            {"1E+2",100, true, true},
            {"10E+2",1000, true, true},
            {"12345.678E2",1234567.8, true, true},
            {"1e-2",0.01, true, true},
            {"0.1E-2",0.001, true, true},
            {"16E-2",0.16, true, true},
            // see the precision now is the same!
            {"12345.678E-2",123.45678, true, true},
            {"1E+20",1E+20, true, true},
            {"123E+63",123E+63, true, true},
            {"123e+83",123E+83, true, true},
            // this is overflow!
            {"123E+510",123, true, false},
            {"123E+513",123, true, false},
            {"1e-32",1E-32, true, true},
            {"0.000000001",1E-9, true, true},
            //Overflow!
     //       {"179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368",179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368},
     //       {"179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368",179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



///////////////////////////////////////////////////////////////////////////////////////////////

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt8ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,int8> table[]={
            {1,1, true, true},
            {-1,-1,true, true},
            {127,127,true, true},
            {-128, -128, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt8ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,uint8> table[]={
            {1,1, true, true},
            {-1,0,true, true},
            {127,127,true, true},
            {-128, 0, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt8ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,int16> table[]={
            {1,1, true, true},
            {-1,-1,true, true},
            {127,127,true, true},
            {-128, -128, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt8ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,uint16> table[]={
            {1,1, true, true},
            {-1,0,true, true},
            {127,127,true, true},
            {-128, 0, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt8ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,int32> table[]={
            {1,1, true, true},
            {-1,-1,true, true},
            {127,127,true, true},
            {-128, -128, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt8ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,uint32> table[]={
            {1,1, true, true},
            {-1,0,true, true},
            {127,127,true, true},
            {-128, 0, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt8ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,int64> table[]={
            {1,1, true, true},
            {-1,-1,true, true},
            {127,127,true, true},
            {-128, -128, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt8ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,uint64> table[]={
            {1,1ul, true, true},
            {-1,0ul,true, true},
            {127,127ul,true, true},
            {-128, 0ul, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

//////////////////////////////////////////////////////////////////////////////////////////




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt8ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,int8> table[]={
            {1,1, true, true},
            {128,127,true, true},
            {255,127,true, true},
            {127,127,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt8ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,uint8> table[]={
            {1,1, true, true},
            {255,255,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt8ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,int16> table[]={
            {1,1, true, true},
            {255,255,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt8ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,uint16> table[]={
            {1,1, true, true},
            {255,255,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt8ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,int32> table[]={
            {1,1, true, true},
            {255,255,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt8ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,uint32> table[]={
            {1,1, true, true},
            {255,255,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt8ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,int64> table[]={
            {1,1, true, true},
            {255,255,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt8ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,uint64> table[]={
            {1,1ul, true, true},
            {255,255ul,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

//////////////////////////////////////////////////////////////



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt16ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,int8> table[]={
            {1,1, true, true},
            {-1,-1,true, true},
            {127,127,true, true},
            {128, 127, true, true},
            {-128, -128, true, true},
            {-129, -128, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt16ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,uint8> table[]={
            {1,1, true, true},
            {-1,0,true, true},
            {255,255,true, true},
            {256, 255, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt16ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,int16> table[]={
            {1,1, true, true},
            {-1,-1,true, true},
            {32767,32767,true, true},
            {-32768, -32768, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt16ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,uint16> table[]={
            {1,1, true, true},
            {32767,32767,true, true},
            {-1, 0, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt16ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,int32> table[]={
            {1,1, true, true},
            {-1,-1,true, true},
            {32767,32767,true, true},
            {-32768, -32768, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt16ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,uint32> table[]={
            {1,1, true, true},
            {32767,32767,true, true},
            {-1, 0, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt16ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,int64> table[]={
            {1,1ll, true, true},
            {-1,-1ll,true, true},
            {32767,32767ll,true, true},
            {-32768, -32768ll, true, true},
            {0,0ll,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt16ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,uint64> table[]={
            {1,1ul, true, true},
            {32767,32767ul,true, true},
            {-1, 0ul, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

//////////////////////////////////////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt16ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,int8> table[]={
            {1,1, true, true},
            {127,127,true, true},
            {128,127,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt16ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,uint8> table[]={
            {1,1, true, true},
            {256,255,true, true},
            {255,255,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt16ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,int16> table[]={
            {1,1, true, true},
            {32768,32767,true, true},
            {32767, 32767, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt16ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,uint16> table[]={
            {1,1, true, true},
            {65535,65535,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt16ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,int32> table[]={
            {1,1, true, true},
            {65535,65535,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt16ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,uint32> table[]={
            {1,1, true, true},
            {65535,65535,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt16ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,int64> table[]={
            {1,1, true, true},
            {65535,65535,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt16ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,uint64> table[]={
            {1,1ul, true, true},
            {65535,65535ul,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,int8> table[]={
            {1,1, true, true},
            {-1,-1,true, true},
            {-128, -128, true, true},
            {127,127,true, true},
            {128,127,true, true},
            {-129,-128, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,uint8> table[]={
            {1,1, true, true},
            {-1,0,true, true},
            {255,255,true, true},
            {256, 255, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,int16> table[]={
            {1,1, true, true},
            {-1,-1,true, true},
            {32767,32767,true, true},
            {-32768, -32768, true, true},
            {32768,32767,true, true},
            {-32769, -32768, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,uint16> table[]={
            {1,1, true, true},
            {65535,65535,true, true},
            {65536,65535,true, true},
            {-1, 0, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,int32> table[]={
            {1,1, true, true},
            {-1,-1,true, true},
            {2147483647,2147483647,true, true},
            {-2147483648, -2147483648, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,uint32> table[]={
            {1,1, true, true},
            {2147483647,2147483647,true, true},
            {-1, 0, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,int64> table[]={
            {1,1LL, true, true},
            {-1,-1LL,true, true},
            {2147483647,2147483647LL,true, true},
            {-2147483648, -2147483648LL, true, true},
            {0,0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,uint64> table[]={
            {1,1ul, true, true},
            {2147483647,2147483647ul,true, true},
            {-1, 0ul, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



///////////////////////////////////////////////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,int8> table[]={
            {1,1, true, true},
            {127,127,true, true},
            {128,127,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,uint8> table[]={
            {1,1, true, true},
            {256,255,true, true},
            {255,255,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,int16> table[]={
            {1,1, true, true},
            {32768,32767,true, true},
            {32767, 32767, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,uint16> table[]={
            {1,1, true, true},
            {65535,65535,true, true},
            {65536,65535,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,int32> table[]={
            {1,1, true, true},
            {2147483647,2147483647,true, true},
            {2147483648,2147483647,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,uint32> table[]={
            {1,1, true, true},
            {4294967295,4294967295,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,int64> table[]={
            {1,1, true, true},
            {4294967295,4294967295,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,uint64> table[]={
            {1,1ul, true, true},
            {4294967295,4294967295ul,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


////////////////////////////////////////////////////////////////////////////////////



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt64ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,int8> table[]={
            {1,1, true, true},
            {-1,-1,true, true},
            {-128, -128, true, true},
            {127,127,true, true},
            {128,127,true, true},
            {-129,-128, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt64ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,uint8> table[]={
            {1,1, true, true},
            {-1,0,true, true},
            {255,255,true, true},
            {256, 255, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt64ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,int16> table[]={
            {1,1, true, true},
            {-1,-1,true, true},
            {32767,32767,true, true},
            {-32768, -32768, true, true},
            {32768,32767,true, true},
            {-32769, -32768, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt64ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,uint16> table[]={
            {1,1, true, true},
            {65535,65535,true, true},
            {65536,65535,true, true},
            {-1, 0, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt64ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,int32> table[]={
            {1,1, true, true},
            {-1,-1,true, true},
            {2147483647LL,2147483647,true, true},
            {-2147483648LL, -2147483648, true, true},
            {2147483648LL,2147483647,true, true},
            {-2147483649LL, -2147483648, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt64ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,uint32> table[]={
            {1,1, true,true},
            {4294967295,4294967295,true, true},
            {4294967296,4294967295,true, true},
            {-1, 0, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt64ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,int64> table[]={
            {1,1, true, true},
            {-1,-1,true, true},
            {9223372036854775807,9223372036854775807,true, true},
            {(int64)-9223372036854775808ul, (int64)-9223372036854775808ul, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt64ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,uint64> table[]={
            {1,1ul, true, true},
            {9223372036854775807,9223372036854775807ul,true, true},
            {-1, 0ul, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


///////////////////////////////////////////////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt64ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,int8> table[]={
            {1,1, true, true},
            {127,127,true, true},
            {128,127,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt64ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,uint8> table[]={
            {1,1, true, true},
            {256,255,true, true},
            {255,255,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt64ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,int16> table[]={
            {1,1, true, true},
            {32768,32767,true, true},
            {32767, 32767, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt64ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,uint16> table[]={
            {1,1, true, true},
            {65535,65535,true, true},
            {65536,65535,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt64ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,int32> table[]={
            {1,1, true, true},
            {2147483647,2147483647,true, true},
            {2147483648,2147483647,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt64ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,uint32> table[]={
            {1,1, true, true},
            {4294967295,4294967295,true, true},
            {4294967296,4294967295,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt64ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,int64> table[]={
            {1,1, true, true},
            {9223372036854775807ul,9223372036854775807,true, true},
            {9223372036854775808ul,9223372036854775807,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt64ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,uint64> table[]={
            {1,1, true, true},
            {18446744073709551615ul,18446744073709551615ul,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

///////////////////////////////////////////////////////////////////////////////////////////

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt8ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,float32> table[]={
            {1,1, true, true},
            {-1,-1,true, true},
            {-128,-128,true, true},
            {127,127,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt8ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,float64> table[]={
            {1,1, true, true},
            {-1,-1,true, true},
            {-128,-128,true, true},
            {127,127,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


///////////////////////////////////////////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt8ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,float32> table[]={
            {1,1, true, true},
            {255,255,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt8ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,float64> table[]={
            {1,1, true, true},
            {255,255,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt16ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,float32> table[]={
            {1,1, true, true},
            {-1,-1,true, true},
            {32767,32767,true, true},
            {-32768,-32768,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt16ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,float64> table[]={
            {1,1, true, true},
            {-1,-1,true, true},
            {32767,32767,true, true},
            {-32768,-32768,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


///////////////////////////////////////////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt16ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,float32> table[]={
            {1,1, true, true},
            {65535,65535,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt16ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,float64> table[]={
            {1,1, true, true},
            {65535,65535,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,float32> table[]={
            {1,1, true, true},
            {-1,-1,true, true},
            {2147483647,2147483647.0,true, true},
            {-2147483648,-2147483648.0,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,float64> table[]={
            {1,1.0, true, true},
            {-1,-1.0,true, true},
            {2147483647,2147483647.0,true, true},
            {-2147483648,-2147483648.0,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


///////////////////////////////////////////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,float32> table[]={
            {1,1, true, true},
            {4294967295,(float32)4294967295,true, true},
            {0,(float32)0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,float64> table[]={
            {1,1, true, true},
            {4294967295,4294967295,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt64ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,float32> table[]={
            {1,1, true, true},
            {-1,-1,true, true},
            {9223372036854775807,(float32)9223372036854775807,true, true},
            {(int64)-9223372036854775808ul,(float32)-9223372036854775808.0,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt64ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,float64> table[]={
            {1,1, true, true},
            {-1,-1,true, true},
            {9223372036854775807,(float64)9223372036854775807,true, true},
            {(int64)-9223372036854775808ul,(float64)-9223372036854775808.0,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


///////////////////////////////////////////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt64ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,float32> table[]={
            {1,1, true, true},
            {18446744073709551615ul,18446744073709551615.0f,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt64ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,float64> table[]={
            {1,1, true, true},
            {18446744073709551615ul,18446744073709551615.0,true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,int8> table[]={
            {1.9,2, true, true},
            {126.5, 127, true, true},
            {126.51, 127, true, true},
            {127.999, 127, true, true},
            {127.49, 127, true, true},
            {127.51, 127, true, true},
            {128.0, 127, true, true},
            {-1.0, -1, true, true},
            {-127.49,-127,true, true},
            {-127.5,-128,true, true},
            {-127.51, -128, true, true},
            {-128.9, -128, true, true},
            {-129.0, -128, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,uint8> table[]={
            {1.9,2, true, true},
            {255.0, 255, true, true},
            {254.499, 254, true, true},
            {254.5001, 255, true, true},
            {255.99, 255, true, true},
            {256.0, 255, true, true},
            {-1E-9, 0, true, true},
            {-2.33,0,true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,int16> table[]={
            {1.9,2, true, true},
            {32766.5, 32767, true, true},
            {32766.51, 32767, true, true},
            {32767.999, 32767, true, true},
            {32767.49, 32767, true, true},
            {32767.51, 32767, true, true},
            {32768.0, 32767, true, true},
            {-1.0, -1, true, true},
            {-32767.49,-32767,true, true},
            {-32767.5,-32768,true, true},
            {-32767.51, -32768, true, true},
            {-32768.9, -32768, true, true},
            {-32769.0, -32768, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,uint16> table[]={
            {1.9,2, true, true},
            {65535.0, 65535, true, true},
            {65534.49, 65534, true, true},
            {65534.505, 65535, true, true},
            {65535.99, 65535, true, true},
            {65536.0, 65535, true, true},
            {-1E-9, 0, true, true},
            {-2.33,0,true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,int32> table[]={
            {1.9,2, true, true},
            // precision lost!!
            {214748364.0f, 214748368, true, true},
            // precision lost!!
            {2147483646.5f, 2147483647, true, true},
            {-1.0, -1, true, true},
            {-2147483647.5,-2147483648,true, true},
            {-2147483649.0, -2147483648, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,uint32> table[]={
            {1.9,2, true,true},
            //loss of precision !
            {4294967295.0, 4294967295, true, true},
            {4294967294.499, 4294967295, true, true},
            {4294967294.505, 4294967295, true, true},
            {4294967295.99, 4294967295, true, true},
            {4294967296.0,4294967295, true, true},
            {65536.0, 65536, true, true},
            {-1E-9, 0, true, true},
            {-2.33,0,true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,int64> table[]={
            {1.9,2, true,true},
            {99223372036854775809.0f, 9223372036854775807, true, true},
            {99223372036854775807.0f, 9223372036854775807, true, true},
            // precision lost!!
            {9223372036854775807.0f, 9223372036854775807, true, true},
            // precision lost!!
            {9223372036854775806.5f, 9223372036854775807, true, true},
            {-1.0, -1, true,true},
            // precision lost!!
            {-9223372036854775807.0,(int64)-9223372036854775808ul,true, true},
            // precision lost!!
            {-9223372036854775807.5,(int64)-9223372036854775808ul, true, true},
            {-9223372036854775808.0, (int64)-9223372036854775808ul, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,uint64> table[]={
            {1.9,2, true,true},
            {18446744073709551617.0, 18446744073709551615ul, true, true},
            //loss of precision !
            {18446744073709551615.0, 18446744073709551615ul, true, true},
            {18446744073709551614.499, 18446744073709551615ul, true, true},
            {18446744073709551613.9, 18446744073709551615ul, true, true},
            {-1E-9, 0, true, true},
            {-2.33,0,true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat64ToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,int8> table[]={
            {1.9,2, true, true},
            {126.5, 127, true, true},
            {126.51, 127, true, true},
            {127.999, 127, true, true},
            {127.49, 127, true, true},
            {127.51, 127, true, true},
            {128.0, 127, true, true},
            {-1.0, -1, true, true},
            {-127.49,-127,true, true},
            {-127.5,-128,true, true},
            {-127.51, -128, true, true},
            {-128.9, -128, true, true},
            {-129.0, -128, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat64ToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,uint8> table[]={
            {1.9,2, true, true},
            {255.0, 255, true, true},
            {254.499, 254, true, true},
            {254.5001, 255, true, true},
            {255.99, 255, true, true},
            {256.0, 255, true, true},
            {-1E-9, 0, true, true},
            {-2.33,0,true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat64ToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,int16> table[]={
            {1.9,2, true, true},
            {32766.5, 32767, true, true},
            {32766.51, 32767, true, true},
            {32767.999, 32767, true, true},
            {32767.49, 32767, true, true},
            {32767.51, 32767, true, true},
            {32768.0, 32767, true, true},
            {-1.0, -1, true, true},
            {-32767.5,-32768,true, true},
            {-32767.49,-32767,true, true},
            {-32767.51, -32768, true, true},
            {-32768.9, -32768, true, true},
            {-32769.0, -32768, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat64ToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,uint16> table[]={
            {1.9,2, true, true},
            {65535.0, 65535, true, true},
            {65534.499, 65534, true, true},
            {65534.505, 65535, true, true},
            {65535.99, 65535, true, true},
            {65536.0, 65535, true, true},
            {-1E-9, 0, true, true},
            {-2.33,0,true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat64ToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,int32> table[]={
            {1.9,2, true, true},
            {2147483646.49, 2147483646, true, true},
            {2147483646.5, 2147483647, true, true},
            {-1.0, -1, true, true},
            {-2147483647.5,-2147483648,true, true},
            {-2147483647.49,-2147483647,true, true},
            {-2147483649.0, -2147483648, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat64ToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,uint32> table[]={
            {1.9,2, true, true},
            //loss of precision !
            {4294967295.0, 4294967295, true, true},
            {4294967294.499, 4294967294, true, true},
            {4294967294.505, 4294967295, true, true},
            {4294967295.99, 4294967295, true, true},
            {4294967296.0,4294967295, true, true},
            {65536.0, 65536, true, true},
            {-1E-9, 0, true, true},
            {-2.33,0,true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

/////////////////////////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat64ToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,int64> table[]={
            {1.9,2, true, true},
            {99223372036854775809.0f, 9223372036854775807, true, true},
            {99223372036854775807.0f, 9223372036854775807, true, true},
            // precision lost!!
            {9223372036854775807.0f, 9223372036854775807, true, true},
            // precision lost!!
            {9223372036854775806.5f, 9223372036854775807, true, true},
            {-1.0, -1, true,true},
            // precision lost!!
            {-9223372036854775807.0,(int64)-9223372036854775808ul,true, true},
            // precision lost!!
            {-9223372036854775807.5,(int64)-9223372036854775808ul,true, true},
            {-9223372036854775808.0, (int64)-9223372036854775808ul, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat64ToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,uint64> table[]={
            {1.9,2, true, true},
            {18446744073709551617.0, 18446744073709551615ul, true,true},
            //loss of precision !
            {18446744073709551615.0, 18446744073709551615ul, true,true},
            {18446744073709551614.499, 18446744073709551615ul, true,true},
            {18446744073709551613.9, 18446744073709551615ul, true,true},
            {-2.33,0,true,true},
            {-1E-9, 0, true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


/////////////////////////////////////////////////////////////////



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,float32> table[]={
            {1.9,1.9, true,true},
            {-1.9, -1.9, true,true},
            {340282346638528859811704183484516925440.000000,340282346638528859811704183484516925440.000000, true,true},
            {-340282346638528859811704183484516925440.000000,-340282346638528859811704183484516925440.000000, true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,float64> table[]={
            {1.5,1.5, true,true},
            {-1.5, -1.5, true,true},
            {340282346638528859811704183484516925440.000000,340282346638528859811704183484516925440.000000, true,true},
            {-340282346638528859811704183484516925440.000000,-340282346638528859811704183484516925440.000000, true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat64ToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,float64> table[]={
            {1.9,1.9, true,true},
            {-1.9, -1.9, true,true},
#if ENVIRONMENT != Windows
            {179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000,179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000, true,true},
            {-179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000,-179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.000000, true,true},
#endif
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat64ToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,float32> table[]={
            {1.5,1.5, true,true},
            {-1.5, -1.5, true,true},
            {340282346638528859811704183484516925440.000000,340282346638528859811704183484516925440.000000, true,true},
            {-340282346638528859811704183484516925440.000000,-340282346638528859811704183484516925440.000000, true,true},

            // Saturation
            {440282346638528859811704183484516925480.000000,340282346638528859811704183484516925440.000000, true,true},
            {-440282346638528859811704183484516925480.000000,-340282346638528859811704183484516925440.000000, true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



//////////////////////////////////////////////////////////////////////////////////////////

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt8ToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,StreamString> table[]={
            {0,"0",true,true},
            {123, "123", true,true},
            {-1,"-1", true,true},
            {127,"127", true,true},
            {-128,"-128", true,true},
            {(int8)255,"-1",true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt8ToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,StreamString> table[]={
            {123,"123", true,true},
            {255,"255", true,true},
            {127,"127", true,true},
            {(uint8)-1,"255",true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt16ToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,StreamString> table[]={
            {0,"0",true,true},
            {123, "123", true,true},
            {-1,"-1", true,true},
            {32767,"32767", true,true},
            {-32768,"-32768", true,true},
            {(int16)65535,"-1", true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt16ToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,StreamString> table[]={
            {123,"123", true,true},
            {65535,"65535", true,true},
            {(uint16)65536,"0", true,true},
            {32767,"32767", true,true},
            {(uint16)-1,"65535",true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,StreamString> table[]={
            {0,"0",true,true},
            {123, "123", true,true},
            {-1,"-1", true,true},
            {2147483647,"2147483647", true,true},
            {(int32)2147483648,"-2147483648", true,true},
            {(int32)-2147483648,"-2147483648", true,true},
            {(int32)-2147483649,"2147483647", true,true},
            {(int32)4294967295,"-1", true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,StreamString> table[]={
            {123,"123", true,true},
            {4294967295,"4294967295", true,true},
            {(uint32)4294967296,"0", true,true},
            {2147483647,"2147483647", true,true},
            {(uint32)-1,"4294967295",true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt64ToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,StreamString> table[]={
            {0,"0",true,true},
            {123, "123", true,true},
            {-1,"-1", true,true},
            {9223372036854775807,"9223372036854775807", true,true},
            {(int64)9223372036854775808ul,"-9223372036854775808", true,true},
            {(int64)-9223372036854775808ul,"-9223372036854775808", true,true},
            {(int64)-9223372036854775809ul,"9223372036854775807", true,true},
            {(int64)18446744073709551615ul,"-1", true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt64ToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,StreamString> table[]={
            {123ULL,"123", true,true},
            {18446744073709551615ULL,"18446744073709551615", true,true},
            {0ULL,"0", true,true},
            {9223372036854775808ULL,"9223372036854775808", true,true},
            {-1ULL,"18446744073709551615",true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,float32ToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,StreamString> table[]={
            {123.5,"123.5000", true,true},
            {123.12345,"123.1234", true,true},
            {123.123456,"123.1235", true,true},
            {100,"100.0000", true,true},
            {10E+2,"1.000000E+3", true,true},
            {12345.678E+2,"1.234568E+6", true,true},
            {1E-2,"10.00000E-3", true,true},
            {0.1E-2,"1.000000E-3", true,true},
            {16E-2,"160.0000E-3", true,true},
            {1E+20,"100.0000E+18", true,true},
            {1E-7,"100.0000E-9", true,true},
            {1E-9,"1.000000E-9", true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat64ToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,StreamString> table[]={
            {123.5,"123.500000000000", true,true},
            {123.12345,"123.123450000000", true,true},
            {123.123456,"123.123456000000", true,true},
            {100,"100.000000000000", true,true},
            {10E+2,"1.00000000000000E+3", true,true},
            {12345.678E+2,"1.23456780000000E+6", true,true},
            {1E-2,"10.0000000000000E-3", true,true},
            {0.1E-2,"1.00000000000000E-3", true,true},
            {16E-2,"160.000000000000E-3", true,true},
            {123E+63,"123.000000000000E+63", true,true},
            {123E+83,"12.3000000000000E+84", true,true},
            {1E-7,"100.000000000000E-9", true,true},
            {1E-9,"1.00000000000000E-9", true,true},
            {1E-25,"100.000000000000E-27", true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvert(table));
}


///////////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt8ToCCString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,const char8*> table[]={
            {0,"0",true,true},
            {123, "123", true,true},
            {-1,"-1", true,true},
            {127,"127", true,true},
            {-128,"-128", true,true},
            {(int8)255,"-1",true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCCString(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt8ToCCString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,const char8*> table[]={
            {123,"123", true,true},
            {255,"255", true,true},
            {127,"127", true,true},
            {(uint8)-1,"255",true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCCString(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt16ToCCString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,const char8*> table[]={
            {0,"0",true,true},
            {123, "123", true,true},
            {-1,"-1", true,true},
            {32767,"32767", true,true},
            {-32768,"-32768", true,true},
            {(int16)65535,"-1", true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCCString(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt16ToCCString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,const char8*> table[]={
            {123,"123", true,true},
            {65535,"65535", true,true},
            {(uint16)65536,"0", true,true},
            {32767,"32767", true,true},
            {(uint16)-1,"65535",true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCCString(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToCCString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,const char8*> table[]={
            {0,"0",true,true},
            {123, "123", true,true},
            {-1,"-1", true,true},
            {2147483647,"2147483647", true,true},
            {(int32)2147483648,"-2147483648", true,true},
            {(int32)-2147483648,"-2147483648", true,true},
            {(int32)-2147483649,"2147483647", true,true},
            {(int32)4294967295,"-1", true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCCString(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToCCString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,const char8*> table[]={
            {123,"123", true,true},
            {4294967295,"4294967295", true,true},
            {(uint32)4294967296,"0", true,true},
            {2147483647,"2147483647", true,true},
            {(uint32)-1,"4294967295",true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCCString(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt64ToCCString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,const char8*> table[]={
            {0,"0",true,true},
            {123, "123", true,true},
            {-1,"-1", true,true},
            {9223372036854775807,"9223372036854775807", true,true},
            {(int64)9223372036854775808ul,"-9223372036854775808", true,true},
            {(int64)-9223372036854775808ul,"-9223372036854775808", true,true},
            {(int64)-9223372036854775809ul,"9223372036854775807", true,true},
            {-1,"-1", true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCCString(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt64ToCCString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,const char8*> table[]={
            {123ULL,"123", true,true},
            {(uint64)9223372036854775808ULL,"9223372036854775808", true,true},
            {18446744073709551615ULL,"18446744073709551615", true,true},
            {0ULL,"0", true,true},
            {-1ULL,"18446744073709551615",true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCCString(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,float32ToCCString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,const char8*> table[]={
            {123.5,"123.5000", true,true},
            {123.12345,"123.1234", true,true},
            {123.123456,"123.1235", true,true},
            {100,"100.0000", true,true},
            {10E+2,"1.000000E+3", true,true},
            {12345.678E+2,"1.234568E+6", true,true},
            {1E-2,"10.00000E-3", true,true},
            {0.1E-2,"1.000000E-3", true,true},
            {16E-2,"160.0000E-3", true,true},
            {1E+20,"100.0000E+18", true,true},
            {1E-7,"100.0000E-9", true,true},
            {1E-9,"1.000000E-9", true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCCString(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat64ToCCString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,const char8*> table[]={
            {123.5,"123.500000000000", true,true},
            {123.12345,"123.123450000000", true,true},
            {123.123456,"123.123456000000", true,true},
            {100,"100.000000000000", true,true},
            {10E+2,"1.00000000000000E+3", true,true},
            {12345.678E+2,"1.23456780000000E+6", true,true},
            {1E-2,"10.0000000000000E-3", true,true},
            {0.1E-2,"1.00000000000000E-3", true,true},
            {16E-2,"160.000000000000E-3", true,true},
            {123E+63,"123.000000000000E+63", true,true},
            {123E+83,"12.3000000000000E+84", true,true},
            {1E-7,"100.000000000000E-9", true,true},
            {1E-9,"1.00000000000000E-9", true,true},
            {1E-25,"100.000000000000E-27", true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCCString(table));
}


/////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToInt32Vector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<int32,int32,3> table[]={
            {{-2147483648,0,2147483647},{-2147483648,0,2147483647}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVector(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToUInt32Vector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<int32,uint32,3> table[]={
            {{-1,2147483647,0},{0,2147483647,0}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVector(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToInt32Vector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<uint32,int32,3> table[]={
            {{2147483647,0,2147483648},{2147483647,0,2147483647}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVector(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToFloat32Vector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<int32,float32,3> table[]={
            {{-2147483648,0,2147483647},{-2147483648.0,0.0,2147483647.0}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVector(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToFloat32Vector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<uint32,float32,3> table[]={
            {{4294967295,0,2147483647},{4294967295.0,0.0,2147483647.0}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVector(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToInt32Vector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<float32,int32,3> table[]={
            {{123.45,0.0,-123.5},{123,0,-124}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVector(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToUInt32Vector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<float32,uint32,3> table[]={
            {{123.45,-1.2,123.5},{123,0,124}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVector(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToInt32Vector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<const char8 *,int32,3> table[]={
            {{"-1","0","2147483647"},{-1,0,2147483647}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVector(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToUInt32Vector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<const char8 *,uint32,3> table[]={
            {{"-1","2147483647","4294967295"},{0,2147483647,4294967295}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVector(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToInt32Vector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<StreamString,int32,3> table[]={
            {{"1","0","2147483647"},{1,0,2147483647}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVector(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToUInt32Vector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<StreamString,uint32,3> table[]={
            {{"-1","2147483647","4294967295"},{0,2147483647,4294967295}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVector(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToFloat32Vector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<const char8 *,float32,3> table[]={
            {{"-0.02E-2","1234.5E+5","0.00001"},{-0.02E-2,1234.5E+5,1E-5}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVector(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToFloat32Vector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<StreamString,float32,3> table[]={
            {{"-0.02E-2","1234.5E+5","0.00001"},{-0.02E-2,1234.5E+5,1E-5}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVector(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToSStringVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<int32,StreamString,3> table[]={
            {{(int32)2147483648,(int32)4294967295,2147483647},{"-2147483648","-1","2147483647"}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVector(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToSStringVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<uint32,StreamString,3> table[]={
            {{(uint32)-1,2147483647,4294967295},{"4294967295","2147483647","4294967295"}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVector(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToSStringVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<float32,StreamString,3> table[]={
            {{-0.02E-2,1234.5E+5,1E-5},{"-200.0000E-6","123.4500E+6","10.00000E-6"}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVector(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToStringVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<const char8*,StreamString,3> table[]={
            {{"-200E-6","123.4500E+6","10000000000000000000000000000E-6"},{"-200E-6","123.4500E+6","10000000000000000000000000000E-6"}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVector(table));
}

////////////////////////////////////////////////////////



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToCCStringVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<int32,const char8 *,3> table[]={
            {{(int32)2147483648,(int32)4294967295,2147483647},{"-2147483648","-1","2147483647"}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVectorToCharStarVector(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToCCStringVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<uint32,const char8 *,3> table[]={
            {{(uint32)-1,2147483647,4294967295},{"4294967295","2147483647","4294967295"}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVectorToCharStarVector(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToCCStringVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<float32,const char8 *,3> table[]={
            {{-0.02E-2,1234.5E+5,1E-5},{"-200.0000E-6","123.4500E+6","10.00000E-6"}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVectorToCharStarVector(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToCCStringVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<const char8*,const char8 *,3> table[]={
            {{"-200E-6","123.4500E+6","10000000000000000000000000000E-6"},{"-200E-6","123.4500E+6","10000000000000000000000000000E-6"}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVectorToCharStarVector(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestStringToCCStringVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<StreamString,const char8 *,3> table[]={
            {{"-200E-6","123.4500E+6","10000000000000000000000000000E-6"},{"-200E-6","123.4500E+6","10000000000000000000000000000E-6"}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVectorToCharStarVector(table));
}

////////////////////////////////////////////////////////

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToInt32MatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<int32,int32,2,2> table[]={
            {{{2147483647,0},{-2147483648,-1}},{{2147483647,0},{-2147483648,-1}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStatic(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToUInt32MatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<int32,uint32,2,2> table[]={
            {{{2147483647,0},{1234,-1}},{{2147483647,0},{1234,0}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStatic(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToInt32MatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<uint32,int32,2,2> table[]={
            {{{2147483647,2147483648},{1234,0}},{{2147483647,2147483647},{1234,0}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStatic(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToInt32MatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<float32,int32,2,2> table[]={
            {{{2147483647.5,-1234.47},{1234.67,0.01}},{{2147483647,-1234},{1235,0}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStatic(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToUInt32MatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<float32,uint32,2,2> table[]={
            {{{4294967295.5,-1234.47},{1234.67,0.5}},{{4294967295,0},{1235,1}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStatic(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToFloat32MatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<int32,float32,2,2> table[]={
            {{{2147483647,-1234},{1234,-2147483648}},{{2147483647.0,-1234.0},{1234.0,-2147483648.0}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStatic(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToFloat32MatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<uint32,float32,2,2> table[]={
            {{{4294967295,0},{1235,1}},{{4294967295.0,0.0},{1235.0,1.0}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStatic(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToInt32MatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8 *,int32,2,2> table[]={
            {{{"-1","0"},{"2147483647","-2147483648"}},{{-1,0},{2147483647,-2147483648}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStatic(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToUInt32MatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8 *,uint32,2,2> table[]={
            {{{"-1","0"},{"2147483647","4294967295"}},{{0,0},{2147483647,4294967295}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStatic(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToInt32MatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<StreamString,int32,2,2> table[]={
            {{{"-1","0"},{"2147483647","-2147483648"}},{{-1,0},{2147483647,-2147483648}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStatic(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToUInt32MatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<StreamString,uint32,2,2> table[]={
            {{{"-1","0"},{"2147483647","4294967295"}},{{0,0},{2147483647,4294967295}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStatic(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToFloat32MatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8 *,float32,2,2> table[]={
            {{{"-0.02E-2","1234.5E+5"},{"0.00001","1E-20"}},{{-0.02E-2,1234.5E+5},{1E-5,1E-20}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStatic(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToFloat32MatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<StreamString,float32,2,2> table[]={
            {{{"-0.02E-2","1234.5E+5"},{"0.00001","1E-20"}},{{-0.02E-2,1234.5E+5},{1E-5,1E-20}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStatic(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToSStringMatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<int32,StreamString,2,2> table[]={
            {{{(int32)2147483648,(int32)4294967295},{2147483647,0}},{{"-2147483648","-1"},{"2147483647","0"}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStatic(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToSStringMatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<uint32,StreamString,2,2> table[]={
            {{{(uint32)-1,2147483647},{4294967295,(uint32)4294967296}},{{"4294967295","2147483647"},{"4294967295","0"}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStatic(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToSStringMatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<float32,StreamString,2,2> table[]={
            {{{-0.02E-2,1234.5E+5},{1E-5,0.123}},{{"-200.0000E-6","123.4500E+6"},{"10.00000E-6","123.0000E-3"}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStatic(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToStringMatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8*,StreamString,2,2> table[]={
            {{{"-200E-6","123.4500E+6"},{"10000000000000000000000000000E-6","HelloWorld"}},{{"-200E-6","123.4500E+6"},{"10000000000000000000000000000E-6","HelloWorld"}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStatic(table));
}


////////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToInt32MatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<int32,int32,2,2> table[]={
            {{{2147483647,0},{-2147483648,-1}},{{2147483647,0},{-2147483648,-1}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeap(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToUInt32MatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<int32,uint32,2,2> table[]={
            {{{2147483647,0},{1234,-1}},{{2147483647,0},{1234,0}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeap(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToInt32MatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<uint32,int32,2,2> table[]={
            {{{2147483647,2147483648},{1234,0}},{{2147483647,2147483647},{1234,0}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeap(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToInt32MatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<float32,int32,2,2> table[]={
            {{{2147483647.5,-1234.47},{1234.67,0.01}},{{2147483647,-1234},{1235,0}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeap(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToUInt32MatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<float32,uint32,2,2> table[]={
            {{{4294967295.5,-1234.47},{1234.67,0.5}},{{4294967295,0},{1235,1}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeap(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToFloat32MatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<int32,float32,2,2> table[]={
            {{{2147483647,-1234},{1234,-2147483648}},{{2147483647.0,-1234.0},{1234.0,-2147483648.0}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeap(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToFloat32MatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<uint32,float32,2,2> table[]={
            {{{4294967295,0},{1235,1}},{{4294967295.0,0.0},{1235.0,1.0}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeap(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToInt32MatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8 *,int32,2,2> table[]={
            {{{"-1","0"},{"2147483647","-2147483648"}},{{-1,0},{2147483647,-2147483648}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeap(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToUInt32MatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8 *,uint32,2,2> table[]={
            {{{"-1","0"},{"2147483647","4294967295"}},{{0,0},{2147483647,4294967295}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeap(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToInt32MatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<StreamString,int32,2,2> table[]={
            {{{"-1","0"},{"2147483647","-2147483648"}},{{-1,0},{2147483647,-2147483648}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeap(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToUInt32MatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<StreamString,uint32,2,2> table[]={
            {{{"-1","0"},{"2147483647","4294967295"}},{{0,0},{2147483647,4294967295}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeap(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToFloat32MatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8 *,float32,2,2> table[]={
            {{{"-0.02E-2","1234.5E+5"},{"0.00001","1E-20"}},{{-0.02E-2,1234.5E+5},{1E-5,1E-20}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeap(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToFloat32MatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<StreamString,float32,2,2> table[]={
            {{{"-0.02E-2","1234.5E+5"},{"0.00001","1E-20"}},{{-0.02E-2,1234.5E+5},{1E-5,1E-20}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeap(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToSStringMatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<int32,StreamString,2,2> table[]={
            {{{(int32)2147483648,(int32)4294967295},{2147483647,0}},{{"-2147483648","-1"},{"2147483647","0"}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeap(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToSStringMatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<uint32,StreamString,2,2> table[]={
            {{{(uint32)-1,2147483647},{4294967295,(uint32)4294967296}},{{"4294967295","2147483647"},{"4294967295","0"}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeap(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToSStringMatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<float32,StreamString,2,2> table[]={
            {{{-0.02E-2,1234.5E+5},{1E-5,0.123}},{{"-200.0000E-6","123.4500E+6"},{"10.00000E-6","123.0000E-3"}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeap(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToStringMatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8*,StreamString,2,2> table[]={
            {{{"-200E-6","123.4500E+6"},{"10000000000000000000000000000E-6","HelloWorld"}},{{"-200E-6","123.4500E+6"},{"10000000000000000000000000000E-6","HelloWorld"}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeap(table));
}

//////////////////////////////////////////////////////////////////////////////////

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToInt32MatrixHeapToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<int32,int32,2,2> table[]={
            {{{2147483647,0},{-2147483648,-1}},{{2147483647,0},{-2147483648,-1}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToStatic(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToUInt32MatrixHeapToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<int32,uint32,2,2> table[]={
            {{{2147483647,0},{1234,-1}},{{2147483647,0},{1234,0}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToStatic(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToInt32MatrixHeapToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<uint32,int32,2,2> table[]={
            {{{2147483647,2147483648},{1234,0}},{{2147483647,2147483647},{1234,0}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToStatic(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToInt32MatrixHeapToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<float32,int32,2,2> table[]={
            {{{2147483647.5,-1234.47},{1234.67,0.01}},{{2147483647,-1234},{1235,0}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToStatic(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToUInt32MatrixHeapToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<float32,uint32,2,2> table[]={
            {{{4294967295.5,-1234.47},{1234.67,0.5}},{{4294967295,0},{1235,1}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToStatic(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToFloat32MatrixHeapToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<int32,float32,2,2> table[]={
            {{{2147483647,-1234},{1234,-2147483648}},{{2147483647.0,-1234.0},{1234.0,-2147483648.0}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToStatic(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToFloat32MatrixHeapToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<uint32,float32,2,2> table[]={
            {{{4294967295,0},{1235,1}},{{4294967295.0,0.0},{1235.0,1.0}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToStatic(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToInt32MatrixHeapToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8 *,int32,2,2> table[]={
            {{{"-1","0"},{"2147483647","-2147483648"}},{{-1,0},{2147483647,-2147483648}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToStatic(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToUInt32MatrixHeapToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8 *,uint32,2,2> table[]={
            {{{"-1","0"},{"2147483647","4294967295"}},{{0,0},{2147483647,4294967295}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToStatic(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToInt32MatrixHeapToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<StreamString,int32,2,2> table[]={
            {{{"-1","0"},{"2147483647","-2147483648"}},{{-1,0},{2147483647,-2147483648}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToStatic(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToUInt32MatrixHeapToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<StreamString,uint32,2,2> table[]={
            {{{"-1","0"},{"2147483647","4294967295"}},{{0,0},{2147483647,4294967295}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToStatic(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToFloat32MatrixHeapToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8 *,float32,2,2> table[]={
            {{{"-0.02E-2","1234.5E+5"},{"0.00001","1E-20"}},{{-0.02E-2,1234.5E+5},{1E-5,1E-20}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToStatic(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToFloat32MatrixHeapToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<StreamString,float32,2,2> table[]={
            {{{"-0.02E-2","1234.5E+5"},{"0.00001","1E-20"}},{{-0.02E-2,1234.5E+5},{1E-5,1E-20}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToStatic(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToSStringMatrixHeapToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<int32,StreamString,2,2> table[]={
            {{{(int32)2147483648,(int32)4294967295},{2147483647,0}},{{"-2147483648","-1"},{"2147483647","0"}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToStatic(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToSStringMatrixHeapToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<uint32,StreamString,2,2> table[]={
            {{{(uint32)-1,2147483647},{4294967295,(uint32)4294967296}},{{"4294967295","2147483647"},{"4294967295","0"}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToStatic(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToSStringMatrixHeapToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<float32,StreamString,2,2> table[]={
            {{{-0.02E-2,1234.5E+5},{1E-5,0.123}},{{"-200.0000E-6","123.4500E+6"},{"10.00000E-6","123.0000E-3"}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToStatic(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToStringMatrixHeapToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8*,StreamString,2,2> table[]={
            {{{"-200E-6","123.4500E+6"},{"10000000000000000000000000000E-6","HelloWorld"}},{{"-200E-6","123.4500E+6"},{"10000000000000000000000000000E-6","HelloWorld"}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToStatic(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToInt32MatrixHeapToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<int32,int32,2,2> table[]={
            {{{2147483647,0},{-2147483648,-1}},{{2147483647,0},{-2147483648,-1}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToHeap(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToUInt32MatrixHeapToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<int32,uint32,2,2> table[]={
            {{{2147483647,0},{1234,-1}},{{2147483647,0},{1234,0}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToHeap(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToInt32MatrixHeapToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<uint32,int32,2,2> table[]={
            {{{2147483647,2147483648},{1234,0}},{{2147483647,2147483647},{1234,0}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToHeap(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToInt32MatrixHeapToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<float32,int32,2,2> table[]={
            {{{2147483647.5,-1234.47},{1234.67,0.01}},{{2147483647,-1234},{1235,0}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToHeap(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToUInt32MatrixHeapToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<float32,uint32,2,2> table[]={
            {{{4294967295.5,-1234.47},{1234.67,0.5}},{{4294967295,0},{1235,1}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToHeap(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToFloat32MatrixHeapToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<int32,float32,2,2> table[]={
            {{{2147483647,-1234},{1234,-2147483648}},{{2147483647.0,-1234.0},{1234.0,-2147483648.0}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToHeap(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToFloat32MatrixHeapToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<uint32,float32,2,2> table[]={
            {{{4294967295,0},{1235,1}},{{4294967295.0,0.0},{1235.0,1.0}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToHeap(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToInt32MatrixHeapToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8 *,int32,2,2> table[]={
            {{{"-1","0"},{"2147483647","-2147483648"}},{{-1,0},{2147483647,-2147483648}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToHeap(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToUInt32MatrixHeapToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8 *,uint32,2,2> table[]={
            {{{"-1","0"},{"2147483647","4294967295"}},{{0,0},{2147483647,4294967295}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToHeap(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToInt32MatrixHeapToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<StreamString,int32,2,2> table[]={
            {{{"-1","0"},{"2147483647","-2147483648"}},{{-1,0},{2147483647,-2147483648}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToHeap(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToUInt32MatrixHeapToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<StreamString,uint32,2,2> table[]={
            {{{"-1","0"},{"2147483647","4294967295"}},{{0,0},{2147483647,4294967295}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToHeap(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToFloat32MatrixHeapToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8 *,float32,2,2> table[]={
            {{{"-0.02E-2","1234.5E+5"},{"0.00001","1E-20"}},{{-0.02E-2,1234.5E+5},{1E-5,1E-20}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToHeap(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToFloat32MatrixHeapToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<StreamString,float32,2,2> table[]={
            {{{"-0.02E-2","1234.5E+5"},{"0.00001","1E-20"}},{{-0.02E-2,1234.5E+5},{1E-5,1E-20}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToHeap(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToSStringMatrixHeapToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<int32,StreamString,2,2> table[]={
            {{{(int32)2147483648,(int8)4294967295},{2147483647,0}},{{"-2147483648","-1"},{"2147483647","0"}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToHeap(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToSStringMatrixHeapToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<uint32,StreamString,2,2> table[]={
            {{{(uint32)-1,2147483647},{4294967295,(uint32)4294967296}},{{"4294967295","2147483647"},{"4294967295","0"}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToHeap(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToSStringMatrixHeapToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<float32,StreamString,2,2> table[]={
            {{{-0.02E-2,1234.5E+5},{1E-5,0.123}},{{"-200.0000E-6","123.4500E+6"},{"10.00000E-6","123.0000E-3"}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToHeap(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToStringMatrixHeapToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8*,StreamString,2,3> table[]={
            {{{"-200E-6","123.4500E+6","abcd"},{"10000000000000000000000000000E-6","HelloWorld","efgh"}},{{"-200E-6","123.4500E+6","abcd"},{"10000000000000000000000000000E-6","HelloWorld","efgh"}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToHeap(table));
}


//////////////////////////////////////////////////////////////////////////////////

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt8ToCArray) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,const char8 *> table[]={
            {0,"0",true,true},
            {123, "123", true,true},
            {-1,"-1", true,true},
            {127,"127", true,true},
            {-128,"-128", true,true},
            {(int8)255,"-1",true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertCArray(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt8ToCArray) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,const char8 *> table[]={
            {123,"123", true,true},
            {255,"255", true,true},
            {127,"127", true,true},
            {(uint8)-1,"255",true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertCArray(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt16ToCArray) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,const char8 *> table[]={
            {0,"0",true,true},
            {123, "123", true,true},
            {-1,"-1", true,true},
            {32767,"32767", true,true},
            {-32768,"-32768", true,true},
            {(int16)65535,"-1", true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertCArray(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt16ToCArray) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,const char8 *> table[]={
            {123,"123", true,true},
            {65535,"65535", true,true},
            {(uint16)65536,"0", true,true},
            {32767,"32767", true,true},
            {(uint16)-1,"65535",true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertCArray(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToCArray) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,const char8 *> table[]={
            {0,"0",true,true},
            {123, "123", true,true},
            {-1,"-1", true,true},
            {2147483647,"2147483647", true,true},
            {(int32)2147483648,"-2147483648", true,true},
            {-2147483648,"-2147483648", true,true},
            {(int32)-2147483649,"2147483647", true,true},
            {(int32)4294967295,"-1", true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertCArray(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToCArray) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,const char8 *> table[]={
            {123,"123", true,true},
            {4294967295,"4294967295", true,true},
            {(uint32)4294967296,"0", true,true},
            {2147483647,"2147483647", true,true},
            {(uint32)-1,"4294967295",true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertCArray(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt64ToCArray) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,const char8 *> table[]={
            {0,"0",true,true},
            {123, "123", true,true},
            {-1,"-1", true,true},
            {9223372036854775807,"9223372036854775807", true,true},
            {(int64)9223372036854775808ul,"-9223372036854775808", true,true},
            {(int64)-9223372036854775808ul,"-9223372036854775808", true,true},
            {(int64)-9223372036854775809ul,"9223372036854775807", true,true},
            {(int64)18446744073709551615ul,"-1", true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertCArray(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt64ToCArray) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,const char8 *> table[]={
            {123ULL,"123", true,true},
            {18446744073709551615ULL,"18446744073709551615", true,true},
            {0ULL,"0", true,true},
            {9223372036854775808ULL,"9223372036854775808", true,true},
            {-1ULL,"18446744073709551615",true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertCArray(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,float32ToCArray) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,const char8 *> table[]={
            {123.5,"123.5000", true,true},
            {123.12345,"123.1234", true,true},
            {123.123456,"123.1235", true,true},
            {100,"100.0000", true,true},
            {10E+2,"1.000000E+3", true,true},
            {12345.678E+2,"1.234568E+6", true,true},
            {1E-2,"10.00000E-3", true,true},
            {0.1E-2,"1.000000E-3", true,true},
            {16E-2,"160.0000E-3", true,true},
            {1E+20,"100.0000E+18", true,true},
            {1E-7,"100.0000E-9", true,true},
            {1E-9,"1.000000E-9", true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertCArray(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat64ToCArray) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,const char8 *> table[]={
            {123.5,"123.500000000000", true,true},
            {123.12345,"123.123450000000", true,true},
            {123.123456,"123.123456000000", true,true},
            {100,"100.000000000000", true,true},
            {10E+2,"1.00000000000000E+3", true,true},
            {12345.678E+2,"1.23456780000000E+6", true,true},
            {1E-2,"10.0000000000000E-3", true,true},
            {0.1E-2,"1.00000000000000E-3", true,true},
            {16E-2,"160.000000000000E-3", true,true},
            {123E+63,"123.000000000000E+63", true,true},
            {123E+83,"12.3000000000000E+84", true,true},
            {1E-7,"100.000000000000E-9", true,true},
            {1E-9,"1.00000000000000E-9", true,true},
            {1E-25,"100.000000000000E-27", true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertCArray(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToCArray) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,const char8*> table[]={
            {"Hello","Hello", true, true},
            {"123.12345","123.12345", true, true},
            {"","", true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertCArray(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToCArray) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<StreamString,const char8*> table[]={
            {"Hello","Hello", true, true},
            {"123.12345","123.12345", true, true},
            {"","", true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertCArray(table));
}
/////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt8ToCharVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int8,const char8 *> table[]={
            {0,"0",true,true},
            {123, "123", true,true},
            {-1,"-1", true,true},
            {127,"127", true,true},
            {-128,"-128", true,true},
            {(int8)255,"-1",true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCharVector(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt8ToCharVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint8,const char8 *> table[]={
            {123,"123", true,true},
            {255,"255", true,true},
            {127,"127", true,true},
            {(uint8)-1,"255",true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCharVector(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt16ToCharVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int16,const char8 *> table[]={
            {0,"0",true,true},
            {123, "123", true,true},
            {-1,"-1", true,true},
            {32767,"32767", true,true},
            {-32768,"-32768", true,true},
            {(int16)65535,"-1", true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCharVector(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt16ToCharVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint16,const char8 *> table[]={
            {123,"123", true,true},
            {65535,"65535", true,true},
            {(uint16)65536,"0", true,true},
            {32767,"32767", true,true},
            {(uint16)-1,"65535",true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCharVector(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToCharVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int32,const char8 *> table[]={
            {0,"0",true,true},
            {123, "123", true,true},
            {-1,"-1", true,true},
            {2147483647,"2147483647", true,true},
            {(int32)2147483648,"-2147483648", true,true},
            {-2147483648,"-2147483648", true,true},
            {(int32)-2147483649,"2147483647", true,true},
            {(int32)4294967295,"-1", true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCharVector(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToCharVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint32,const char8 *> table[]={
            {123,"123", true,true},
            {4294967295,"4294967295", true,true},
            {(uint32)4294967296,"0", true,true},
            {2147483647,"2147483647", true,true},
            {(uint32)-1,"4294967295",true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCharVector(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt64ToCharVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<int64,const char8 *> table[]={
            {0,"0",true,true},
            {123, "123", true,true},
            {-1,"-1", true,true},
            {9223372036854775807,"9223372036854775807", true,true},
            {(int64)9223372036854775808ul,"-9223372036854775808", true,true},
            {(int64)-9223372036854775808ul,"-9223372036854775808", true,true},
            {(int64)-9223372036854775809ul,"9223372036854775807", true,true},
            {(int64)18446744073709551615ul,"-1", true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCharVector(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt64ToCharVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<uint64,const char8 *> table[]={
            {123ULL,"123", true,true},
            {18446744073709551615ULL,"18446744073709551615", true,true},
            {0ULL,"0", true,true},
            {9223372036854775808ULL,"9223372036854775808", true,true},
            {-1ULL,"18446744073709551615",true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCharVector(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,float32ToCharVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,const char8 *> table[]={
            {123.5,"123.5000", true,true},
            {123.12345,"123.1234", true,true},
            {123.123456,"123.1235", true,true},
            {100,"100.0000", true,true},
            {10E+2,"1.000000E+3", true,true},
            {12345.678E+2,"1.234568E+6", true,true},
            {1E-2,"10.00000E-3", true,true},
            {0.1E-2,"1.000000E-3", true,true},
            {16E-2,"160.0000E-3", true,true},
            {1E+20,"100.0000E+18", true,true},
            {1E-7,"100.0000E-9", true,true},
            {1E-9,"1.000000E-9", true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCharVector(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat64ToCharVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float64,const char8 *> table[]={
            {123.5,"123.500000000000", true,true},
            {123.12345,"123.123450000000", true,true},
            {123.123456,"123.123456000000", true,true},
            {100,"100.000000000000", true,true},
            {10E+2,"1.00000000000000E+3", true,true},
            {12345.678E+2,"1.23456780000000E+6", true,true},
            {1E-2,"10.0000000000000E-3", true,true},
            {0.1E-2,"1.00000000000000E-3", true,true},
            {16E-2,"160.000000000000E-3", true,true},
            {123E+63,"123.000000000000E+63", true,true},
            {123E+83,"12.3000000000000E+84", true,true},
            {1E-7,"100.000000000000E-9", true,true},
            {1E-9,"1.00000000000000E-9", true,true},
            {1E-25,"100.000000000000E-27", true,true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCharVector(table));
}




TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToCharVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,const char8*> table[]={
            {"Hello","Hello", true, true},
            {"123.12345","123.12345", true, true},
            {"","", true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCharVector(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToCharVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<StreamString,const char8*> table[]={
            {"Hello","Hello", true, true},
            {"123.12345","123.12345", true, true},
            {"","", true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertToCharVector(table));
}

//////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCArrayToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,int8> table[]={
            {"123",123, true,true},
            {"123s",123, true,false},
            {"a123s5",0, true,false},
            {"0xFF",-1, true,true},
            {"0xFF1",-1, true,false},
            {"0xABCD",(int8)0xab, true,false},
            {"0o12",10, true,true},
            {"0x7F",127, true,true},
            {"0x7F1",127, true,false},
            {"0x80",-128, true,true},
            {"0o377",-1, true,true},
            {"0o577",47, true,false},
            {"0b11111111",-1, true,true},
            {"0b111111111",-1, true,false},
            {"0b01111111",127, true,true},
            {"0b011111111",127, true,false},
            {"127",127, true,true},
            {"128",12, true,false},
            {"-128",-128, true,true},
            {"-129",-12, true,false},
            {0,0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertFromCArray(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCArrayToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,uint8> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFF",255, true, true},
            {"0xFF1",255, true, false},
            {"0xABCD",0xab, true, false},
            {"0o12",10, true, true},
            {"0x7F",127, true, true},
            {"0x80",128, true, true},
            {"0o377",255, true, true},
            {"0o577",47, true, false},
            {"0b11111111",255, true, true},
            {"0b111111111",255, true, false},
            {"0b01111111",127, true, true},
            {"0b011111111",127, true, false},
            {"255",255, true, true},
            {"256",25, true, false},
            {"-1",0, true, false},
            {0,0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertFromCArray(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCArrayToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,int16> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFF",-1, true, true},
            {"0xFFFF1",-1, true, false},
            {"0x7FFF",32767, true, true},
            {"0o177777",-1, true, true},
            {"0o77777",32767, true, true},
            {"0o277777",12287, true, false},
            {"0b1111111111111111",-1, true, true},
            {"0b11111111111111111",-1, true, false},
            {"0b0111111111111111",32767, true, true},
            {"0b01111111111111111",32767, true, false},
            {"32767",32767, true, true},
            {"32768",3276, true, false},
            {"-32768",-32768, true, true},
            {"-32769",-3276, true, false},
            {0,0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertFromCArray(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCArrayToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,uint16> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFF",65535, true, true},
            {"0xFFFF1",65535, true, false},
            {"0x7FFF",32767, true, true},
            {"0o177777",65535, true, true},
            {"0o77777",32767, true, true},
            {"0o277777",12287, true, false},
            {"0b1111111111111111",65535, true, true},
            {"0b11111111111111111",65535, true, false},
            {"32767",32767, true, true},
            {"65536",6553, true, false},
            {"-1",0, true, false},
            {0,0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertFromCArray(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCArrayToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,int32> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFFFFFF",-1, true, true},
            {"0xFFFFFFFF1",-1, true, false},
            {"0x7FFFFFFF",2147483647, true, true},
            {"0o37777777777",-1, true, true},
            {"0o17777777777",2147483647, true, true},
            {"0o47777777777",671088639, true, false},
            {"2147483647",2147483647, true, true},
            {"2147483648",214748364, true, false},
            {"0b11111111111111111111111111111111",-1, true, true},
            {"0b111111111111111111111111111111111",-1, true, false},
            {"0b01111111111111111111111111111111",2147483647, true, true},
            {"0b011111111111111111111111111111111",2147483647, true, false},
            {"-2147483648",-2147483648, true, true},
            {"-2147483649",-214748364, true, false},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertFromCArray(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCArrayToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,uint32> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFFFFFF",4294967295, true, true},
            {"0xFFFFFFFF1",4294967295, true, false},
            {"0x7FFFFFFF",2147483647, true, true},
            {"0o37777777777",4294967295, true, true},
            {"0o17777777777",2147483647, true, true},
            {"0o47777777777",671088639, true, false},
            {"0b11111111111111111111111111111111",4294967295, true, true},
            {"0b111111111111111111111111111111111",4294967295, true, false},
            {"4294967295",4294967295, true, true},
            {"4294967296",429496729, true, false},
            {"-1",0, true, false},
            {0,0, 0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertFromCArray(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCArrayToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,int64> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFFFFFFFFFFFFFF",-1, true, true},
            {"0xFFFFFFFFFFFFFFFF1",-1, true, false},
            {"0x7FFFFFFFFFFFFFFF",9223372036854775807, true, true},
            {"0o1777777777777777777777",-1, true, true},
            {"0o777777777777777777777",9223372036854775807, true, true},
            {"0o2777777777777777777777",3458764513820540927, true, false},
            {"0b1111111111111111111111111111111111111111111111111111111111111111",-1, true, true},
            {"0b11111111111111111111111111111111111111111111111111111111111111111",-1, true, false},
            {"0b0111111111111111111111111111111111111111111111111111111111111111",9223372036854775807, true, true},
            {"0b01111111111111111111111111111111111111111111111111111111111111111",9223372036854775807, true, false},
            {"9223372036854775807",9223372036854775807, true, true},
            {"9223372036854775808",922337203685477580, true, false},
            {"-9223372036854775808",(int64)-9223372036854775808ul, true, true},
            {"-9223372036854775809",(int64)-922337203685477580, true, false},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertFromCArray(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCArrayToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,uint64> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFFFFFFFFFFFFFF",18446744073709551615ul, true, true},
            {"0xFFFFFFFFFFFFFFFF1",18446744073709551615ul, true, false},
            {"0x7FFFFFFfFFFFFFFF",9223372036854775807ul, true, true},
            {"0o1777777777777777777777",18446744073709551615ul, true, true},
            {"0o777777777777777777777",9223372036854775807ul, true, true},
            {"0o2777777777777777777777",3458764513820540927ul, true, false},
            {"0b1111111111111111111111111111111111111111111111111111111111111111",18446744073709551615ul, true, true},
            {"0b11111111111111111111111111111111111111111111111111111111111111111",18446744073709551615ul, true, false},
            {"0b0111111111111111111111111111111111111111111111111111111111111111",9223372036854775807ul, true, true},
            {"0b01111111111111111111111111111111111111111111111111111111111111111",9223372036854775807ul, true, false},
            {"18446744073709551615",18446744073709551615ul, true, true},
            {"18446744073709551616",1844674407370955161ul, true, false},
            {"-1",0, true, false},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertFromCArray(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCArrayToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,float32> table[]={
            {"123.5",123.5, true, true},
            {"123.12345",123.12345, true, true},
            {"1E+2",100, true,true},
            {"10E+2",1000, true,true},
            {"12345.678E2",1234567.8, true,true},
            {"1E-2",0.01, true,true},
            {"0.1E-2",0.001, true,true},
            {"16E-2",0.16, true,true},
            // see the precision is not the same!
            {"12345.678E-2",123.456772, true,true},
            {"1E+20",1E+20, true,true},
            // this is overflow!
            {"123E+42",123, true,false},
            {"1E-25",1E-25, true,true},
            {"0.000000001",1E-9, true,true},
            //Overflow!
     //       {"340282346638528859811704183484516999999",340282346638528859811704183484516925440},
     //       {"340282346638528859811704183484516925440",340282346638528859811704183484516925440},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertFromCArray(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCArrayToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,float64> table[]={
            {"123.5",123.5, true, true},
            {"123.12345",123.12345, true, true},
            {"1E+2",100, true, true},
            {"10E+2",1000, true, true},
            {"12345.678E2",1234567.8, true, true},
            {"1E-2",0.01, true, true},
            {"0.1E-2",0.001, true, true},
            {"16E-2",0.16, true, true},
            // see the precision now is the same!
            {"12345.678E-2",123.45678, true, true},
            {"1E+20",1E+20, true, true},
            {"123E+63",123E+63, true, true},
            {"123E+83",123E+83, true, true},
            // this is overflow!
            {"123E+510",123, true, false},
            {"1E-32",1E-32, true, true},
            {"0.000000001",1E-9, true, true},
            //Overflow!
     //       {"179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368",179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368},
     //       {"179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368",179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertFromCArray(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCArrayToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,StreamString> table[]={
            {"Hello","Hello", true, true},
            {"123.12345","123.12345", true, true},
            {"","", true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertFromCArray(table));
}


//////////////////////////////////////////////////////////////

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCharVectorToInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,int8> table[]={
            {"123",123, true,true},
            {"123s",123, true,false},
            {"a123s5",0, true,false},
            {"0xFF",-1, true,true},
            {"0xFF1",-1, true,false},
            {"0xABCD",(int8)0xab, true,false},
            {"0o12",10, true,true},
            {"0x7F",127, true,true},
            {"0x7F1",127, true,false},
            {"0x80",(int8)-128, true,true},
            {"0o377",-1, true,true},
            {"0o577",47, true,false},
            {"0b11111111",-1, true,true},
            {"0b111111111",-1, true,false},
            {"0b01111111",127, true,true},
            {"0b011111111",127, true,false},
            {"127",127, true,true},
            {"128",12, true,false},
            {"-128",(int8)-128, true,true},
            {"-129",-12, true,false},
            {0,0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertFromCharVector(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCharVectorToUInt8) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,uint8> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFF",255, true, true},
            {"0xFF1",255, true, false},
            {"0xABCD",0xab, true, false},
            {"0o12",10, true, true},
            {"0x7F",127, true, true},
            {"0x80",128, true, true},
            {"0o377",255, true, true},
            {"0o577",47, true, false},
            {"0b11111111",255, true, true},
            {"0b111111111",255, true, false},
            {"0b01111111",127, true, true},
            {"0b011111111",127, true, false},
            {"255",255, true, true},
            {"256",25, true, false},
            {"-1",0, true, false},
            {0,0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertFromCharVector(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCharVectorToInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,int16> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFF",-1, true, true},
            {"0xFFFF1",-1, true, false},
            {"0x7FFF",32767, true, true},
            {"0o177777",-1, true, true},
            {"0o77777",32767, true, true},
            {"0o277777",12287, true, false},
            {"0b1111111111111111",-1, true, true},
            {"0b11111111111111111",-1, true, false},
            {"0b0111111111111111",32767, true, true},
            {"0b01111111111111111",32767, true, false},
            {"32767",32767, true, true},
            {"32768",3276, true, false},
            {"-32768",-32768, true, true},
            {"-32769",-3276, true, false},
            {0,0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertFromCharVector(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCharVectorToUInt16) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,uint16> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFF",65535, true, true},
            {"0xFFFF1",65535, true, false},
            {"0x7FFF",32767, true, true},
            {"0o177777",65535, true, true},
            {"0o77777",32767, true, true},
            {"0o277777",12287, true, false},
            {"0b1111111111111111",65535, true, true},
            {"0b11111111111111111",65535, true, false},
            {"32767",32767, true, true},
            {"65536",6553, true, false},
            {"-1",0, true, false},
            {0,0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertFromCharVector(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCharVectorToInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,int32> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFFFFFF",-1, true, true},
            {"0xFFFFFFFF1",-1, true, false},
            {"0x7FFFFFFF",2147483647, true, true},
            {"0o37777777777",-1, true, true},
            {"0o17777777777",2147483647, true, true},
            {"0o47777777777",671088639, true, false},
            {"2147483647",2147483647, true, true},
            {"2147483648",214748364, true, false},
            {"0b11111111111111111111111111111111",-1, true, true},
            {"0b111111111111111111111111111111111",-1, true, false},
            {"0b01111111111111111111111111111111",2147483647, true, true},
            {"0b011111111111111111111111111111111",2147483647, true, false},
            {"-2147483648",-2147483648, true, true},
            {"-2147483649",-214748364, true, false},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertFromCharVector(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCharVectorToUInt32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,uint32> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFFFFFF",4294967295, true, true},
            {"0xFFFFFFFF1",4294967295, true, false},
            {"0x7FFFFFFF",2147483647, true, true},
            {"0o37777777777",4294967295, true, true},
            {"0o17777777777",2147483647, true, true},
            {"0o47777777777",671088639, true, false},
            {"0b11111111111111111111111111111111",4294967295, true, true},
            {"0b111111111111111111111111111111111",4294967295, true, false},
            {"4294967295",4294967295, true, true},
            {"4294967296",429496729, true, false},
            {"-1",0, true, false},
            {0,0, 0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertFromCharVector(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCharVectorToInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,int64> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFFFFFFFFFFFFFF",-1, true, true},
            {"0xFFFFFFFFFFFFFFFF1",-1, true, false},
            {"0x7FFFFFFFFFFFFFFF",9223372036854775807, true, true},
            {"0o1777777777777777777777",-1, true, true},
            {"0o777777777777777777777",9223372036854775807, true, true},
            {"0o2777777777777777777777",3458764513820540927, true, false},
            {"0b1111111111111111111111111111111111111111111111111111111111111111",-1, true, true},
            {"0b11111111111111111111111111111111111111111111111111111111111111111",-1, true, false},
            {"0b0111111111111111111111111111111111111111111111111111111111111111",9223372036854775807, true, true},
            {"0b01111111111111111111111111111111111111111111111111111111111111111",9223372036854775807, true, false},
            {"9223372036854775807",9223372036854775807, true, true},
            {"9223372036854775808",922337203685477580, true, false},
            {"-9223372036854775808",(int64)-9223372036854775808ul, true, true},
            {"-9223372036854775809",(int64)-922337203685477580, true, false},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertFromCharVector(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCharVectorToUInt64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,uint64> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFFFFFFFFFFFFFF",18446744073709551615ul, true, true},
            {"0xFFFFFFFFFFFFFFFF1",18446744073709551615ul, true, false},
            {"0x7FFFFFFFFFFFFFFF",9223372036854775807ul, true, true},
            {"0o1777777777777777777777",18446744073709551615ul, true, true},
            {"0o777777777777777777777",9223372036854775807ul, true, true},
            {"0o2777777777777777777777",3458764513820540927ul, true, false},
            {"0b1111111111111111111111111111111111111111111111111111111111111111",18446744073709551615ul, true, true},
            {"0b11111111111111111111111111111111111111111111111111111111111111111",18446744073709551615ul, true, false},
            {"0b0111111111111111111111111111111111111111111111111111111111111111",9223372036854775807ul, true, true},
            {"0b01111111111111111111111111111111111111111111111111111111111111111",9223372036854775807ul, true, false},
            {"18446744073709551615",18446744073709551615ul, true, true},
            {"18446744073709551616",1844674407370955161ul, true, false},
            {"-1",0, true, false},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertFromCharVector(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCharVectorToFloat32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,float32> table[]={
            {"123.5",123.5, true, true},
            {"123.12345",123.12345, true, true},
            {"1E+2",100, true,true},
            {"10E+2",1000, true,true},
            {"12345.678E2",1234567.8, true,true},
            {"1E-2",0.01, true,true},
            {"0.1E-2",0.001, true,true},
            {"16E-2",0.16, true,true},
            // see the precision is not the same!
            {"12345.678E-2",123.456772, true,true},
            {"1E+20",1E+20, true,true},
            // this is overflow!
            {"123E+42",123, true,false},
            {"1E-25",1E-25, true,true},
            {"0.000000001",1E-9, true,true},
            //Overflow!
     //       {"340282346638528859811704183484516999999",340282346638528859811704183484516925440},
     //       {"340282346638528859811704183484516925440",340282346638528859811704183484516925440},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertFromCharVector(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCharVectorToFloat64) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,float64> table[]={
            {"123.5",123.5, true, true},
            {"123.12345",123.12345, true, true},
            {"1E+2",100, true, true},
            {"10E+2",1000, true, true},
            {"12345.678E2",1234567.8, true, true},
            {"1E-2",0.01, true, true},
            {"0.1E-2",0.001, true, true},
            {"16E-2",0.16, true, true},
            // see the precision now is the same!
            {"12345.678E-2",123.45678, true, true},
            {"1E+20",1E+20, true, true},
            {"123E+63",123E+63, true, true},
            {"123E+83",123E+83, true, true},
            // this is overflow!
            {"123E+510",123, true, false},
            {"1E-32",1E-32, true, true},
            {"0.000000001",1E-9, true, true},
            //Overflow!
     //       {"179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368",179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368},
     //       {"179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368",179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertFromCharVector(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCharVectorToSString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,StreamString> table[]={
            {"Hello","Hello", true, true},
            {"123.12345","123.12345", true, true},
            {"","", true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertFromCharVector(table));
}




/////////////////////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToCArrayVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<int32,const char8*,3> table[]={
            {{(int32)2147483648,(int32)4294967295,2147483647},{"-2147483648","-1","2147483647"}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVectorCArray(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToCArrayVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<uint32,const char8 *,3> table[]={
            {{(uint32)-1,2147483647,(uint32)4294967295},{"4294967295","2147483647","4294967295"}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVectorCArray(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToCArrayVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<float32,const char8 *,3> table[]={
            {{-0.02E-2,1234.5E+5,1E-5},{"-200.0000E-6","123.4500E+6","10.00000E-6"}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVectorCArray(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToCArrayVector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<const char8*,const char8 *,3> table[]={
            {{"-200E-6","123.4500E+6","10000000000000000000000000000E-6"},{"-200E-6","123.4500E+6","10000000000000000000000000000E-6"}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVectorCArray(table));
}





TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCArrayToInt32Vector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<const char8 *,int32,3> table[]={
            {{"1","0","2147483647"},{1,0,2147483647}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVectorFromCArray(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCArrayToUInt32Vector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<const char8 *,uint32,3> table[]={
            {{"-1","2147483647","4294967295"},{0,2147483647,4294967295}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVectorFromCArray(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCArrayToFloat32Vector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<const char8 *,float32,3> table[]={
            {{"-0.02E-2","1234.5E+5","0.00001"},{-0.02E-2,1234.5E+5,1E-5}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVectorFromCArray(table));
}

///////////////////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32VectorToCharMatrix) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<int32,const char8*,3> table[]={
            {{(int32)2147483648,(int32)4294967295,2147483647},{"-2147483648","-1","2147483647"}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVectorToCharMatrix(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32VectorToCharMatrix) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<uint32,const char8 *,3> table[]={
            {{(uint32)-1,2147483647,4294967295},{"4294967295","2147483647","4294967295"}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVectorToCharMatrix(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32VectorToCharMatrix) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<float32,const char8 *,3> table[]={
            {{-0.02E-2,1234.5E+5,1E-5},{"-200.0000E-6","123.4500E+6","10.00000E-6"}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVectorToCharMatrix(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringVectorToCharMatrix) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<const char8*,const char8 *,3> table[]={
            {{"-200E-6","123.4500E+6","10000000000000000000000000000E-6"},{"-200E-6","123.4500E+6","10000000000000000000000000000E-6"}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVectorToCharMatrix(table));
}





TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCharMatrixToInt32Vector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<const char8 *,int32,3> table[]={
            {{"1","0","2147483647"},{1,0,2147483647}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVectorFromCharMatrix(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCharMatrixToUInt32Vector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<const char8 *,uint32,3> table[]={
            {{"-1","2147483647","4294967295"},{0,2147483647,4294967295}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVectorFromCharMatrix(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCharMatrixToFloat32Vector) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<const char8 *,float32,3> table[]={
            {{"-0.02E-2","1234.5E+5","0.00001"},{-0.02E-2,1234.5E+5,1E-5}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVectorFromCharMatrix(table));
}


///////////////////////////////////////////////////////////

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToCArrayMatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<float32,const char8*,2,2> table[]={
            {{{-0.02E-2,1234.5E+5},{1E-5,0.123}},{{"-200.0000E-6","123.4500E+6"},{"10.00000E-6","123.0000E-3"}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStaticCArray(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCArrayToFloat32MatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8 *,float32,2,2> table[]={
            {{{"-0.02E-2","1234.5E+5"},{"0.00001","1E-20"}},{{-0.02E-2,1234.5E+5},{1E-5,1E-20}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStaticFromCArray(table));
}

/////////////////////////

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestTypeConvertVectorStaticToHeapMatrixCArray) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<int32,const char8*,3> table[]={
            {{(int32)2147483648,(int32)4294967295,2147483647},{"-2147483648","-1","2147483647"}, true},
            {{0},{0},0}
    };


    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertVectorStaticToHeapMatrixCArray(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCArrayToFloat32MatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8 *,float32,2,2> table[]={
            {{{"-0.02E-2","1234.5E+5"},{"0.00001","1E-20"}},{{-0.02E-2,1234.5E+5},{1E-5,1E-20}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeapFromCArray(table));
}
//////////////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToCCStringMatrixHeapToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<int32,const char8*,2,2> table[]={
            {{{(int32)2147483648,(int32)4294967295},{2147483647,0}},{{"-2147483648","-1"},{"2147483647","0"}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToHeapCCString(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToCCStringMatrixHeapToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<uint32,const char8*,2,2> table[]={
            {{{(uint32)-1,2147483647},{4294967295,(uint32)4294967296}},{{"4294967295","2147483647"},{"4294967295","0"}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToHeapCCString(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToCCStringMatrixHeapToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<float32,const char8*,2,2> table[]={
            {{{-0.02E-2,1234.5E+5},{1E-5,0.123}},{{"-200.0000E-6","123.4500E+6"},{"10.00000E-6","123.0000E-3"}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToHeapCCString(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToCCStringMatrixHeapToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8*,const char8*,2,3> table[]={
            {{{"-200E-6","123.4500E+6","abcd"},{"10000000000000000000000000000E-6","HelloWorld","efgh"}},{{"-200E-6","123.4500E+6","abcd"},{"10000000000000000000000000000E-6","HelloWorld","efgh"}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToHeapCCString(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToCCStringMatrixHeapToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<StreamString,const char8*,2,3> table[]={
            {{{"-200E-6","123.4500E+6","abcd"},{"10000000000000000000000000000E-6","HelloWorld","efgh"}},{{"-200E-6","123.4500E+6","abcd"},{"10000000000000000000000000000E-6","HelloWorld","efgh"}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToHeapCCString(table));
}

///////////////////////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToCCStringMatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<int32,const char8*,2,2> table[]={
            {{{(int32)2147483648,(int32)4294967295},{2147483647,0}},{{"-2147483648","-1"},{"2147483647","0"}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeapCCString(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToCCStringMatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<uint32,const char8*,2,2> table[]={
            {{{(uint32)-1,2147483647},{4294967295,(uint32)4294967296}},{{"4294967295","2147483647"},{"4294967295","0"}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeapCCString(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToCCStringMatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<float32,const char8*,2,2> table[]={
            {{{-0.02E-2,1234.5E+5},{1E-5,0.123}},{{"-200.0000E-6","123.4500E+6"},{"10.00000E-6","123.0000E-3"}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeapCCString(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToCCStringMatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8*,const char8*,2,3> table[]={
            {{{"-200E-6","123.4500E+6","abcd"},{"10000000000000000000000000000E-6","HelloWorld","efgh"}},{{"-200E-6","123.4500E+6","abcd"},{"10000000000000000000000000000E-6","HelloWorld","efgh"}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeapCCString(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToCCStringMatrixStaticToHeap) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<StreamString,const char8*,2,3> table[]={
            {{{"-200E-6","123.4500E+6","abcd"},{"10000000000000000000000000000E-6","HelloWorld","efgh"}},{{"-200E-6","123.4500E+6","abcd"},{"10000000000000000000000000000E-6","HelloWorld","efgh"}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToHeapCCString(table));
}


///////////////////////////////////////////////////////


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToCCStringMatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<int32,const char8*,2,2> table[]={
            {{{(int32)2147483648,(int32)4294967295},{(int32)2147483647,(int32)0}},{{"-2147483648","-1"},{"2147483647","0"}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStaticCCString(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToCCStringMatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<uint32,const char8*,2,2> table[]={
            {{{(uint32)-1,2147483647},{4294967295,(uint32)4294967296}},{{"4294967295","2147483647"},{"4294967295","0"}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStaticCCString(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToCCStringMatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<float32,const char8*,2,2> table[]={
            {{{-0.02E-2,1234.5E+5},{1E-5,0.123}},{{"-200.0000E-6","123.4500E+6"},{"10.00000E-6","123.0000E-3"}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStaticCCString(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToCCStringMatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8*,const char8*,2,3> table[]={
            {{{"-200E-6","123.4500E+6","abcd"},{"10000000000000000000000000000E-6","HelloWorld","efgh"}},{{"-200E-6","123.4500E+6","abcd"},{"10000000000000000000000000000E-6","HelloWorld","efgh"}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStaticCCString(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToCCStringMatrixStaticToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<StreamString,const char8*,2,3> table[]={
            {{{"-200E-6","123.4500E+6","abcd"},{"10000000000000000000000000000E-6","HelloWorld","efgh"}},{{"-200E-6","123.4500E+6","abcd"},{"10000000000000000000000000000E-6","HelloWorld","efgh"}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixStaticToStaticCCString(table));
}

//////////////////////////////////////////////////////////////////////

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestInt32ToCCStringMatrixHeapToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<int32,const char8*,2,2> table[]={
            {{{(int32)2147483648,(int32)4294967295},{2147483647,0}},{{"-2147483648","-1"},{"2147483647","0"}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToStaticCCString(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestUInt32ToCCStringMatrixHeapToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<uint32,const char8*,2,2> table[]={
            {{{(uint32)-1,2147483647},{4294967295,(uint32)4294967296}},{{"4294967295","2147483647"},{"4294967295","0"}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToStaticCCString(table));
}



TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloat32ToCCStringMatrixHeapToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<float32,const char8*,2,2> table[]={
            {{{-0.02E-2,1234.5E+5},{1E-5,0.123}},{{"-200.0000E-6","123.4500E+6"},{"10.00000E-6","123.0000E-3"}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToStaticCCString(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToCCStringMatrixHeapToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8*,const char8*,2,3> table[]={
            {{{"-200E-6","123.4500E+6","abcd"},{"10000000000000000000000000000E-6","HelloWorld","efgh"}},{{"-200E-6","123.4500E+6","abcd"},{"10000000000000000000000000000E-6","HelloWorld","efgh"}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToStaticCCString(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestSStringToCCStringMatrixHeapToStatic) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<StreamString,const char8*,2,3> table[]={
            {{{"-200E-6","123.4500E+6","abcd"},{"10000000000000000000000000000E-6","HelloWorld","efgh"}},{{"-200E-6","123.4500E+6","abcd"},{"10000000000000000000000000000E-6","HelloWorld","efgh"}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertMatrixHeapToStaticCCString(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCArrayToCCStringScalar) {
    TypeConversionTest myTypeConversionTest;

    ASSERT_TRUE(myTypeConversionTest.TestCArrayToCCStringScalar());
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCArrayToCCStringVector) {
    TypeConversionTest myTypeConversionTest;

    ASSERT_TRUE(myTypeConversionTest.TestCArrayToCCStringVector());
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCArrayToCCStringMatrix) {
    TypeConversionTest myTypeConversionTest;

    ASSERT_TRUE(myTypeConversionTest.TestCArrayToCCStringMatrix());
}

//TODO this is currently not supported in msc
#if ENVIRONMENT != Windows
TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToCArrayScalar_Trunc) {
    TypeConversionTest myTypeConversionTest;

    ASSERT_TRUE(myTypeConversionTest.TestCCStringToCArrayScalar_Trunc());
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToCArrayVector_Trunc) {
    TypeConversionTest myTypeConversionTest;

    ASSERT_TRUE(myTypeConversionTest.TestCCStringToCArrayVector_Trunc());
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestCCStringToCArrayMatrix_Trunc) {
    TypeConversionTest myTypeConversionTest;

    ASSERT_TRUE(myTypeConversionTest.TestCCStringToCArrayMatrix_Trunc());
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestIntegerToCArrayScalar_Trunc) {
    TypeConversionTest myTypeConversionTest;

    ASSERT_TRUE(myTypeConversionTest.TestIntegerToCArrayScalar_Trunc());
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestFloatToCArrayScalar_Trunc) {
    TypeConversionTest myTypeConversionTest;

    ASSERT_TRUE(myTypeConversionTest.TestFloatToCArrayScalar_Trunc());
}
#endif


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestObjectToObject) {
    TypeConversionTest myTypeConversionTest;

    ASSERT_TRUE(myTypeConversionTest.TestObjectToObject());
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestObjectToObject_Reverse) {
    TypeConversionTest myTypeConversionTest;

    ASSERT_TRUE(myTypeConversionTest.TestObjectToObject_Reverse());
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestObjectToObject_ErrorNoSourceIntrospection) {
    TypeConversionTest myTypeConversionTest;

    ASSERT_TRUE(myTypeConversionTest.TestObjectToObject_ErrorNoSourceIntrospection());
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestObjectToObject_ErrorNoDestIntrospection) {
    TypeConversionTest myTypeConversionTest;

    ASSERT_TRUE(myTypeConversionTest.TestObjectToObject_ErrorNoDestIntrospection());
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestObjectToObject_NoCompatibility) {
    TypeConversionTest myTypeConversionTest;

    ASSERT_TRUE(myTypeConversionTest.TestObjectToObject_NoCompatibility());
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestStructuredDataToObject_SourceIntrospection) {
    TypeConversionTest myTypeConversionTest;

    ASSERT_TRUE(myTypeConversionTest.TestStructuredDataToObject_SourceIntrospection());
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestStructuredDataToObject_NoSourceIntrospection) {
    TypeConversionTest myTypeConversionTest;

    ASSERT_TRUE(myTypeConversionTest.TestStructuredDataToObject_NoSourceIntrospection());
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestStructuredDataToObject_ErrorNoDestIntrospection) {
    TypeConversionTest myTypeConversionTest;

    ASSERT_TRUE(myTypeConversionTest.TestStructuredDataToObject_ErrorNoDestIntrospection());
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestStructuredDataToObject_NoCompatibility) {
    TypeConversionTest myTypeConversionTest;

    ASSERT_TRUE(myTypeConversionTest.TestStructuredDataToObject_NoCompatibility());
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestObjectToStructuredData) {
    TypeConversionTest myTypeConversionTest;

    ASSERT_TRUE(myTypeConversionTest.TestObjectToStructuredData());
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestObjectArrayToStructuredData) {
    TypeConversionTest myTypeConversionTest;

    ASSERT_TRUE(myTypeConversionTest.TestObjectArrayToStructuredData());
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestStructuredDataToStructuredData) {
    TypeConversionTest myTypeConversionTest;

    ASSERT_TRUE(myTypeConversionTest.TestStructuredDataToStructuredData());
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestStructuredDataToObject_SourceIntrospectionArray) {
    TypeConversionTest myTypeConversionTest;

    ASSERT_TRUE(myTypeConversionTest.TestStructuredDataToObject_SourceIntrospectionArray());
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestTypeConvertPointer_CString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<const char8*,uint32> table[]={
            {"123",123, true, true},
            {"123s1",123, true, false},
            {"a123s5",0, true, false},
            {"0xFFFFFFFF",4294967295, true, true},
            {"0xFFFFFFFF1",4294967295, true, false},
            {"0x7FFFFFFF",2147483647, true, true},
            {"0o37777777777",4294967295, true, true},
            {"0o17777777777",2147483647, true, true},
            {"0o47777777777",671088639, true, false},
            {"0b11111111111111111111111111111111",4294967295, true, true},
            {"0b111111111111111111111111111111111",4294967295, true, false},
            {"4294967295",4294967295, true, true},
            {"4294967296",429496729, true, false},
            {"10000000000",1000000000, true, false},
            {"-1",0, true, false},
            {0,0, 0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertPointer(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestTypeConvertPointer_Int32) {
        TypeConversionTest myTypeConversionTest;
        const TypeToTypeTableTest<int32,uint32> table[]={
                {1,1, true, true},
                {2147483647,2147483647,true, true},
                {-1, 0, true, true},
                {0,0,0}
        };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertPointer(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestTypeConvertPointer_Float32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeTableTest<float32,int64> table[]={
            {1.9,2, true,true},
            {99223372036854775809.0f, 9223372036854775807, true, true},
            {99223372036854775807.0f, 9223372036854775807, true, true},
            // precision lost!!
            {9223372036854775807.0f, 9223372036854775807, true, true},
            // precision lost!!
            {9223372036854775806.5f, 9223372036854775807, true, true},
            {-1.0, -1, true,true},
            // precision lost!!
            {-9223372036854775807.0, (int64)-9223372036854775808ull,true, true},
            // precision lost!!
            {-9223372036854775807.5,(int64)-9223372036854775808ull, true, true},
            {-9223372036854775808.0, (int64)-9223372036854775808ull, true, true},
            {0,0,0}
    };
    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertPointer(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestTypeConvertPointerVector_CString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<const char8 *,int32,3> table[]={
            {{"1","0","2147483647"},{1,0,2147483647}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertPointerVector(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestTypeConvertPointerVector_Int32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<uint32,int32,3> table[]={
            {{2147483647,0,2147483648},{2147483647,0,2147483647}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertPointerVector(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestTypeConvertPointerVector_Float32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeVectorTableTest<float32,uint32,3> table[]={
            {{123.45,-1.2,123.5},{123,0,124}, true},
            {{0},{0},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertPointerVector(table));
}


TEST(BareMetal_L4Configuration_TypeConversionGTest,TestTypeConvertPointerMatrix_CString) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<const char8 *,float32,2,2> table[]={
            {{{"-0.02E-2","1234.5E+5"},{"0.00001","1E-20"}},{{-0.02E-2,1234.5E+5},{1E-5,1E-20}}, true},
            {{{0},{0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertPointerMatrix(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestTypeConvertPointerMatrix_Int32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<uint32,float32,2,2> table[]={
            {{{4294967295,0},{1235,1}},{{4294967295.0,0.0},{1235.0,1.0}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertPointerMatrix(table));
}

TEST(BareMetal_L4Configuration_TypeConversionGTest,TestTypeConvertPointerMatrix_Float32) {
    TypeConversionTest myTypeConversionTest;
    const TypeToTypeMatrixTableTest<float32,int32,2,2> table[]={
            {{{2147483647.5,-1234.47},{1234.67,0.01}},{{2147483647,-1234},{1235,0}}, true},
            {{{0,0}},{{0,0}},0}
    };

    ASSERT_TRUE(myTypeConversionTest.TestTypeConvertPointerMatrix(table));
}
