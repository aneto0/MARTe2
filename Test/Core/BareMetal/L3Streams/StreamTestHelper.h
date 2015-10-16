#include "DoubleBufferedStream.h"
#include "SingleBufferedStream.h"
#include "StreamString.h"
#include "MemoryOperationsHelper.h"
#include "RawStream.h"
#include "AnyType.h"
#include "stdio.h"

#define MAX_STREAM_DIMENSION 512
using namespace MARTe;

class PrintfNode {

public:
    AnyType inputs[4];
    const char8* expectedResult;
    const char8* format;

    PrintfNode(const char8* formatIn,
               const char8* expectedResultIn,
               const AnyType x0=voidAnyType,
               const AnyType x1=voidAnyType,
               const AnyType x2=voidAnyType,
               const AnyType x3=voidAnyType) {


        inputs[0]=x0;
        inputs[1]=x1;
        inputs[2]=x2;
        inputs[3]=x3;

        expectedResult = expectedResultIn;
        format = formatIn;

    }

};

static const uint32 numberOfIntegers=32;
static const uint32 numberOfFloats=64;


static inline uint8 *SaveNumber(uint8 number) {
    static uint8 numberx[numberOfIntegers];
    static int32 index = -1;

    for (int32 i = index; i >= 0; i--) {
        if(numberx[i]==number){
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
        if(numberx[i]==number){
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
        if(numberx[i]==number){
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
        if(numberx[i]==number){
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
        if(numberx[i]==number){
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
        if(numberx[i]==number){
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
        if(numberx[i]==number){
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
        if(numberx[i]==number){
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
        if(numberx[i]==number){
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
        if(numberx[i]==number){
            return &numberx[i];
        }
    }

    //the number is not already saved
    index++;
    numberx[index] = number;
    return &numberx[index];
}


static const PrintfNode printfDecTable[]= {
    //integers
    //decimal notations
    PrintfNode("%d", "255",*SaveNumber((uint8)255)),
    PrintfNode("%5d", "255",*SaveNumber((uint8)255)),
    PrintfNode("% 5d","  255",*SaveNumber((uint8)255)),
    PrintfNode("%-5d","255  ",*SaveNumber((uint8)255)),
    PrintfNode("%d","-127",*SaveNumber((int8)-127)),
    PrintfNode("%-6i","-127  ",*SaveNumber((int8)-127)),
    PrintfNode("%d","-128",*SaveNumber((int8)0x80)),
    PrintfNode("%u","12345",*SaveNumber((uint16)12345)),
    PrintfNode("%-#8d","+12345  ",*SaveNumber((uint16)12345)),
    PrintfNode("%d","-12345",*SaveNumber((int16)-12345)),
    PrintfNode("%i","-32768",*SaveNumber((int16)0x8000)),
    PrintfNode("%d","123456789",*SaveNumber((uint32)123456789)),
    PrintfNode("% #11u"," +123456789",*SaveNumber((uint32)123456789)),
    PrintfNode("%i","-123456789",*SaveNumber((int32)-123456789)),
    PrintfNode("%i","-2147483648",*SaveNumber((int32)0x80000000)),
    PrintfNode("%u","12345678912345678",*SaveNumber((uint64)12345678912345678)),
    PrintfNode("%i","-12345678912345678",*SaveNumber((int64)-12345678912345678)),
    PrintfNode("%i","-9223372036854775808",*SaveNumber((int64)0x8000000000000000)),
    PrintfNode("% #11d","          ?",*SaveNumber((int64)0x8000000000000000)),
    PrintfNode("","")
};

 static const PrintfNode printfHexTable[]={
 //exadecimal notation
         PrintfNode("%x","EA",*SaveNumber((uint8)0xea)),
         PrintfNode("%x","EA",*SaveNumber((int8)0xea)),
         PrintfNode("%5x","EA",*SaveNumber((int8)0xea)),
         PrintfNode("%#0x","0xEA",*SaveNumber((int8)0xea)),
         PrintfNode("%0x","0F",*SaveNumber((uint8)0xf)),
         PrintfNode("%-#07x","0xABCD ",*SaveNumber((uint16)0xabcd)),
         PrintfNode("%#05x","0x0CD",*SaveNumber((uint16)0xcd)),
         PrintfNode("% #12x","  0xABCDEF78",*SaveNumber((uint32)0xabcdef78)),
         PrintfNode("% #011x"," 0x00ABCD0F",*SaveNumber((uint32)0x00abcd0f)),
         PrintfNode("% 11x","     ABCD0F",*SaveNumber((uint32)0x00abcd0f)),
         PrintfNode("%120x","89ABCDEF0123FFF0",*SaveNumber((uint64)0x89abcdef0123fff0)),
         PrintfNode("% 5x","    ?",*SaveNumber((uint64)0x123fff0)),
         PrintfNode("%-07x","000FFF0",*SaveNumber((int64)0xfff0)),
         PrintfNode("","")
 };

 static const PrintfNode printfOctTable[]={
 //exadecimal notation
         PrintfNode("%o","352",*SaveNumber((uint8)0xea)),
         PrintfNode("%o","352",*SaveNumber((int8)-22)),
         PrintfNode("%5o","352",*SaveNumber((int8)-22)),
         PrintfNode("%#0o","0o352",*SaveNumber((int8)-22)),
         PrintfNode("%0o","017",*SaveNumber((uint8)0xf)),
         PrintfNode("%-#09o","0o075715 ",*SaveNumber((uint16)0x7bcd)),
         PrintfNode("%#07o","0o00315",*SaveNumber((uint16)0xcd)),
         PrintfNode("% #15o","  0o25363367570",*SaveNumber((uint32)0xabcdef78)),
         PrintfNode("% #015o","  0o00000146417",*SaveNumber((uint32)0xcd0f)),
         PrintfNode("% 15o","         146417",*SaveNumber((uint32)0xcd0f)),
         PrintfNode("%120o","1046536336740111000000",*SaveNumber((uint64)0x89abcdef01240000)),
         PrintfNode("% 5o","    ?",*SaveNumber((int64)0x123fff0)),
         PrintfNode("%-07o","0177760",*SaveNumber((int64)0xfff0)),
         PrintfNode("","")
 };



 static const PrintfNode printfBinTable[]={
         PrintfNode("%b","11101010",*SaveNumber((uint8)0xea)),
         PrintfNode("%b","11101010",*SaveNumber((int8)-22)),
         PrintfNode("%50b","11101010",*SaveNumber((int8)-22)),
         PrintfNode("%#0b","0b11101010",*SaveNumber((int8)-22)),
         PrintfNode("%0b","00001111",*SaveNumber((uint8)0xf)),
         PrintfNode("%-#019b","0b0111101111001101 ",*SaveNumber((uint16)0x7bcd)),
         PrintfNode("%#012b","0b0011001101",*SaveNumber((int16)0xcd)),
         PrintfNode("% #36b","  0b10101011110011011110111101111000",*SaveNumber((uint32)0xabcdef78)),
         PrintfNode("% 034b","  00000000000000001100110100001111",*SaveNumber((uint32)0xcd0f)),
         PrintfNode("% 34b","                  1100110100001111",*SaveNumber((uint32)0xcd0f)),
         PrintfNode("%120b","1000000000000000000000000000000000000000000000000000000000000001",*SaveNumber((uint64)0x8000000000000001)),
         PrintfNode("% 5b","    ?",*SaveNumber((uint64)0x123fff0)),
         PrintfNode("%-014b","00111111110000",*SaveNumber((int64)0xff0)),
         PrintfNode("","")
 };

 static const PrintfNode printfFloatFPTable[]={
         PrintfNode("%f","-1.123457",*SaveNumber(-1.1234567f)),
         PrintfNode("%- 12.7f","-1.1234567  ",*SaveNumber(-1.1234567f)),
         PrintfNode("%- 12.3f","-1.123      ",*SaveNumber(-1.1234567f)),
         PrintfNode("% 12.2f","   112345.67",*SaveNumber(112345.67f)),
         PrintfNode("% 3.2f","  0",*SaveNumber((float32)0)),
         PrintfNode("% 1.2f","0",*SaveNumber(0.09f)),
         PrintfNode("% 10.3f"," 12346.000",*SaveNumber(12345.9999)),
         PrintfNode("% 10.0f","     12346",*SaveNumber(12345.9999)),
         PrintfNode("% 17.9f"," 999999.999960000",*SaveNumber(999999.99996)),
         PrintfNode("% 6.4f","     ?",*SaveNumber(999999.99996)),
         PrintfNode("%-8.4f","1000000 ",*SaveNumber(999999.99996)),
         PrintfNode("%-9.4f","1000000.0",*SaveNumber(999999.99996)),
         PrintfNode("%7.4f","-222223",*SaveNumber(-222222.5255)),
         PrintfNode("% 8.4f"," -222223",*SaveNumber(-222222.5255)),
         PrintfNode("% 9.4f","-222222.5",*SaveNumber(-222222.5255)),
         PrintfNode("% 10.4f","-222222.53",*SaveNumber(-222222.5255)),
         PrintfNode("% 5.4f","    ?",*SaveNumber(-222222.5255)),
         PrintfNode("% 5.2f"," 0.01",*SaveNumber(0.009)),
         PrintfNode("% 12.4f","           0",*SaveNumber(1e-12)),
         PrintfNode("% 1.10f","0",*SaveNumber(-1e-12)),
         PrintfNode("%10.10f","+Inf",*SaveNumber((float64)(1.0 / 0.0))),
         PrintfNode("% 10.10f","      -Inf",*SaveNumber((float64)(-1.0 / 0.0))),
         PrintfNode("%-1.10f","?",*SaveNumber((float32)(-1.0 / 0.0))),
         PrintfNode("%-10.10f","NaN       ",*SaveNumber((float64)(0.0 / 0.0))),
         PrintfNode("% 0.4f","NaN",*SaveNumber((float32)(0.0 / 0.0))),
         PrintfNode("% 2.10f"," ?",*SaveNumber((float64)(0.0 / 0.0))),
         PrintfNode("","")
 };


 static const PrintfNode printfFloatFPRTable[]={
         PrintfNode("%-12.8F","-1.1234567  ",*SaveNumber(-1.1234567f)),
         PrintfNode("%-12F","-1.123457   ",*SaveNumber(-1.1234567f)),
         PrintfNode("%-12.0F","0           ",*SaveNumber(-1.1234567f)),
         PrintfNode("%-12.4F","-1.123      ",*SaveNumber(-1.1234567f)),
         PrintfNode("% 12.8F","   112345.67",*SaveNumber(112345.67f)),
         PrintfNode("% 10.8F"," 12346.000",*SaveNumber(12345.9999)),
         PrintfNode("% 10.3F","     12300",*SaveNumber(12345.9999)),
         PrintfNode("% 17.15F"," 999999.999960000",*SaveNumber(999999.99996)),
         PrintfNode("% 6.10F","     ?",*SaveNumber(999999.99996)),
         PrintfNode("%-8.10F","1000000 ",*SaveNumber(999999.99996)),
         PrintfNode("%-9.10F","1000000.0",*SaveNumber(999999.99996)),
         PrintfNode("%7.10F","-222223",*SaveNumber(-222222.5255)),
         PrintfNode("% 8.10F"," -222223",*SaveNumber(-222222.5255)),
         PrintfNode("% 9.10F","-222222.5",*SaveNumber(-222222.5255)),
         PrintfNode("% 10.10F","-222222.53",*SaveNumber(-222222.5255)),
         PrintfNode("% 5.10F","    ?",*SaveNumber(-222222.5255)),
         PrintfNode("% 12.12F","           0",*SaveNumber(1e-12)),
         PrintfNode("%10.10F","+Inf",*SaveNumber((float32)1.0 / 0.0)),
         PrintfNode("% 10.10F","      -Inf",*SaveNumber((float64)-1.0 / 0.0)),
         PrintfNode("%-1.10F","?",*SaveNumber((float64)-1.0 / 0.0)),
         PrintfNode("%-10.10F","NaN       ",*SaveNumber((float64)0.0 / 0.0)),
         PrintfNode("% 0.10F","NaN",*SaveNumber((float64)0.0 / 0.0)),
         PrintfNode("% 2.10F"," ?",*SaveNumber((float64)-1.0 / 0.0)),
         PrintfNode("","")
 };

 static const PrintfNode printfFloatExpTable[]={
         PrintfNode("%-15.8e","-1.1234567E+1  ",*SaveNumber(-11.234567f)),
         PrintfNode("%-9.4e","-1.123E+1",*SaveNumber(-11.234567f)),
         PrintfNode("% 10.8e","1.12346E+5",*SaveNumber(112345.67f)),
         PrintfNode("% 21.e"," -1.23459999000000E+4",*SaveNumber(-12345.9999)),
         PrintfNode("% 13.8e","-1.2346000E+4",*SaveNumber(-12345.9999)),
         PrintfNode("%.6e","1.00000E+6",*SaveNumber(999999.55556)),
         PrintfNode("%-5.6e","1E+6 ",*SaveNumber(999999.55556)),
         PrintfNode("%-3.2e","?  ",*SaveNumber(999999.55556)),
         PrintfNode("%-4.2e","1E-9",*SaveNumber(9.5e-10)),
         PrintfNode("%-4.2e","?   ",*SaveNumber(9.4999e-10)),
       // quad float
       //  PrintfNode("%-6.2e","1E-100",*SaveNumber(9.9999e-101)),
       //  PrintfNode("%-7.2e","1E+1000",*SaveNumber(9.9999e+999)),
         PrintfNode("","")

 };


 static const PrintfNode printfFloatEngTable[]={
         PrintfNode("%.9E","-11.2345671",*SaveNumber(-11.234567f)),
         PrintfNode("%-10.4E","-1.123E+3 ",*SaveNumber(-1123.4567f)),
         PrintfNode("% 10.8E","112.346E+3",*SaveNumber(112345.67f)),
         PrintfNode("% 10.9E","12.3460E+3",*SaveNumber(12345.9999)),
         PrintfNode("% 3.3E"," -1",*SaveNumber(-0.9999)),
         PrintfNode("% 3.5E"," -1",*SaveNumber(-0.9999)),
         PrintfNode("% 20.10E","      1.000000000E+6",*SaveNumber(999999.99999)),
         PrintfNode("% 19E","999.999999990000E+3",*SaveNumber(999999.9999900000000000000000000000)),
         PrintfNode("%-7.3E","-10E+9 ",*SaveNumber(-9.99999e9)),
         PrintfNode("% 6.4E","     ?",*SaveNumber(-99.9999e9)),
         PrintfNode("% 6.6E","     ?",*SaveNumber(-99.9999e9)),
         PrintfNode("%5.2E","?",*SaveNumber(-999.999e9)),
         PrintfNode("% 4.10E","1E+3",*SaveNumber(999.9)),
         PrintfNode("% 3.10E","  ?",*SaveNumber(999.9)),
         PrintfNode("% 4.10E","1E+6",*SaveNumber(999.9e3)),
         PrintfNode("% 4.10E","1E-3",*SaveNumber(950.0e-6)),
         PrintfNode("% 4.10E","   ?",*SaveNumber(949.9e-6)),
         PrintfNode("","")
 };

static const PrintfNode printfFloatSmartTable[] = {
        PrintfNode("%.9g","-11.2345671",*SaveNumber(-11.234567f)),
        PrintfNode("%- 10.4g","-1.123K   ",*SaveNumber(-1123.4567f)),
        PrintfNode("% 8.6g", "112.346K",*SaveNumber(112345.67f)),
        PrintfNode("% 10.6g", "  12.3460K",*SaveNumber(12345.9999)),
        PrintfNode("% 3.3g", " -1",*SaveNumber(-0.9999)),
        PrintfNode("% 20.10g", "        1.000000000M",*SaveNumber(999999.99999)),
        PrintfNode("%-7.3g", "-10.0G ",*SaveNumber(-9.99999e9)),
        PrintfNode("%-4.4g", "?   ",*SaveNumber(-99.9999e9)),
        PrintfNode("% 4.5g", " -1T",*SaveNumber(-999.999e9)),
        PrintfNode("% 4.10g", "1.0K",*SaveNumber(999.9)),
        PrintfNode("% 3.10g", " 1M",*SaveNumber(999.9e3)),
        PrintfNode( "%-7.10g", "90E+27 ",*SaveNumber(9e28)),
        PrintfNode("% 9.10g", " -990E-30",*SaveNumber(-9.9e-28)),
        PrintfNode("", "")
};


static const PrintfNode printfFloatCompactTable[] = {
        PrintfNode("%.9G","-11.2345671",*SaveNumber(-11.234567f)),
        PrintfNode("%-10.4G","-1.123K   ",*SaveNumber(-1123.4567f)),
        PrintfNode("% 8.6G","112.346K",*SaveNumber(112345.67f)),
        PrintfNode("% 10.6G","  12.3460K",*SaveNumber(12345.9999)),
        PrintfNode("% 3.3G"," -1",*SaveNumber(-0.9999)),
        PrintfNode("% 20.10G","        1.000000000M",*SaveNumber(999999.99999)),
        PrintfNode("%-7.3G","-10.0G ",*SaveNumber(-9.99999e9)),
        PrintfNode("%-4.4G","?   ",*SaveNumber(-99.9999e9)),
        PrintfNode("% 4.54G"," -1T",*SaveNumber(-999.999e9)),
        PrintfNode("% 4.10G","1.0K",*SaveNumber(999.9)),
        PrintfNode("% 3.10G"," 1M",*SaveNumber(999.9e3)),
        PrintfNode("%-7.10G","9.0E+28",*SaveNumber(9e28)),
        PrintfNode("% 9.10G","-9.90E-28",*SaveNumber(-9.9e-28)),
        PrintfNode("", "")


};

static const PrintfNode printfGenericTable[] = {
        PrintfNode("%.9G %5x%u :) %s","-11.2345671 EA12345 :) Hello!",*SaveNumber(-11.234567f),*SaveNumber((int8)0xea),*SaveNumber((uint16)12345),"Hello!"),
        PrintfNode("", "")
};



class DummyRawStream: public RawStream {

public:

    DummyRawStream() {
        position = 0;
        seekable = true;
        size = 0;
        for (uint32 i = 0; i < MAX_STREAM_DIMENSION; i++) {
            buffer[i] = 0;
        }
    }

    /**
     * @brief Returns the current stream position.
     * @return the current stream position.
     */
    uint64 UnbufferedSize() {
        return size;
    }

    /**
     * @brief Moves to the desired position.
     * @param seek is the desired absolute position.
     * @return true. */
    bool UnbufferedSeek(uint64 seek) {
        position = seek;
        return true;
    }

    bool UnbufferedRelativeSeek(int32 delta) {
        return false;
    }

    /**
     * @brief Returns the current stream position.
     * @return the current stream position.
     */
    uint64 UnbufferedPosition() {
        return position;
    }

    /**
     * @brief Sets the stream size.
     * @param desSize unused here.
     * @return true. */
    bool UnbufferedSetSize(uint64 desSize) {
        return true;
    }

    /** 
     * @brief Read from the stream to the output buffer. 
     * @param outBuffer is the output buffer.
     * @param inSize is the number of bytes to read.
     * @param timeout is unused here.
     * @param complete is unused here.
     * @return true if successful, false otherwise. */
    bool UnbufferedRead(char8 * const outBuffer,
                        uint32 &inSize,
                        const TimeoutType &timeout) {

        if ((position + inSize) >= MAX_STREAM_DIMENSION) {
            inSize = MAX_STREAM_DIMENSION - position - 1;
        }

        if (!MemoryOperationsHelper::Copy(outBuffer, &buffer[position], inSize)) {
            return false;
        }

        position += inSize;
        return true;

    }

    /** 
     * @brief Write from the inBuffer to the stream. 
     * @param inBuffer contains datas to be written on stream.
     * @param outSize is the number of bytes to write.
     * @param timeout is unused here.
     * @param complete is unused here.
     * @return true if successful, false otherwise. */
    bool UnbufferedWrite(const char8 * const inBuffer,
                         uint32 &outSize,
                         const TimeoutType &timeout) {

        if ((size + outSize) >= MAX_STREAM_DIMENSION) {
            outSize = MAX_STREAM_DIMENSION - size - 1;
        }
        if (!MemoryOperationsHelper::Copy(&buffer[position], inBuffer, outSize)) {
            return false;
        }

        position += outSize;

        if (position > size) {
            size = position;
        }

        return true;
    }

    /**
     * @brief Defines if the write operations are allowed on the stream.
     * @return true. */
    bool CanWrite() const {
        return true;
    }

    /**
     * @brief Defines if seek operations are allowed on the stream.
     * @return true if both read and write operations are allowed on the same buffer. */
    bool CanSeek() const {
        return seekable;
    }

    /**
     * @brief Defines if read operations are allowed on the stream.
     * @return true. */
    bool CanRead() const {
        return true;
    }

    bool CanBlock() {
        return false;
    }

    bool SetBlocking(bool flag) {
        return true;
    }

    /** @brief Clean the buffer. */
    void Clear() {
        position = 0;
        size = 0;
        for (int i = 0; i < MAX_STREAM_DIMENSION; i++) {
            buffer[i] = 0;
        }
    }

    char8 * Buffer() {
        return buffer;
    }
private:
    /** first buffer */
    char8 buffer[MAX_STREAM_DIMENSION];

    /** current position of the first buffer */
    uint32 position;

    /** size of the first buffer */
    uint32 size;

    /** true if the buffers are used in read&write mode. */
    bool seekable;

};

class DummySingleBufferedStream: public DummyRawStream, public SingleBufferedStream {
public:

    DummySingleBufferedStream() :
            DummyRawStream(),
            SingleBufferedStream() {
    }

    virtual ~DummySingleBufferedStream() {
    }

    /**
     * @brief Returns the current stream position.
     * @return the current stream position.
     */
    uint64 UnbufferedSize() {
        return DummyRawStream::UnbufferedSize();
    }

    /**
     * @brief Moves to the desired position.
     * @param seek is the desired absolute position.
     * @return true. */
    bool UnbufferedSeek(uint64 seek) {
        return DummyRawStream::UnbufferedSeek(seek);
    }

    bool UnbufferedRelativeSeek(int32 delta) {
        return DummyRawStream::UnbufferedRelativeSeek(delta);
    }

    /**
     * @brief Returns the current stream position.
     * @return the current stream position.
     */
    uint64 UnbufferedPosition() {
        return DummyRawStream::UnbufferedPosition();
    }

    /**
     * @brief Sets the stream size.
     * @param desSize unused here.
     * @return true. */
    bool UnbufferedSetSize(uint64 desSize) {
        return DummyRawStream::UnbufferedSetSize(desSize);
    }

    /**
     * @brief Read from the stream to the output buffer.
     * @param outBuffer is the output buffer.
     * @param inSize is the number of bytes to read.
     * @param timeout is unused here.
     * @param complete is unused here.
     * @return true if successful, false otherwise. */
    bool UnbufferedRead(char8 * const outBuffer,
                        uint32 &inSize) {
        return DummyRawStream::UnbufferedRead(outBuffer, inSize, GetTimeout());
    }

    /**
     * @brief Write from the inBuffer to the stream.
     * @param inBuffer contains datas to be written on stream.
     * @param outSize is the number of bytes to write.
     * @param timeout is unused here.
     * @param complete is unused here.
     * @return true if successful, false otherwise. */
    bool UnbufferedWrite(const char8 * const inBuffer,
                         uint32 &outSize) {

        return DummyRawStream::UnbufferedWrite(inBuffer, outSize, GetTimeout());
    }

    /**
     * @brief Defines if the write operations are allowed on the stream.
     * @return true. */
    bool CanWrite() const {
        return DummyRawStream::CanWrite();
    }

    /**
     * @brief Defines if seek operations are allowed on the stream.
     * @return true if both read and write operations are allowed on the same buffer. */
    bool CanSeek() const {
        return DummyRawStream::CanSeek();
    }

    /**
     * @brief Defines if read operations are allowed on the stream.
     * @return true. */
    bool CanRead() const {
        return DummyRawStream::CanRead();
    }

    bool CanBlock() {
        return DummyRawStream::CanBlock();
    }

    bool SetBlocking(bool flag) {
        return DummyRawStream::SetBlocking(flag);
    }

};

