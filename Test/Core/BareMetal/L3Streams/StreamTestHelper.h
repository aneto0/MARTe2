#include "DoubleBufferedStream.h"
#include "StreamString.h"
#include "MemoryOperationsHelper.h"
#include "RawStream.h"

#define MAX_STREAM_DIMENSION 512
using namespace MARTe;
/**
 * @file StreamTestHelper.h
 * @brief Implements classes used for stream tests.
 */

/**
 * @brief A generic stream class used for types prints.
 *
 * Inherits from StreamString and allows to get the buffer for the print operations.*/
class MyStream: public StreamString {

public:
    /** @brief Return the write buffer */
    IOBuffer *GetMyOutputBuffer() {
        return GetOutputBuffer();
    }

    /** @brief Clean the string */
    bool Clear() {
        StreamString* me = this;
        return (*me) = "";
    }

};

class DummyRawStream: public RawStream {

public:

    DummyRawStream() {
        position = 0;
        seekable = false;
        size = 0;
        for (uint32 i = 0; i < MAX_STREAM_DIMENSION; i++) {
            buffer[i] = 0;
        }
    }

    /**
     * @brief Returns the current stream position.
     * @return the current stream position.
     */
    int64 UnbufferedSize() const {
        return size;
    }

    /**
     * @brief Moves to the desired position.
     * @param seek is the desired absolute position.
     * @return true. */
    bool UnbufferedSeek(int64 seek) {
        position = seek;
        return true;
    }

    /**
     * @brief Returns the current stream position.
     * @return the current stream position.
     */
    int64 UnbufferedPosition() const {
        return position;
    }

    /**
     * @brief Sets the stream size.
     * @param desSize unused here.
     * @return true. */
    bool UnbufferedSetSize(int64 desSize) {
        return true;
    }

    /** 
     * @brief Read from the stream to the output buffer. 
     * @param outBuffer is the output buffer.
     * @param inSize is the number of bytes to read.
     * @param timeout is unused here.
     * @param complete is unused here.
     * @return true if successful, false otherwise. */
    bool UnbufferedRead(char* outBuffer,
                        uint32 &inSize,
                        TimeoutType timeout = TTDefault,
                        bool complete = false) {

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
    bool UnbufferedWrite(const char* inBuffer,
                         uint32 &outSize,
                         TimeoutType timeout = TTDefault,
                         bool complete = false) {

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

    /** @brief Clean the buffer. */
    void Clear() {
        position = 0;
        size = 0;
        for (int i = 0; i < MAX_STREAM_DIMENSION; i++) {
            buffer[i] = 0;
        }
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





