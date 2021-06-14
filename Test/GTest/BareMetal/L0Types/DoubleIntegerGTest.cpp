/*
 *  DoubleIntegerGTest.cpp
 *
 *  Created on: Mar 5, 2015
 */
#include "MARTe2UTest.h"
#include "DoubleIntegerTest.h"



TEST(BareMetal_L0Types_DoubleIntegerGTest,TestDefaultConstructor_64) {

    DoubleIntegerTest<uint64> Unumbertest;
    ASSERT_TRUE(Unumbertest.TestDefaultConstructor());
    DoubleIntegerTest<int64> Snumbertest;
    ASSERT_TRUE(Snumbertest.TestDefaultConstructor());

}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestDefaultConstructor_32) {

    DoubleIntegerTest<uint32> Unumbertest;
    ASSERT_TRUE(Unumbertest.TestDefaultConstructor());
    DoubleIntegerTest<int32> Snumbertest;
    ASSERT_TRUE(Snumbertest.TestDefaultConstructor());

}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestDefaultConstructor_16) {

    DoubleIntegerTest<uint16> Unumbertest;
    ASSERT_TRUE(Unumbertest.TestDefaultConstructor());
    DoubleIntegerTest<int16> Snumbertest;
    ASSERT_TRUE(Snumbertest.TestDefaultConstructor());

}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestDefaultConstructor_8) {

    DoubleIntegerTest<uint8> Unumbertest;
    ASSERT_TRUE(Unumbertest.TestDefaultConstructor());
    DoubleIntegerTest<int8> Snumbertest;
    ASSERT_TRUE(Snumbertest.TestDefaultConstructor());

}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestConstructorByType_8) {

    DoubleIntegerTest<uint8> Unumbertest;
    uint16 u16x1=0xff;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(u16x1));
    uint16 u16x2=0xffff;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(u16x2));
    uint8 u8x1=0xf;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(u8x1));
    uint16 u8x2=0xff;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(u8x2));

    DoubleIntegerTest<int8> Snumbertest;
    int16 s16x1=0xff;
    ASSERT_TRUE(Snumbertest.TestConstructorByType(s16x1));
    int16 s16x2=-1;
    ASSERT_TRUE(Snumbertest.TestConstructorByType(s16x2));
    int8 s8x1=0xf;
    ASSERT_TRUE(Snumbertest.TestConstructorByType(s8x1));
    int8 s8x2=-1;
    ASSERT_TRUE(Snumbertest.TestConstructorByType(s8x2));

}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestConstructorByType_16) {

    DoubleIntegerTest<uint16> Unumbertest;
    uint32 u32x1=0xffff;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(u32x1));
    uint32 u32x2=0xffffffff;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(u32x2));
    uint16 u16x1=0xff;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(u16x1));
    uint16 u16x2=0xffff;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(u16x2));
    uint8 u8x1=0xf;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(u8x1));
    uint8 u8x2=0xff;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(u8x2));
    DoubleIntegerTest<int16> Snumbertest;
    int32 s32x1=0xffff;
    ASSERT_TRUE(Snumbertest.TestConstructorByType(s32x1));
    int32 s32x2=-1;
    ASSERT_TRUE(Snumbertest.TestConstructorByType(s32x2));
    int16 s16x1=0xff;
    ASSERT_TRUE(Snumbertest.TestConstructorByType(s16x1));
    int16 s16x2=-1;
    ASSERT_TRUE(Snumbertest.TestConstructorByType(s16x2));
    int8 s8x1=0xf;
    ASSERT_TRUE(Snumbertest.TestConstructorByType(s8x1));
    int8 s8x2=-1;
    ASSERT_TRUE(Snumbertest.TestConstructorByType(s8x2));

}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestConstructorByType_32) {

    DoubleIntegerTest<uint32> Unumbertest;
    uint64 u64x1=0xffffffff;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(u64x1));
    uint64 u64x2=0xffffffffffffffff;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(u64x2));
    uint32 u32x1=0xffff;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(u32x1));
    uint32 u32x2=0xffffffff;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(u32x2));
    uint16 u16x1=0xff;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(u16x1));
    uint16 u16x2=0xffff;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(u16x2));
    uint8 u8x1=0xf;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(u8x1));
    uint8 u8x2=0xff;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(u8x2));
    DoubleIntegerTest<int32> Snumbertest;
    int64 s64x1=0xffffffff;
    ASSERT_TRUE(Snumbertest.TestConstructorByType(s64x1));
    int64 s64x2=-1;
    ASSERT_TRUE(Snumbertest.TestConstructorByType(s64x2));
    int32 s32x1=0xffff;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(s32x1));
    int32 s32x2=-1;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(s32x2));
    int16 s16x1=0xff;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(s16x1));
    int16 s16x2=-1;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(s16x2));
    int8 s8x1=0xf;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(s8x1));
    int8 s8x2=-1;
    ASSERT_TRUE(Unumbertest.TestConstructorByType(s8x2));

}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestCopyConstructor_64) {

    DoubleIntegerTest<uint64> Unumbertest;
    for(uint32 i=0; i<64; i++){
        DoubleInteger<uint64> Uinput;
        Uinput.SetUpper(1<<i);
        Uinput.SetLower(1<<i);
        ASSERT_TRUE(Unumbertest.TestCopyConstructor(Uinput));
    }
    DoubleIntegerTest<int64> Snumbertest;
    for(uint32 i=0; i<64; i++){
        DoubleInteger<int64> Sinput;
        Sinput.SetUpper(1<<i);
        Sinput.SetLower(1<<i);
        ASSERT_TRUE(Snumbertest.TestCopyConstructor(Sinput));
    }

}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestCopyConstructor_32) {

    DoubleIntegerTest<uint32> Unumbertest;
    for(uint32 i=0; i<32; i++){
        DoubleInteger<uint32> Uinput;
        Uinput.SetUpper(1<<i);
        Uinput.SetLower(1<<i);
        ASSERT_TRUE(Unumbertest.TestCopyConstructor(Uinput));
    }
    DoubleIntegerTest<int32> Snumbertest;
    for(uint32 i=0; i<32; i++){
        DoubleInteger<int32> Sinput;
        Sinput.SetUpper(1<<i);
        Sinput.SetLower(1<<i);
        ASSERT_TRUE(Snumbertest.TestCopyConstructor(Sinput));
    }

}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestCopyConstructor_16) {

    DoubleIntegerTest<uint16> Unumbertest;
    for(uint32 i=0; i<16; i++){
        DoubleInteger<uint16> Uinput;
        Uinput.SetUpper(1<<i);
        Uinput.SetLower(1<<i);
        ASSERT_TRUE(Unumbertest.TestCopyConstructor(Uinput));
    }
    DoubleIntegerTest<int16> Snumbertest;
    for(uint32 i=0; i<16; i++){
        DoubleInteger<int16> Sinput;
        Sinput.SetUpper(1<<i);
        Sinput.SetLower(1<<i);
        ASSERT_TRUE(Snumbertest.TestCopyConstructor(Sinput));
    }

}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestCopyConstructor_8) {

    DoubleIntegerTest<uint8> Unumbertest;
    for(uint32 i=0; i<8; i++){
        DoubleInteger<uint8> Uinput;
        Uinput.SetUpper(1<<i);
        Uinput.SetLower(1<<i);
        ASSERT_TRUE(Unumbertest.TestCopyConstructor(Uinput));
    }
    DoubleIntegerTest<int8> Snumbertest;
    for(uint32 i=0; i<8; i++){
        DoubleInteger<int8> Sinput;
        Sinput.SetUpper(1<<i);
        Sinput.SetLower(1<<i);
        ASSERT_TRUE(Snumbertest.TestCopyConstructor(Sinput));
    }
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestConstructorByParts_64) {

    DoubleIntegerTest<uint64> Unumbertest;
    for(uint32 i=0; i<64; i++){
        uint64 upper=(1<<i);
        uint64 lower=(1<<i);
        ASSERT_TRUE(Unumbertest.TestConstructorByParts(upper, lower));
    }
    DoubleIntegerTest<int64> Snumbertest;
    for(uint32 i=0; i<64; i++){
        int64 upper=(1<<i);
        int64 lower=(1<<i);
        ASSERT_TRUE(Snumbertest.TestConstructorByParts(upper, lower));
    }

}





TEST(BareMetal_L0Types_DoubleIntegerGTest,TestConstructorByParts_32) {

    DoubleIntegerTest<uint32> Unumbertest;
    for(uint32 i=0; i<32; i++){
        uint32 upper=(1<<i);
        uint32 lower=(1<<i);
        ASSERT_TRUE(Unumbertest.TestConstructorByParts(upper, lower));
    }
    DoubleIntegerTest<int32> Snumbertest;
    for(uint32 i=0; i<32; i++){
        int32 upper=(1<<i);
        int32 lower=(1<<i);
        ASSERT_TRUE(Snumbertest.TestConstructorByParts(upper, lower));
    }

}



TEST(BareMetal_L0Types_DoubleIntegerGTest,TestConstructorByParts_16) {

    DoubleIntegerTest<uint16> Unumbertest;
    for(uint32 i=0; i<16; i++){
        uint16 upper=(1<<i);
        uint16 lower=(1<<i);
        ASSERT_TRUE(Unumbertest.TestConstructorByParts(upper, lower));
    }
    DoubleIntegerTest<int16> Snumbertest;
    for(uint32 i=0; i<16; i++){
        int16 upper=(1<<i);
        int16 lower=(1<<i);
        ASSERT_TRUE(Snumbertest.TestConstructorByParts(upper, lower));
    }

}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestConstructorByParts_8) {

    DoubleIntegerTest<uint8> Unumbertest;
    for(uint32 i=0; i<8; i++){
        uint8 upper=(1<<i);
        uint8 lower=(1<<i);
        ASSERT_TRUE(Unumbertest.TestConstructorByParts(upper, lower));
    }
    DoubleIntegerTest<int8> Snumbertest;
    for(uint32 i=0; i<8; i++){
        int8 upper=(1<<i);
        int8 lower=(1<<i);
        ASSERT_TRUE(Snumbertest.TestConstructorByParts(upper, lower));
    }

}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestGetLower_8) {
    DoubleIntegerTest<uint8> Unumbertest;
    for(uint32 i=0; i<8; i++){
        uint8 lower=(1<<i);
        ASSERT_TRUE(Unumbertest.TestGetLower(lower));
    }
    DoubleIntegerTest<int8> Snumbertest;
    for(uint32 i=0; i<8; i++){
        int8 lower=(1<<i);
        ASSERT_TRUE(Snumbertest.TestGetLower(lower));
    }
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestGetLower_16) {
    DoubleIntegerTest<uint16> Unumbertest;
    for(uint32 i=0; i<16; i++){
        uint8 lower=(1<<i);
        ASSERT_TRUE(Unumbertest.TestGetLower(lower));
    }
    DoubleIntegerTest<int16> Snumbertest;
    for(uint32 i=0; i<16; i++){
        int8 lower=(1<<i);
        ASSERT_TRUE(Snumbertest.TestGetLower(lower));
    }
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestGetLower_32) {
    DoubleIntegerTest<uint32> Unumbertest;
    for(uint32 i=0; i<32; i++){
        uint8 lower=(1<<i);
        ASSERT_TRUE(Unumbertest.TestGetLower(lower));
    }
    DoubleIntegerTest<int32> Snumbertest;
    for(uint32 i=0; i<32; i++){
        int8 lower=(1<<i);
        ASSERT_TRUE(Snumbertest.TestGetLower(lower));
    }
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestGetLower_64) {
    DoubleIntegerTest<uint64> Unumbertest;
    for(uint32 i=0; i<64; i++){
        uint8 lower=(1<<i);
        ASSERT_TRUE(Unumbertest.TestGetLower(lower));
    }
    DoubleIntegerTest<int64> Snumbertest;
    for(uint32 i=0; i<64; i++){
        int8 lower=(1<<i);
        ASSERT_TRUE(Snumbertest.TestGetLower(lower));
    }
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSetLower_8) {
    DoubleIntegerTest<uint8> Unumbertest;
    for(uint32 i=0; i<8; i++){
        uint8 lower=(1<<i);
        ASSERT_TRUE(Unumbertest.TestSetLower(lower));
    }
    DoubleIntegerTest<int8> Snumbertest;
    for(uint32 i=0; i<8; i++){
        int8 lower=(1<<i);
        ASSERT_TRUE(Snumbertest.TestSetLower(lower));
    }
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSetLower_16) {
    DoubleIntegerTest<uint16> Unumbertest;
    for(uint32 i=0; i<16; i++){
        uint8 lower=(1<<i);
        ASSERT_TRUE(Unumbertest.TestSetLower(lower));
    }
    DoubleIntegerTest<int16> Snumbertest;
    for(uint32 i=0; i<16; i++){
        int8 lower=(1<<i);
        ASSERT_TRUE(Snumbertest.TestSetLower(lower));
    }
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSetLower_32) {
    DoubleIntegerTest<uint32> Unumbertest;
    for(uint32 i=0; i<32; i++){
        uint8 lower=(1<<i);
        ASSERT_TRUE(Unumbertest.TestSetLower(lower));
    }
    DoubleIntegerTest<int32> Snumbertest;
    for(uint32 i=0; i<32; i++){
        int8 lower=(1<<i);
        ASSERT_TRUE(Snumbertest.TestSetLower(lower));
    }
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSetLower_64) {
    DoubleIntegerTest<uint64> Unumbertest;
    for(uint32 i=0; i<64; i++){
        uint8 lower=(1<<i);
        ASSERT_TRUE(Unumbertest.TestSetLower(lower));
    }
    DoubleIntegerTest<int64> Snumbertest;
    for(uint32 i=0; i<64; i++){
        int8 lower=(1<<i);
        ASSERT_TRUE(Snumbertest.TestSetLower(lower));
    }
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestGetUpper_8) {
    DoubleIntegerTest<uint8> Unumbertest;
    for(uint32 i=0; i<8; i++){
        uint8 upper=(1<<i);
        ASSERT_TRUE(Unumbertest.TestGetUpper(upper));
    }
    DoubleIntegerTest<int8> Snumbertest;
    for(uint32 i=0; i<8; i++){
        int8 upper=(1<<i);
        ASSERT_TRUE(Snumbertest.TestGetUpper(upper));
    }
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestGetUpper_16) {
    DoubleIntegerTest<uint16> Unumbertest;
    for(uint32 i=0; i<16; i++){
        uint8 upper=(1<<i);
        ASSERT_TRUE(Unumbertest.TestGetUpper(upper));
    }
    DoubleIntegerTest<int16> Snumbertest;
    for(uint32 i=0; i<16; i++){
        int8 upper=(1<<i);
        ASSERT_TRUE(Snumbertest.TestGetUpper(upper));
    }
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestGetUpper_32) {
    DoubleIntegerTest<uint32> Unumbertest;
    for(uint32 i=0; i<32; i++){
        uint8 upper=(1<<i);
        ASSERT_TRUE(Unumbertest.TestGetUpper(upper));
    }
    DoubleIntegerTest<int32> Snumbertest;
    for(uint32 i=0; i<32; i++){
        int8 upper=(1<<i);
        ASSERT_TRUE(Snumbertest.TestGetUpper(upper));
    }
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestGetUpper_64) {
    DoubleIntegerTest<uint64> Unumbertest;
    for(uint32 i=0; i<64; i++){
        uint8 upper=(1<<i);
        ASSERT_TRUE(Unumbertest.TestGetUpper(upper));
    }
    DoubleIntegerTest<int64> Snumbertest;
    for(uint32 i=0; i<64; i++){
        int8 upper=(1<<i);
        ASSERT_TRUE(Snumbertest.TestGetUpper(upper));
    }
}




TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSetUpper_8) {
    DoubleIntegerTest<uint8> Unumbertest;
    for(uint32 i=0; i<8; i++){
        uint8 upper=(1<<i);
        ASSERT_TRUE(Unumbertest.TestSetUpper(upper));
    }
    DoubleIntegerTest<int8> Snumbertest;
    for(uint32 i=0; i<8; i++){
        int8 upper=(1<<i);
        ASSERT_TRUE(Snumbertest.TestSetUpper(upper));
    }
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSetUpper_16) {
    DoubleIntegerTest<uint16> Unumbertest;
    for(uint32 i=0; i<16; i++){
        uint8 upper=(1<<i);
        ASSERT_TRUE(Unumbertest.TestSetUpper(upper));
    }
    DoubleIntegerTest<int16> Snumbertest;
    for(uint32 i=0; i<16; i++){
        int8 upper=(1<<i);
        ASSERT_TRUE(Snumbertest.TestSetUpper(upper));
    }
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSetUpper_32) {
    DoubleIntegerTest<uint32> Unumbertest;
    for(uint32 i=0; i<32; i++){
        uint8 upper=(1<<i);
        ASSERT_TRUE(Unumbertest.TestSetUpper(upper));
    }
    DoubleIntegerTest<int32> Snumbertest;
    for(uint32 i=0; i<32; i++){
        int8 upper=(1<<i);
        ASSERT_TRUE(Snumbertest.TestSetUpper(upper));
    }
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSetUpper_64) {
    DoubleIntegerTest<uint64> Unumbertest;
    for(uint32 i=0; i<64; i++){
        uint8 upper=(1<<i);
        ASSERT_TRUE(Unumbertest.TestSetUpper(upper));
    }
    DoubleIntegerTest<int64> Snumbertest;
    for(uint32 i=0; i<64; i++){
        int8 upper=(1<<i);
        ASSERT_TRUE(Snumbertest.TestSetUpper(upper));
    }
}




TEST(BareMetal_L0Types_DoubleIntegerGTest,TestRightShift_int64) {
    const testTableShiftDI<int64> testTable[]={
            testTableShiftDI<int64>(DoubleInteger<int64>(0xf000000000000000, 0), 124, -1),
            testTableShiftDI<int64>(DoubleInteger<int64>(0x8000000000000000, 0), 3, DoubleInteger<int64>(0xf000000000000000, 0)),
            testTableShiftDI<int64>(DoubleInteger<int64>(0x7000000000000000, 0), 62, DoubleInteger<int64>(1,0xc000000000000000)),
            testTableShiftDI<int64>(0, 0xff, 0)
    };
    DoubleIntegerTest<int64> numbertest;
    ASSERT_TRUE(numbertest.TestRightShift(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestLeftShift_uint64) {
    const testTableShiftDI<uint64> testTable[]={
            testTableShiftDI<uint64>(DoubleInteger<uint64>(0, 0x000000000000000f), 124, DoubleInteger<uint64>(0xf000000000000000,0)),
            testTableShiftDI<uint64>(DoubleInteger<uint64>(0x1800000000000000, 0), 4, DoubleInteger<uint64>(0x8000000000000000,0)),
            testTableShiftDI<uint64>(DoubleInteger<uint64>(0, 0x0000000000000007), 62, DoubleInteger<uint64>(1,0xc000000000000000)),
            testTableShiftDI<uint64>(0, 0xff, 0)
    };
    DoubleIntegerTest<uint64> numbertest;
    ASSERT_TRUE(numbertest.TestLeftShift(testTable));
}



TEST(BareMetal_L0Types_DoubleIntegerGTest,TestRightShift_int32) {
    const testTableShiftDI<int32> testTable[]={
            testTableShiftDI<int32>(0xf000000000000000, 60, -1),
            testTableShiftDI<int32>(0x8000000000000000, 3, 0xf000000000000000),
            testTableShiftDI<int32>(0x7000000000000000, 30, 0x1c0000000),
            testTableShiftDI<int32>(0, 0xff, 0)
    };
    DoubleIntegerTest<int32> numbertest;
    ASSERT_TRUE(numbertest.TestRightShift(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestLeftShift_int32) {
    const testTableShiftDI<int32> testTable[]={
            testTableShiftDI<int32>(0x000000000000000f, 60, 0xf000000000000000),
            testTableShiftDI<int32>(0x1800000000000000, 4, 0x8000000000000000),
            testTableShiftDI<int32>(0x0000000000000007, 30, 0x1c0000000),
            testTableShiftDI<int32>(0, 0xff, 0)
    };
    DoubleIntegerTest<int32> numbertest;
    ASSERT_TRUE(numbertest.TestLeftShift(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestRightShift_uint32) {


    const testTableShiftDI<uint32> testTable[]={
            testTableShiftDI<uint32>(0xf000000000000000, 60, 0xf),
            testTableShiftDI<uint32>(0x8000000000000000, 3, 0x1000000000000000),
            testTableShiftDI<uint32>(0x7000000000000000, 30, 0x1c0000000),
            testTableShiftDI<uint32>(0, 0xff, 0)
    };
    DoubleIntegerTest<uint32> numbertest;

    ASSERT_TRUE(numbertest.TestRightShift(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestLeftShift_uint32) {
    const testTableShiftDI<uint32> testTable[]={
            testTableShiftDI<uint32>(0x000000000000000f, 60, 0xf000000000000000),
            testTableShiftDI<uint32>(0x1800000000000000, 4, 0x8000000000000000),
            testTableShiftDI<uint32>(0x0000000000000007, 30, 0x1c0000000),
            testTableShiftDI<uint32>(0, 0xff, 0)
    };    DoubleIntegerTest<uint32> numbertest;

    ASSERT_TRUE(numbertest.TestLeftShift(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestRightShift_int16) {
    const testTableShiftDI<int16> testTable[]={
            testTableShiftDI<int16>(0xf0000000, 30, -1),
            testTableShiftDI<int16>(0x80000000, 3, 0xf0000000),
            testTableShiftDI<int16>(0x70000000, 14, 0x1c000),
            testTableShiftDI<int16>(0, 0xff, 0)
    };
    DoubleIntegerTest<int16> numbertest;
    ASSERT_TRUE(numbertest.TestRightShift(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestLeftShift_uint16) {
    const testTableShiftDI<uint16> testTable[]={
            testTableShiftDI<uint16>(0x0000000f, 28, 0xf0000000),
            testTableShiftDI<uint16>(0x18000000, 4, 0x80000000),
            testTableShiftDI<uint16>(0x00000007, 14, 0x1c000),
            testTableShiftDI<uint16>(0, 0xff, 0)
    };
    DoubleIntegerTest<uint16> numbertest;
    ASSERT_TRUE(numbertest.TestLeftShift(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestRightShift_int8) {
    const testTableShiftDI<int8> testTable[]={
            testTableShiftDI<int8>(0xf000, 12, -1),
            testTableShiftDI<int8>(0x8000, 3, 0xf000),
            testTableShiftDI<int8>(0x7000, 6, 0x1c0),
            testTableShiftDI<int8>(0, 0xff, 0)
    };
    DoubleIntegerTest<int8> numbertest;
    ASSERT_TRUE(numbertest.TestRightShift(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestLeftShift_uint8) {
    const testTableShiftDI<uint8> testTable[]={
            testTableShiftDI<uint8>(0x000f, 13, 0xe000),
            testTableShiftDI<uint8>(0x1800, 4, 0x8000),
            testTableShiftDI<uint8>(0x0007, 6, 0x1c0),
            testTableShiftDI<uint8>(0, 0xff, 0)
    };
    DoubleIntegerTest<uint8> numbertest;
    ASSERT_TRUE(numbertest.TestLeftShift(testTable));
}




TEST(BareMetal_L0Types_DoubleIntegerGTest,TestAndOperator_int64) {

    const testTableLogicDI<int64> testTable[]={
            testTableLogicDI<int64>(DoubleInteger<int64>(0xffffffffffffffff, 0xffffffffffffffff), 0, 0),
            testTableLogicDI<int64>(DoubleInteger<int64>(0xffffffff0fffffff,0xffffffff0fffffff), DoubleInteger<int64>(0xfffffff0ffffffff,0xfffffff0ffffffff), DoubleInteger<int64>(0xfffffff00fffffff,0xfffffff00fffffff)),
            testTableLogicDI<int64>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<int64> numbertest;

    ASSERT_TRUE(numbertest.TestAndOperator(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestAndOperator_uint64) {

    const testTableLogicDI<uint64> testTable[]={
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0xffffffffffffffff, 0xffffffffffffffff), 0, 0),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0xffffffff0fffffff,0xffffffff0fffffff), DoubleInteger<uint64>(0xfffffff0ffffffff,0xfffffff0ffffffff), DoubleInteger<uint64>(0xfffffff00fffffff,0xfffffff00fffffff)),
            testTableLogicDI<uint64>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<uint64> numbertest;

    ASSERT_TRUE(numbertest.TestAndOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestAndOperator_int32) {

    const testTableLogicDI<int32> testTable[]={
            testTableLogicDI<int32>(0xffffffffffffffff, 0, 0),
            testTableLogicDI<int32>(0xffffffff0fffffff, 0xfffffff0ffffffff, 0xfffffff00fffffff),
            testTableLogicDI<int32>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<int32> numbertest;

    ASSERT_TRUE(numbertest.TestAndOperator(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestAndOperator_uint32) {

    const testTableLogicDI<uint32> testTable[]={
            testTableLogicDI<uint32>(0xffffffffffffffff, 0, 0),
            testTableLogicDI<uint32>(0xffffffff0fffffff, 0xfffffff0ffffffff, 0xfffffff00fffffff),
            testTableLogicDI<uint32>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<uint32> numbertest;

    ASSERT_TRUE(numbertest.TestAndOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestAndOperator_int16) {

    const testTableLogicDI<int16> testTable[]={
            testTableLogicDI<int16>(0xffffffff, 0, 0),
            testTableLogicDI<int16>(0xfff0ffff, 0xffff0fff, 0xfff00fff),
            testTableLogicDI<int16>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<int16> numbertest;

    ASSERT_TRUE(numbertest.TestAndOperator(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestAndOperator_uint16) {

    const testTableLogicDI<uint16> testTable[]={
            testTableLogicDI<uint16>(0xffffffff, 0, 0),
            testTableLogicDI<uint16>(0xfff0ffff, 0xffff0fff, 0xfff00fff),
            testTableLogicDI<uint16>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<uint16> numbertest;

    ASSERT_TRUE(numbertest.TestAndOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestAndOperator_int8) {

    const testTableLogicDI<int8> testTable[]={
            testTableLogicDI<int8>(0xffff, 0, 0),
            testTableLogicDI<int8>(0xf0ff, 0xff0f, 0xf00f),
            testTableLogicDI<int8>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<int8> numbertest;

    ASSERT_TRUE(numbertest.TestAndOperator(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestAndOperator_uint8) {

    const testTableLogicDI<uint8> testTable[]={
            testTableLogicDI<uint8>(0xffff, 0, 0),
            testTableLogicDI<uint8>(0xf0ff, 0xff0f, 0xf00f),
            testTableLogicDI<uint8>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<uint8> numbertest;

    ASSERT_TRUE(numbertest.TestAndOperator(testTable));
}



TEST(BareMetal_L0Types_DoubleIntegerGTest,TestInvertOperator_int64) {

    const testTableLogicDI<int64> testTable[]={
            testTableLogicDI<int64>(DoubleInteger<int64>(0xffffffffffffffff,0xffffffffffffffff), 0, 0),
            testTableLogicDI<int64>(DoubleInteger<int64>(0xffffffff0fffffff,0xffffffff0fffffff), 0, DoubleInteger<int64>(0x00000000f0000000,0x00000000f0000000)),
            testTableLogicDI<int64>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<int64> numbertest;
    ASSERT_TRUE(numbertest.TestInvertOperator(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestInvertOperator_uint64) {

    const testTableLogicDI<uint64> testTable[]={
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0xffffffffffffffff,0xffffffffffffffff), 0, 0),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0xffffffff0fffffff,0xffffffff0fffffff), 0, DoubleInteger<uint64>(0x00000000f0000000,0x00000000f0000000)),
            testTableLogicDI<uint64>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<uint64> numbertest;
    ASSERT_TRUE(numbertest.TestInvertOperator(testTable));
}



TEST(BareMetal_L0Types_DoubleIntegerGTest,TestInvertOperator_int32) {

    const testTableLogicDI<int32> testTable[]={
            testTableLogicDI<int32>(0xffffffffffffffff, 0, 0),
            testTableLogicDI<int32>(0xffffffff0fffffff, 0, 0x00000000f0000000),
            testTableLogicDI<int32>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<int32> numbertest;
    ASSERT_TRUE(numbertest.TestInvertOperator(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestInvertOperator_uint32) {

    const testTableLogicDI<uint32> testTable[]={
            testTableLogicDI<uint32>(0xffffffffffffffff, 0, 0),
            testTableLogicDI<uint32>(0xffffffff0fffffff, 0, 0x00000000f0000000),
            testTableLogicDI<uint32>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint32> numbertest;
    ASSERT_TRUE(numbertest.TestInvertOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestInvertOperator_int16) {

    const testTableLogicDI<int16> testTable[]={
            testTableLogicDI<int16>(0xffffffff, 0, 0),
            testTableLogicDI<int16>(0xffff0fff, 0, 0x0000f000),
            testTableLogicDI<int16>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<int16> numbertest;
    ASSERT_TRUE(numbertest.TestInvertOperator(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestInvertOperator_uint16) {
    const testTableLogicDI<uint16> testTable[]={
            testTableLogicDI<uint16>(0xffffffff, 0, 0),
            testTableLogicDI<uint16>(0xffff0fff, 0, 0x0000f000),
            testTableLogicDI<uint16>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<uint16> numbertest;
    ASSERT_TRUE(numbertest.TestInvertOperator(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestInvertOperator_int8) {

    const testTableLogicDI<int8> testTable[]={
            testTableLogicDI<int8>(0xffff, 0, 0),
            testTableLogicDI<int8>(0xff0f, 0, 0x00f0),
            testTableLogicDI<int8>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<int8> numbertest;
    ASSERT_TRUE(numbertest.TestInvertOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestInvertOperator_uint8) {

    const testTableLogicDI<uint8> testTable[]={
            testTableLogicDI<uint8>(0xffff, 0, 0),
            testTableLogicDI<uint8>(0xff0f, 0, 0x00f0),
            testTableLogicDI<uint8>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<uint8> numbertest;
    ASSERT_TRUE(numbertest.TestInvertOperator(testTable));
}



TEST(BareMetal_L0Types_DoubleIntegerGTest,TestOrOperator_int64) {

    const testTableLogicDI<int64> testTable[]={
            testTableLogicDI<int64>(DoubleInteger<int64>(0xffffffffffffffff,0xffffffffffffffff), 0, DoubleInteger<int64>(0xffffffffffffffff,0xffffffffffffffff)),
            testTableLogicDI<int64>(DoubleInteger<int64>(0xffffffff0fffffff,0xffffffff0fffffff), DoubleInteger<int64>(0xfffffff0ffffffff,0xfffffff0ffffffff), DoubleInteger<int64>(0xffffffffffffffff,0xffffffffffffffff)),
            testTableLogicDI<int64>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<int64> numbertest;

    ASSERT_TRUE(numbertest.TestOrOperator(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestOrOperator_uint64) {

    const testTableLogicDI<uint64> testTable[]={
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0xffffffffffffffff,0xffffffffffffffff), 0, DoubleInteger<uint64>(0xffffffffffffffff,0xffffffffffffffff)),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0xffffffff0fffffff,0xffffffff0fffffff), DoubleInteger<uint64>(0xfffffff0ffffffff,0xfffffff0ffffffff), DoubleInteger<uint64>(0xffffffffffffffff,0xffffffffffffffff)),
            testTableLogicDI<uint64>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<uint64> numbertest;

    ASSERT_TRUE(numbertest.TestOrOperator(testTable));
}



TEST(BareMetal_L0Types_DoubleIntegerGTest,TestOrOperator_int32) {

    const testTableLogicDI<int32> testTable[]={
            testTableLogicDI<int32>(0xffffffffffffffff, 0, 0xffffffffffffffff),
            testTableLogicDI<int32>(0xffffffff0fffffff, 0xfffffff0ffffffff, 0xffffffffffffffff),
            testTableLogicDI<int32>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<int32> numbertest;

    ASSERT_TRUE(numbertest.TestOrOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestOrOperator_uint32) {

    const testTableLogicDI<uint32> testTable[]={
            testTableLogicDI<uint32>(0xffffffffffffffff, 0, 0xffffffffffffffff),
            testTableLogicDI<uint32>(0xffffffff0fffffff, 0xfffffff0ffffffff, 0xffffffffffffffff),
            testTableLogicDI<uint32>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<uint32> numbertest;

    ASSERT_TRUE(numbertest.TestOrOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestOrOperator_int16) {

    const testTableLogicDI<int16> testTable[]={
            testTableLogicDI<int16>(0xffffffff, 0, 0xffffffff),
            testTableLogicDI<int16>(0xffff0fff, 0xfff0ffff, 0xffffffffffffffff),
            testTableLogicDI<int16>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<int16> numbertest;

    ASSERT_TRUE(numbertest.TestOrOperator(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestOrOperator_uint16) {

    const testTableLogicDI<uint16> testTable[]={
            testTableLogicDI<uint16>(0xffffffff, 0, 0xffffffff),
            testTableLogicDI<uint16>(0xffff0fff, 0xfff0ffff, 0xffffffffffffffff),
            testTableLogicDI<uint16>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<uint16> numbertest;

    ASSERT_TRUE(numbertest.TestOrOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestOrOperator_int8) {

    const testTableLogicDI<int8> testTable[]={
            testTableLogicDI<int8>(0xffff, 0, 0xffff),
            testTableLogicDI<int8>(0xff0f, 0xf0ff, 0xffffffffffffffff),
            testTableLogicDI<int8>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<int8> numbertest;

    ASSERT_TRUE(numbertest.TestOrOperator(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestOrOperator_uint8) {

    const testTableLogicDI<uint8> testTable[]={
            testTableLogicDI<uint8>(0xffff, 0, 0xffff),
            testTableLogicDI<uint8>(0xff0f, 0xf0ff, 0xffffffffffffffff),
            testTableLogicDI<uint8>(0, 0xff, 0, false)
    };
    DoubleIntegerTest<uint8> numbertest;

    ASSERT_TRUE(numbertest.TestOrOperator(testTable));
}



TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMajorOperator_int64) {

    const testTableLogicDI<int64> testTable[]={
            testTableLogicDI<int64>(DoubleInteger<int64>(0xffffffffffffffff,0xffffffffffffffff), 0, 0),
            testTableLogicDI<int64>(1, 0, 1),
            testTableLogicDI<int64>(DoubleInteger<int64>(0xffffffffffffffff,0xffffffffffffffff), DoubleInteger<int64>(0xffffffffffffffff,0xfffffffffffffffe), 1),
            testTableLogicDI<int64>(DoubleInteger<int64>(0x7fffffffffffffff,0xffffffffffffffff), DoubleInteger<int64>(0xffffffffffffffff,0xffffffffffffffff), 1),
            testTableLogicDI<int64>(DoubleInteger<int64>(0x7fffffffffffffff,0xffffffffffffffff), DoubleInteger<int64>(0x7fffffffffffffff,0xfffffffffffffffe), 1),
            testTableLogicDI<int64>(DoubleInteger<int64>(0x7fffffffffffffff,0xffffffffffffffff), DoubleInteger<int64>(0x7fffffffffffffff,0xffffffffffffffff), 0),
            testTableLogicDI<int64>(0, 0, 0, false)
    };
    DoubleIntegerTest<int64> numbertest;

    ASSERT_TRUE(numbertest.TestIsMajorOperator(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMajorOperator_uint64) {

    const testTableLogicDI<uint64> testTable[]={
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0xffffffffffffffff,0xffffffffffffffff), 0, 1),
            testTableLogicDI<uint64>(1, 0, 1),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0xffffffffffffffff,0xffffffffffffffff), DoubleInteger<uint64>(0xffffffffffffffff,0xfffffffffffffffe), 1),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0x7fffffffffffffff,0xffffffffffffffff), DoubleInteger<uint64>(0xffffffffffffffff,0xffffffffffffffff), 0),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0x7fffffffffffffff,0xffffffffffffffff), DoubleInteger<uint64>(0x7fffffffffffffff,0xfffffffffffffffe), 1),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0x7fffffffffffffff,0xffffffffffffffff), DoubleInteger<uint64>(0x7fffffffffffffff,0xffffffffffffffff), 0),
            testTableLogicDI<uint64>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint64> numbertest;

    ASSERT_TRUE(numbertest.TestIsMajorOperator(testTable));
}



TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMajorOperator_int32) {

    const testTableLogicDI<int32> testTable[]={
            testTableLogicDI<int32>(0xffffffffffffffff, 0, 0),
            testTableLogicDI<int32>(1, 0, 1),
            testTableLogicDI<int32>(0xffffffffffffffff, 0xfffffffffffffffe, 1),
            testTableLogicDI<int32>(0x7fffffffffffffff, 0x7ffffffffffffffe, 1),
            testTableLogicDI<int32>(0x7fffffffffffffff, 0x7fffffffffffffff, 0),
            testTableLogicDI<int32>(0, 0, 0, false)
    };
    DoubleIntegerTest<int32> numbertest;

    ASSERT_TRUE(numbertest.TestIsMajorOperator(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMajorOperator_uint32) {

    const testTableLogicDI<uint32> testTable[]={
            testTableLogicDI<uint32>(0xffffffffffffffff, 0, 1),
            testTableLogicDI<uint32>(1, 0, 1),
            testTableLogicDI<uint32>(0xffffffffffffffff, 0xfffffffffffffffe, 1),
            testTableLogicDI<uint32>(0x7fffffffffffffff, 0x7ffffffffffffffe, 1),
            testTableLogicDI<uint32>(0x7fffffffffffffff, 0x7fffffffffffffff, 0),
            testTableLogicDI<uint32>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint32> numbertest;

    ASSERT_TRUE(numbertest.TestIsMajorOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMajorOperator_int16) {

    const testTableLogicDI<int16> testTable[]={
            testTableLogicDI<int16>(0xffffffff, 0, 0),
            testTableLogicDI<int16>(1, 0, 1),
            testTableLogicDI<int16>(0xfffffff, 0xfffffffe, 1),
            testTableLogicDI<int16>(0x7fffffff, 0x7ffffffe, 1),
            testTableLogicDI<int16>(0x7fffffff, 0x7fffffff, 0),
            testTableLogicDI<int16>(0, 0, 0, false)
    };
    DoubleIntegerTest<int16> numbertest;

    ASSERT_TRUE(numbertest.TestIsMajorOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMajorOperator_uint16) {

    const testTableLogicDI<uint16> testTable[]={
            testTableLogicDI<uint16>(0xffffffff, 0, 1),
            testTableLogicDI<uint16>(1, 0, 1),
            testTableLogicDI<uint16>(0xffffffff, 0xfffffffe, 1),
            testTableLogicDI<uint16>(0x7fffffff, 0x7ffffffe, 1),
            testTableLogicDI<uint16>(0x7fffffff, 0x7fffffff, 0),
            testTableLogicDI<uint16>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint16> numbertest;

    ASSERT_TRUE(numbertest.TestIsMajorOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMajorOperator_int8) {

    const testTableLogicDI<int8> testTable[]={
            testTableLogicDI<int8>(0xffff, 0, 0),
            testTableLogicDI<int8>(1, 0, 1),
            testTableLogicDI<int8>(0xffff, 0xfffe, 1),
            testTableLogicDI<int8>(0x7fff, 0x7ffe, 1),
            testTableLogicDI<int8>(0x7fff, 0x7fff, 0),
            testTableLogicDI<int8>(0, 0, 0, false)
    };
    DoubleIntegerTest<int8> numbertest;

    ASSERT_TRUE(numbertest.TestIsMajorOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMajorOperator_uint8) {

    const testTableLogicDI<uint8> testTable[]={
            testTableLogicDI<uint8>(0xffff, 0, 1),
            testTableLogicDI<uint8>(1, 0, 1),
            testTableLogicDI<uint8>(0xffff, 0xfffe, 1),
            testTableLogicDI<uint8>(0x7fff, 0x7ffe, 1),
            testTableLogicDI<uint8>(0x7fff, 0x7fff, 0),
            testTableLogicDI<uint8>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint8> numbertest;

    ASSERT_TRUE(numbertest.TestIsMajorOperator(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMinorOperator_int64) {

    const testTableLogicDI<int64> testTable[]={
            testTableLogicDI<int64>(DoubleInteger<int64>(0xffffffffffffffff,0xffffffffffffffff), 0, 1),
            testTableLogicDI<int64>(0, 1, 1),
            testTableLogicDI<int64>(DoubleInteger<int64>(0xffffffffffffffff,0xffffffffffffffff), DoubleInteger<int64>(0xffffffffffffffff,0xfffffffffffffffe), 0),
            testTableLogicDI<int64>(DoubleInteger<int64>(0xffffffffffffffff,0xfffffffffffffffe), DoubleInteger<int64>(0xffffffffffffffff,0xfffffffffffffffe), 0),
            testTableLogicDI<int64>(0, 0, 0, false)
    };
    DoubleIntegerTest<int64> numbertest;

    ASSERT_TRUE(numbertest.TestIsMinorOperator(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMinorOperator_uint64) {

    const testTableLogicDI<uint64> testTable[]={
            testTableLogicDI<uint64>(0, DoubleInteger<uint64>(0xffffffffffffffff,0xffffffffffffffff), 1),
            testTableLogicDI<uint64>(0, 1, 1),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0xffffffffffffffff,0xfffffffffffffffe), DoubleInteger<uint64>(0xffffffffffffffff,0xffffffffffffffff), 1),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0xffffffffffffffff,0xfffffffffffffffe), DoubleInteger<uint64>(0xffffffffffffffff,0xfffffffffffffffe), 0),
            testTableLogicDI<uint64>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint64> numbertest;

    ASSERT_TRUE(numbertest.TestIsMinorOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMinorOperator_int32) {

    const testTableLogicDI<int32> testTable[]={
            testTableLogicDI<int32>(0xffffffffffffffff, 0, 1),
            testTableLogicDI<int32>(0, 1, 1),
            testTableLogicDI<int32>(0xffffffffffffffff, 0xfffffffffffffffe, 0),
            testTableLogicDI<int32>(0xfffffffffffffffe, 0xfffffffffffffffe, 0),
            testTableLogicDI<int32>(0, 0, 0, false)
    };
    DoubleIntegerTest<int32> numbertest;

    ASSERT_TRUE(numbertest.TestIsMinorOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMinorOperator_uint32) {

    const testTableLogicDI<uint32> testTable[]={
            testTableLogicDI<uint32>(0xffffffffffffffff, 0, 0),
            testTableLogicDI<uint32>(0, 1, 1),
            testTableLogicDI<uint32>(0xffffffffffffffff, 0xfffffffffffffffe, 0),
            testTableLogicDI<uint32>(0xfffffffffffffffe, 0xfffffffffffffffe, 0),
            testTableLogicDI<uint32>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint32> numbertest;

    ASSERT_TRUE(numbertest.TestIsMinorOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMinorOperator_int16) {

    const testTableLogicDI<int16> testTable[]={
            testTableLogicDI<int16>(0xffffffff, 0, 1),
            testTableLogicDI<int16>(0, 1, 1),
            testTableLogicDI<int16>(0xffffffff, 0xfffffffe, 0),
            testTableLogicDI<int16>(0xfffffffe, 0xfffffffe, 0),
            testTableLogicDI<int16>(0, 0, 0, false)
    };
    DoubleIntegerTest<int16> numbertest;

    ASSERT_TRUE(numbertest.TestIsMinorOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMinorOperator_uint16) {

    const testTableLogicDI<uint16> testTable[]={
            testTableLogicDI<uint16>(0, 0xffffffff, 1),
            testTableLogicDI<uint16>(0, 1, 1),
            testTableLogicDI<uint16>(0xffffffff, 0xfffffffe, 0),
            testTableLogicDI<uint16>(0xfffffffe, 0xfffffffe, 0),
            testTableLogicDI<uint16>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint16> numbertest;

    ASSERT_TRUE(numbertest.TestIsMinorOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMinorOperator_int8) {

    const testTableLogicDI<int8> testTable[]={
            testTableLogicDI<int8>(0xffff, 0, 1),
            testTableLogicDI<int8>(0, 1, 1),
            testTableLogicDI<int8>(0xffff, 0xfffe, 0),
            testTableLogicDI<int8>(0xfffe, 0xfffe, 0),
            testTableLogicDI<int8>(0, 0, 0, false)
    };
    DoubleIntegerTest<int8> numbertest;

    ASSERT_TRUE(numbertest.TestIsMinorOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMinorOperator_uint8) {

    const testTableLogicDI<uint8> testTable[]={
            testTableLogicDI<uint8>(0, 0xffff, 1),
            testTableLogicDI<uint8>(0, 1, 1),
            testTableLogicDI<uint8>(0xffff, 0xfffe, 0),
            testTableLogicDI<uint8>(0xfffe, 0xfffe, 0),
            testTableLogicDI<uint8>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint8> numbertest;

    ASSERT_TRUE(numbertest.TestIsMinorOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMajorEqualOperator_int64) {

    const testTableLogicDI<int64> testTable[]={
            testTableLogicDI<int64>(DoubleInteger<int64>(0xffffffffffffffff,0xffffffffffffffff), 0, 0),
            testTableLogicDI<int64>(1, 0, 1),
            testTableLogicDI<int64>(DoubleInteger<int64>(0xffffffffffffffff,0xffffffffffffffff), DoubleInteger<int64>(0xffffffffffffffff,0xfffffffffffffffe), 1),
            testTableLogicDI<int64>(DoubleInteger<int64>(0x7fffffffffffffff,0xffffffffffffffff), DoubleInteger<int64>(0x7fffffffffffffff,0xfffffffffffffffe), 1),
            testTableLogicDI<int64>(DoubleInteger<int64>(0x7fffffffffffffff,0xffffffffffffffff), DoubleInteger<int64>(0x7fffffffffffffff,0xffffffffffffffff), 1),
            testTableLogicDI<int64>(0, 0, 0, false)
    };
    DoubleIntegerTest<int64> numbertest;

    ASSERT_TRUE(numbertest.TestIsMajorEqualOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMajorEqualOperator_uint64) {

    const testTableLogicDI<uint64> testTable[]={
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0xffffffffffffffff,0xffffffffffffffff), 0, 1),
            testTableLogicDI<uint64>(1, 0, 1),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0xffffffffffffffff,0xffffffffffffffff), DoubleInteger<uint64>(0xffffffffffffffff,0xfffffffffffffffe), 1),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0x7fffffffffffffff,0xffffffffffffffff), DoubleInteger<uint64>(0x7fffffffffffffff,0xfffffffffffffffe), 1),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0x7fffffffffffffff,0xffffffffffffffff), DoubleInteger<uint64>(0x7fffffffffffffff,0xffffffffffffffff), 1),
            testTableLogicDI<uint64>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint64> numbertest;

    ASSERT_TRUE(numbertest.TestIsMajorEqualOperator(testTable));
}





TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMajorEqualOperator_int32) {

    const testTableLogicDI<int32> testTable[]={
            testTableLogicDI<int32>(0xffffffffffffffff, 0, 0),
            testTableLogicDI<int32>(1, 0, 1),
            testTableLogicDI<int32>(0xffffffffffffffff, 0xfffffffffffffffe, 1),
            testTableLogicDI<int32>(0x7fffffffffffffff, 0x7ffffffffffffffe, 1),
            testTableLogicDI<int32>(0x7fffffffffffffff, 0x7fffffffffffffff, 1),
            testTableLogicDI<int32>(0x7fffffff7fffffff, 0x7fffffffffffffff, 0),
            testTableLogicDI<int32>(0x7fffffffffffffff, 0x7fffffff7fffffff, 1),
            testTableLogicDI<int32>(0, 0, 0, false)
    };
    DoubleIntegerTest<int32> numbertest;

    ASSERT_TRUE(numbertest.TestIsMajorEqualOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMajorEqualOperator_uint32) {

    const testTableLogicDI<uint32> testTable[]={
            testTableLogicDI<uint32>(0xffffffffffffffff, 0, 1),
            testTableLogicDI<uint32>(1, 0, 1),
            testTableLogicDI<uint32>(0xffffffffffffffff, 0xfffffffffffffffe, 1),
            testTableLogicDI<uint32>(0x7fffffffffffffff, 0x7ffffffffffffffe, 1),
            testTableLogicDI<uint32>(0x7fffffffffffffff, 0x7fffffffffffffff, 1),
            testTableLogicDI<uint32>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint32> numbertest;

    ASSERT_TRUE(numbertest.TestIsMajorEqualOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMajorEqualOperator_int16) {

    const testTableLogicDI<int16> testTable[]={
            testTableLogicDI<int16>(0xffffffff, 0, 0),
            testTableLogicDI<int16>(1, 0, 1),
            testTableLogicDI<int16>(0xffffffff, 0xfffffffe, 1),
            testTableLogicDI<int16>(0x7fffffff, 0x7ffffffe, 1),
            testTableLogicDI<int16>(0x7fffffff, 0x7fffffff, 1),
            testTableLogicDI<int16>(0, 0, 0, false)
    };
    DoubleIntegerTest<int16> numbertest;

    ASSERT_TRUE(numbertest.TestIsMajorEqualOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMajorEqualOperator_uint16) {

    const testTableLogicDI<uint16> testTable[]={
            testTableLogicDI<uint16>(0xffffffff, 0, 1),
            testTableLogicDI<uint16>(1, 0, 1),
            testTableLogicDI<uint16>(0xffffffff, 0xfffffffe, 1),
            testTableLogicDI<uint16>(0x7fffffff, 0x7ffffffe, 1),
            testTableLogicDI<uint16>(0x7fffffff, 0x7fffffff, 1),
            testTableLogicDI<uint16>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint16> numbertest;

    ASSERT_TRUE(numbertest.TestIsMajorEqualOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMajorEqualOperator_int8) {

    const testTableLogicDI<int8> testTable[]={
            testTableLogicDI<int8>(0xffff, 0, 0),
            testTableLogicDI<int8>(1, 0, 1),
            testTableLogicDI<int8>(0xffff, 0xfffe, 1),
            testTableLogicDI<int8>(0x7fff, 0x7ffe, 1),
            testTableLogicDI<int8>(0x7fff, 0x7fff, 1),
            testTableLogicDI<int8>(0, 0, 0, false)
    };
    DoubleIntegerTest<int8> numbertest;

    ASSERT_TRUE(numbertest.TestIsMajorEqualOperator(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMajorEqualOperator_uint8) {

    const testTableLogicDI<uint8> testTable[]={
            testTableLogicDI<uint8>(0xffff, 0, 1),
            testTableLogicDI<uint8>(1, 0, 1),
            testTableLogicDI<uint8>(0xffff, 0xfffe, 1),
            testTableLogicDI<uint8>(0x7fff, 0x7ffe, 1),
            testTableLogicDI<uint8>(0x7fff, 0x7fff, 1),
            testTableLogicDI<uint8>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint8> numbertest;

    ASSERT_TRUE(numbertest.TestIsMajorEqualOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMinorEqualOperator_int64) {

    const testTableLogicDI<int64> testTable[]={
            testTableLogicDI<int64>(DoubleInteger<int64>(0xffffffffffffffff,0xffffffffffffffff), 0, 1),
            testTableLogicDI<int64>(0, 1, 1),
            testTableLogicDI<int64>(DoubleInteger<int64>(0xffffffffffffffff,0xffffffffffffffff), DoubleInteger<int64>(0xffffffffffffffff,0xfffffffffffffffe), 0),
            testTableLogicDI<int64>(DoubleInteger<int64>(0xffffffffffffffff,0xfffffffffffffffe), DoubleInteger<int64>(0xffffffffffffffff,0xfffffffffffffffe), 1),
            testTableLogicDI<int64>(0, 0, 0, false)
    };
    DoubleIntegerTest<int64> numbertest;

    ASSERT_TRUE(numbertest.TestIsMinorEqualOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMinorEqualOperator_uint64) {

    const testTableLogicDI<uint64> testTable[]={
            testTableLogicDI<uint64>(0, DoubleInteger<uint64>(0xffffffffffffffff,0xffffffffffffffff), 1),
            testTableLogicDI<uint64>(0, 1, 1),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0xffffffffffffffff,0xffffffffffffffff), DoubleInteger<uint64>(0xffffffffffffffff,0xfffffffffffffffe), 0),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0xffffffffffffffff,0xfffffffffffffffe), DoubleInteger<uint64>(0xffffffffffffffff,0xfffffffffffffffe), 1),
            testTableLogicDI<uint64>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint64> numbertest;

    ASSERT_TRUE(numbertest.TestIsMinorEqualOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMinorEqualOperator_int32) {

    const testTableLogicDI<int32> testTable[]={
            testTableLogicDI<int32>(0xffffffffffffffff, 0, 1),
            testTableLogicDI<int32>(0, 1, 1),
            testTableLogicDI<int32>(0xffffffffffffffff, 0xfffffffffffffffe, 0),
            testTableLogicDI<int32>(0xfffffffffffffffe, 0xfffffffffffffffe, 1),
            testTableLogicDI<int32>(0, 0, 0, false)
    };
    DoubleIntegerTest<int32> numbertest;

    ASSERT_TRUE(numbertest.TestIsMinorEqualOperator(testTable));
}



TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMinorEqualOperator_uint32) {

    const testTableLogicDI<uint32> testTable[]={
            testTableLogicDI<uint32>(0xffffffffffffffff, 0, 0),
            testTableLogicDI<uint32>(0, 1, 1),
            testTableLogicDI<uint32>(0xffffffffffffffff, 0xfffffffffffffffe, 0),
            testTableLogicDI<uint32>(0xfffffffffffffffe, 0xfffffffffffffffe, 1),
            testTableLogicDI<uint32>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint32> numbertest;

    ASSERT_TRUE(numbertest.TestIsMinorEqualOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMinorEqualOperator_int16) {

    const testTableLogicDI<int16> testTable[]={
            testTableLogicDI<int16>(0xffffffff, 0, 1),
            testTableLogicDI<int16>(0, 1, 1),
            testTableLogicDI<int16>(0xffffffff, 0xfffffffe, 0),
            testTableLogicDI<int16>(0xfffffffe, 0xffffffffe, 1),
            testTableLogicDI<int16>(0, 0, 0, false)
    };
    DoubleIntegerTest<int16> numbertest;

    ASSERT_TRUE(numbertest.TestIsMinorEqualOperator(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMinorEqualOperator_uint16) {

    const testTableLogicDI<uint16> testTable[]={
            testTableLogicDI<uint16>(0xffffffff, 0, 0),
            testTableLogicDI<uint16>(0, 1, 1),
            testTableLogicDI<uint16>(0xffffffff, 0xfffffffe, 0),
            testTableLogicDI<uint16>(0xfffffffe, 0xffffffffe, 1),
            testTableLogicDI<uint16>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint16> numbertest;

    ASSERT_TRUE(numbertest.TestIsMinorEqualOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMinorEqualOperator_int8) {

    const testTableLogicDI<int8> testTable[]={
            testTableLogicDI<int8>(0xffff, 0, 1),
            testTableLogicDI<int8>(0, 1, 1),
            testTableLogicDI<int8>(0xffff, 0xfffe, 0),
            testTableLogicDI<int8>(0xfffe, 0xfffe, 1),
            testTableLogicDI<int8>(0, 0, 0, false)
    };
    DoubleIntegerTest<int8> numbertest;

    ASSERT_TRUE(numbertest.TestIsMinorEqualOperator(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsMinorEqualOperator_uint8) {

    const testTableLogicDI<uint8> testTable[]={
            testTableLogicDI<uint8>(0xffff, 0, 0),
            testTableLogicDI<uint8>(0, 1, 1),
            testTableLogicDI<uint8>(0xffff, 0xfffe, 0),
            testTableLogicDI<uint8>(0xfffe, 0xfffe, 1),
            testTableLogicDI<uint8>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint8> numbertest;

    ASSERT_TRUE(numbertest.TestIsMinorEqualOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSumOperator_int64) {

    const testTableLogicDI<int64> testTable[]={
            testTableLogicDI<int64>(0x7fffffffffffffff, 0xfffffffffffffffe, DoubleInteger<int64>(1,0x7ffffffffffffffd)),
            testTableLogicDI<int64>(0x7fffffffffffffff, 0x7ffffffffffffffe, 0xfffffffffffffffd),
            testTableLogicDI<int64>(0xefffffffffffffff, 0xfffffffffffffffe, DoubleInteger<int64>(1,0xeffffffffffffffd)),
            testTableLogicDI<int64>(-1, -2, -3),
            testTableLogicDI<int64>(0, 0, 0, false)
    };
    DoubleIntegerTest<int64> numbertest;

    ASSERT_TRUE(numbertest.TestSumOperator(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSumOperator_uint64) {

    const testTableLogicDI<uint64> testTable[]={
            testTableLogicDI<uint64>(0x7fffffffffffffff, 0xfffffffffffffffe, DoubleInteger<uint64>(1,0x7ffffffffffffffd)),
            testTableLogicDI<uint64>(0x7fffffffffffffff, 0x7ffffffffffffffe, 0xfffffffffffffffd),
            testTableLogicDI<uint64>(0xefffffffffffffff, 0xfffffffffffffffe, DoubleInteger<uint64>(1,0xeffffffffffffffd)),
            testTableLogicDI<uint64>(-1, -2, -3),
            testTableLogicDI<uint64>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint64> numbertest;

    ASSERT_TRUE(numbertest.TestSumOperator(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSumOperator_int32) {

    const testTableLogicDI<int32> testTable[]={
            testTableLogicDI<int32>(0x7fffffff, 0xfffffffe, 0x17ffffffd),
            testTableLogicDI<int32>(0x7fffffff, 0x7ffffffe, 0xfffffffd),
            testTableLogicDI<int32>(0xefffffff, 0xfffffffe, 0x1effffffd),
            testTableLogicDI<int32>(-1, -2, -3),
            testTableLogicDI<int32>(0, 0, 0, false)
    };
    DoubleIntegerTest<int32> numbertest;

    ASSERT_TRUE(numbertest.TestSumOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSumOperator_uint32) {

    const testTableLogicDI<uint32> testTable[]={
            testTableLogicDI<uint32>(0x7fffffff, 0xfffffffe, 0x17ffffffd),
            testTableLogicDI<uint32>(0x7fffffff, 0x7ffffffe, 0xfffffffd),
            testTableLogicDI<uint32>(0xefffffff, 0xfffffffe, 0x1effffffd),
            testTableLogicDI<uint32>(-1, -2, -3),
            testTableLogicDI<uint32>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint32> numbertest;

    ASSERT_TRUE(numbertest.TestSumOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSumOperator_int16) {

    const testTableLogicDI<int16> testTable[]={
            testTableLogicDI<int16>(0x7fff, 0xfffe, 0x17ffd),
            testTableLogicDI<int16>(0x7fff, 0x7ffe, 0xfffd),
            testTableLogicDI<int16>(0xefff, 0xfffe, 0x1effd),
            testTableLogicDI<int16>(-1, -2, -3),
            testTableLogicDI<int16>(0, 0, 0, false)
    };
    DoubleIntegerTest<int16> numbertest;

    ASSERT_TRUE(numbertest.TestSumOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSumOperator_uint16) {

    const testTableLogicDI<uint16> testTable[]={
            testTableLogicDI<uint16>(0x7fff, 0xfffe, 0x17ffd),
            testTableLogicDI<uint16>(0x7fff, 0x7ffe, 0xfffd),
            testTableLogicDI<uint16>(0xefff, 0xfffe, 0x1effd),
            testTableLogicDI<uint16>(-1, -2, -3),
            testTableLogicDI<uint16>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint16> numbertest;

    ASSERT_TRUE(numbertest.TestSumOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSumOperator_int8) {

    const testTableLogicDI<int8> testTable[]={
            testTableLogicDI<int8>(0x7f, 0xfe, 0x17d),
            testTableLogicDI<int8>(0x7f, 0x7e, 0xfd),
            testTableLogicDI<int8>(0xef, 0xfe, 0x1ed),
            testTableLogicDI<int8>(-1, -2, -3),
            testTableLogicDI<int8>(0, 0, 0, false)
    };
    DoubleIntegerTest<int8> numbertest;

    ASSERT_TRUE(numbertest.TestSumOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSumOperator_uint8) {

    const testTableLogicDI<uint8> testTable[]={
            testTableLogicDI<uint8>(0x7f, 0xfe, 0x17d),
            testTableLogicDI<uint8>(0x7f, 0x7e, 0xfd),
            testTableLogicDI<uint8>(0xef, 0xfe, 0x1ed),
            testTableLogicDI<uint8>(-1, -2, -3),
            testTableLogicDI<uint8>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint8> numbertest;

    ASSERT_TRUE(numbertest.TestSumOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSubOperator_int64) {

    const testTableLogicDI<int64> testTable[]={
            testTableLogicDI<int64>(DoubleInteger<int64>(0xf,0x7fffffffffffffff), DoubleInteger<int64>(0xe,0xfffffffffffffffe), 0x8000000000000001),
            testTableLogicDI<int64>(DoubleInteger<int64>(0xf,0x6fffffffffffffff), DoubleInteger<int64>(0xe,0x7ffffffffffffffe), 0xf000000000000001),
            testTableLogicDI<int64>(DoubleInteger<int64>(0xf,0xffffffffffffffff), DoubleInteger<int64>(0xe,0x7ffffffffffffffe), DoubleInteger<int64>(0x1,0x8000000000000001)),
            testTableLogicDI<int64>(DoubleInteger<int64>(0xf,0xefffffffffffffff), DoubleInteger<int64>(0xe,0xfffffffffffffffe), 0xf000000000000001),
            testTableLogicDI<int64>(-2, -1, -1),
            testTableLogicDI<int64>(0, 0, 0, false)
    };
    DoubleIntegerTest<int64> numbertest;

    ASSERT_TRUE(numbertest.TestSubOperator(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSubOperator_uint64) {

    const testTableLogicDI<uint64> testTable[]={
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0xf,0x7fffffffffffffff), DoubleInteger<uint64>(0xe,0xfffffffffffffffe), 0x8000000000000001),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0xf,0x6fffffffffffffff), DoubleInteger<uint64>(0xe,0x7ffffffffffffffe), 0xf000000000000001),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0xf,0xffffffffffffffff), DoubleInteger<uint64>(0xe,0x7ffffffffffffffe), DoubleInteger<uint64>(0x1,0x8000000000000001)),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0xf,0xefffffffffffffff), DoubleInteger<uint64>(0xe,0xfffffffffffffffe), 0xf000000000000001),
            testTableLogicDI<uint64>(-2, -1, -1),
            testTableLogicDI<uint64>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint64> numbertest;

    ASSERT_TRUE(numbertest.TestSubOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSubOperator_int32) {

    const testTableLogicDI<int32> testTable[]={
            testTableLogicDI<int32>(0xf7fffffff, 0xefffffffe, 0x80000001),
            testTableLogicDI<int32>(0xf6fffffff, 0xe7ffffffe, 0xf0000001),
            testTableLogicDI<int32>(0xfffffffff, 0xe7ffffffe, 0x180000001),
            testTableLogicDI<int32>(0xfefffffff, 0xefffffffe, 0xf0000001),
            testTableLogicDI<int32>(-2, -1, -1),
            testTableLogicDI<int32>(0, 0, 0, false)
    };
    DoubleIntegerTest<int32> numbertest;

    ASSERT_TRUE(numbertest.TestSubOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSubOperator_uint32) {

    const testTableLogicDI<uint32> testTable[]={
            testTableLogicDI<uint32>(0xf7fffffff, 0xefffffffe, 0x80000001),
            testTableLogicDI<uint32>(0xf6fffffff, 0xe7ffffffe, 0xf0000001),
            testTableLogicDI<uint32>(0xfffffffff, 0xe7ffffffe, 0x180000001),
            testTableLogicDI<uint32>(0xfefffffff, 0xefffffffe, 0xf0000001),
            testTableLogicDI<uint32>(-2, -1, -1),
            testTableLogicDI<uint32>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint32> numbertest;

    ASSERT_TRUE(numbertest.TestSubOperator(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSubOperator_int16) {

    const testTableLogicDI<int16> testTable[]={
            testTableLogicDI<int16>(0xf7fff, 0xefffe, 0x8001),
            testTableLogicDI<int16>(0xf6fff, 0xe7ffe, 0xf001),
            testTableLogicDI<int16>(0xfffff, 0xe7ffe, 0x18001),
            testTableLogicDI<int16>(0xfefff, 0xefffe, 0xf001),
            testTableLogicDI<int16>(-2, -1, -1),
            testTableLogicDI<int16>(0, 0, 0, false)
    };
    DoubleIntegerTest<int16> numbertest;

    ASSERT_TRUE(numbertest.TestSubOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSubOperator_uint16) {

    const testTableLogicDI<uint16> testTable[]={
            testTableLogicDI<uint16>(0xf7fff, 0xefffe, 0x8001),
            testTableLogicDI<uint16>(0xf6fff, 0xe7ffe, 0xf001),
            testTableLogicDI<uint16>(0xfffff, 0xe7ffe, 0x18001),
            testTableLogicDI<uint16>(0xfefff, 0xefffe, 0xf001),
            testTableLogicDI<uint16>(-2, -1, -1),
            testTableLogicDI<uint16>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint16> numbertest;

    ASSERT_TRUE(numbertest.TestSubOperator(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestSubOperator_int8) {

    const testTableLogicDI<int8> testTable[]={
            testTableLogicDI<int8>(0xf7f, 0xefe, 0x81),
            testTableLogicDI<int8>(0xf6f, 0xe7e, 0xf1),
            testTableLogicDI<int8>(0xfff, 0xe7e, 0x181),
            testTableLogicDI<int8>(0xfef, 0xefe, 0xf1),
            testTableLogicDI<int8>(-2, -1, -1),
            testTableLogicDI<int8>(0, 0, 0, false)
    };
    DoubleIntegerTest<int8> numbertest;

    ASSERT_TRUE(numbertest.TestSubOperator(testTable));
}



TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsEqual_int64) {

    const testTableLogicDI<int64> testTable[]={
            testTableLogicDI<int64>(1,1,1),
            testTableLogicDI<int64>(1,0,0),
            testTableLogicDI<int64>(-2, -1, 0),
            testTableLogicDI<int64>(-2, -2, 1),
            testTableLogicDI<int64>(DoubleInteger<int64>(0x12345678912,0x12345678912), DoubleInteger<int64>(0x12345678912,0x12345678912), 1),
            testTableLogicDI<int64>(0, 0, 0, false)
    };
    DoubleIntegerTest<int64> numbertest;

    ASSERT_TRUE(numbertest.TestIsEqual(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsEqual_uint64) {

    const testTableLogicDI<uint64> testTable[]={
            testTableLogicDI<uint64>(1,1,1),
            testTableLogicDI<uint64>(1,0,0),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(-1,-2), DoubleInteger<uint64>(-1,-1), 0),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(-1,-2), DoubleInteger<uint64>(-1,-2), 1),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0x12345678912,0x12345678912), DoubleInteger<uint64>(0x12345678912,0x12345678912), 1),
            testTableLogicDI<uint64>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint64> numbertest;

    ASSERT_TRUE(numbertest.TestIsEqual(testTable));
}



TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsEqual_int32) {

    const testTableLogicDI<int32> testTable[]={
            testTableLogicDI<int32>(1,1,1),
            testTableLogicDI<int32>(1,0,0),
            testTableLogicDI<int32>(0xfffffffff, 0xffffffffe, 0),
            testTableLogicDI<int32>(0xfefffffff, 0xfefffffff, 1),
            testTableLogicDI<int32>(-2, -1, 0),
            testTableLogicDI<int32>(-2, -2, 1),
            testTableLogicDI<int32>(0x12345678912, 0x12345678912, 1),
            testTableLogicDI<int32>(0, 0, 0, false)
    };
    DoubleIntegerTest<int32> numbertest;

    ASSERT_TRUE(numbertest.TestIsEqual(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsEqual_uint32) {

    const testTableLogicDI<uint32> testTable[]={
            testTableLogicDI<uint32>(1,1,1),
            testTableLogicDI<uint32>(1,0,0),
            testTableLogicDI<uint32>(0xfffffffff, 0xffffffffe, 0),
            testTableLogicDI<uint32>(0xfefffffff, 0xfefffffff, 1),
            testTableLogicDI<uint32>(-2, -1, 0),
            testTableLogicDI<uint32>(-2, -2, 1),
            testTableLogicDI<uint32>(12345678912, 12345678912, 1),
            testTableLogicDI<uint32>(0xfffffffffffffffe, 0xfffffffffffffffe, 1),
            testTableLogicDI<uint32>(0x123f4f2f3e, 0x123f4f2f3e, 1),
            testTableLogicDI<uint32>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint32> numbertest;

    ASSERT_TRUE(numbertest.TestIsEqual(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsEqual_int16) {

    const testTableLogicDI<int16> testTable[]={
            testTableLogicDI<int16>(1,1,1),
            testTableLogicDI<int16>(1,0,0),
            testTableLogicDI<int16>(-2, -1, 0),
            testTableLogicDI<int16>(-2, -2, 1),
            testTableLogicDI<int16>(0x123456, 0x123456, 1),
            testTableLogicDI<int16>(0, 0, 0, false)
    };
    DoubleIntegerTest<int16> numbertest;

    ASSERT_TRUE(numbertest.TestIsEqual(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsEqual_uint16) {

    const testTableLogicDI<uint16> testTable[]={
            testTableLogicDI<uint16>(1,1,1),
            testTableLogicDI<uint16>(1,0,0),
            testTableLogicDI<uint16>(-2, -1, 0),
            testTableLogicDI<uint16>(-2, -2, 1),
            testTableLogicDI<uint16>(0x123456, 0x123456, 1),
            testTableLogicDI<uint16>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint16> numbertest;

    ASSERT_TRUE(numbertest.TestIsEqual(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsEqual_int8) {

    const testTableLogicDI<int8> testTable[]={
            testTableLogicDI<int8>(1,1,1),
            testTableLogicDI<int8>(1,0,0),
            testTableLogicDI<int8>(-2, -1, 0),
            testTableLogicDI<int8>(-2, -2, 1),
            testTableLogicDI<int8>(0x123, 0x123, 1),
            testTableLogicDI<int8>(0, 0, 0, false)
    };
    DoubleIntegerTest<int8> numbertest;

    ASSERT_TRUE(numbertest.TestIsEqual(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsEqual_uint8) {

    const testTableLogicDI<uint8> testTable[]={
            testTableLogicDI<uint8>(1,1,1),
            testTableLogicDI<uint8>(1,0,0),
            testTableLogicDI<uint8>(-2, -1, 0),
            testTableLogicDI<uint8>(-2, -2, 1),
            testTableLogicDI<uint8>(0x123, 0x123, 1),
            testTableLogicDI<uint8>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint8> numbertest;

    ASSERT_TRUE(numbertest.TestIsEqual(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsDifferent_int64) {

    const testTableLogicDI<int64> testTable[]={
            testTableLogicDI<int64>(1,1,0),
            testTableLogicDI<int64>(1,0,1),
            testTableLogicDI<int64>(-2, -1, 1),
            testTableLogicDI<int64>(-2, -2, 0),
            testTableLogicDI<int64>(DoubleInteger<int64>(0x12345678912,0x12345678912), DoubleInteger<int64>(0x12345678912,0x12345678912), 0),
            testTableLogicDI<int64>(0, 0, 0, false)
    };
    DoubleIntegerTest<int64> numbertest;

    ASSERT_TRUE(numbertest.TestIsDifferent(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsDifferent_uint64) {

    const testTableLogicDI<uint64> testTable[]={
            testTableLogicDI<uint64>(1,1,0),
            testTableLogicDI<uint64>(1,0,1),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(-1,-2), DoubleInteger<uint64>(-1,-1), 1),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(-1,-2), DoubleInteger<uint64>(-1,-2), 0),
            testTableLogicDI<uint64>(DoubleInteger<uint64>(0x12345678912,0x12345678912), DoubleInteger<uint64>(0x12345678912,0x12345678912), 0),
            testTableLogicDI<uint64>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint64> numbertest;

    ASSERT_TRUE(numbertest.TestIsDifferent(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsDifferent_int32) {

    const testTableLogicDI<int32> testTable[]={
            testTableLogicDI<int32>(1,1,0),
            testTableLogicDI<int32>(1,0,1),
            testTableLogicDI<int32>(0xfffffffff, 0xffffffffe, 1),
            testTableLogicDI<int32>(0xfefffffff, 0xfefffffff, 0),
            testTableLogicDI<int32>(-2, -1, 1),
            testTableLogicDI<int32>(-2, -2, 0),
            testTableLogicDI<int32>(0x12345678912, 0x12345678912, 0),
            testTableLogicDI<int32>(0, 0, 0, false)
    };
    DoubleIntegerTest<int32> numbertest;

    ASSERT_TRUE(numbertest.TestIsDifferent(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsDifferent_uint32) {

    const testTableLogicDI<uint32> testTable[]={
            testTableLogicDI<uint32>(1,1,0),
            testTableLogicDI<uint32>(1,0,1),
            testTableLogicDI<uint32>(0xfffffffff, 0xffffffffe, 1),
            testTableLogicDI<uint32>(0xfefffffff, 0xfefffffff, 0),
            testTableLogicDI<uint32>(-2, -1, 1),
            testTableLogicDI<uint32>(-2, -2, 0),
            testTableLogicDI<uint32>(0x12345678912, 0x12345678912, 0),
            testTableLogicDI<uint32>(0xfffffffffffffffe, 0xfffffffffffffffe, 0),
            testTableLogicDI<uint32>(0x123f4f2f3e, 0x123f4f2f3e, 0),
            testTableLogicDI<uint32>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint32> numbertest;

    ASSERT_TRUE(numbertest.TestIsDifferent(testTable));
}




TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsDifferent_int16) {

    const testTableLogicDI<int16> testTable[]={
            testTableLogicDI<int16>(1,1,0),
            testTableLogicDI<int16>(1,0,1),
            testTableLogicDI<int16>(-2, -1, 1),
            testTableLogicDI<int16>(-2, -2, 0),
            testTableLogicDI<int16>(0x123456, 0x123456, 0),
            testTableLogicDI<int16>(0, 0, 0, false)
    };
    DoubleIntegerTest<int16> numbertest;

    ASSERT_TRUE(numbertest.TestIsDifferent(testTable));
}


TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsDifferent_uint16) {

    const testTableLogicDI<uint16> testTable[]={
            testTableLogicDI<uint16>(1,1,0),
            testTableLogicDI<uint16>(1,0,1),
            testTableLogicDI<uint16>(-2, -1, 1),
            testTableLogicDI<uint16>(-2, -2, 0),
            testTableLogicDI<uint16>(0x123456, 0x123456, 0),
            testTableLogicDI<uint16>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint16> numbertest;

    ASSERT_TRUE(numbertest.TestIsDifferent(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsDifferent_int8) {

    const testTableLogicDI<int8> testTable[]={
            testTableLogicDI<int8>(1,1,0),
            testTableLogicDI<int8>(1,0,1),
            testTableLogicDI<int8>(-2, -1, 1),
            testTableLogicDI<int8>(-2, -2, 0),
            testTableLogicDI<int8>(0x123, 0x123, 0),
            testTableLogicDI<int8>(0, 0, 0, false)
    };
    DoubleIntegerTest<int8> numbertest;

    ASSERT_TRUE(numbertest.TestIsDifferent(testTable));
}

TEST(BareMetal_L0Types_DoubleIntegerGTest,TestIsDifferent_uint8) {

    const testTableLogicDI<uint8> testTable[]={
            testTableLogicDI<uint8>(1,1,0),
            testTableLogicDI<uint8>(1,0,1),
            testTableLogicDI<uint8>(-2, -1, 1),
            testTableLogicDI<uint8>(-2, -2, 0),
            testTableLogicDI<uint8>(0x123, 0x123, 0),
            testTableLogicDI<uint8>(0, 0, 0, false)
    };
    DoubleIntegerTest<uint8> numbertest;

    ASSERT_TRUE(numbertest.TestIsDifferent(testTable));
}


