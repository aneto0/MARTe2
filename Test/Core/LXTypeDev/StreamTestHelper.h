#include "StreamString.h"
#include "BufferedStream.h"
#include "Memory.h"
#include "StreamWrapperIOBuffer.h"
#define MAX_STREAM_DIMENSION 512

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

/**
 * @brief A simple stream for BufferedStream tests.
 *
 * Inherits from BufferedStream which is abstract and implements UnBuffered functions. */
class SimpleBufferedStream: public BufferedStream {
public:
    /** first buffer */
    char buffer1[MAX_STREAM_DIMENSION];

    /** second buffer */
    char buffer2[MAX_STREAM_DIMENSION];

    /** a pointer to the current buffer */
    char* buffer;

    /** current position of the first buffer */
    uint32 position1;

    /** current position of the second buffer */
    uint32 position2;

    /** pointer to the position of the current buffer */
    uint32 *positionPtr;

    /** size of the first buffer */
    uint32 size1;
 
    /** size of the second buffer */
    uint32 size2;

    /** pointer to the size of the current buffer */
    uint32 *sizePtr;

    /** number of the current used buffer. */
    uint32 nBuffers;

    /** true if the buffers are used in read&write mode. */
    bool seekable;
public:

    /** @brief Default constructor. */
    SimpleBufferedStream() {
        position1 = 0;
        position2 = 0;
	size1 = 0;
	size2 = 0;
        nBuffers = 2;
        buffer = buffer1;
        positionPtr = &position1;
	sizePtr = &size1;
        seekable = false;
        for (int i = 0; i < MAX_STREAM_DIMENSION; i++)
            buffer[i] = 0;
    }

    /** @brief Default destructor. */
    ~SimpleBufferedStream() {
        FlushAndResync();
    }

    /** @brief Clean the buffer. */
    void Clear() {
        (*positionPtr) = 0;
	(*sizePtr) = 0;
        for (int i = 0; i < MAX_STREAM_DIMENSION; i++)
            buffer[i] = 0;
    }

    /** 
     * @brief Read from the stream to the output buffer. 
     * @param outBuffer is the output buffer.
     * @param inSize is the number of bytes to read.
     * @param timeout is unused here.
     * @param complete is unused here.
     * @return true if successful, false otherwise. */
    bool UnBufferedRead(char* outBuffer, uint32 &inSize, TimeoutType timeout =
                                TTDefault,
                        bool complete = false) {

        uint32 position = *positionPtr;
        if ((position + inSize) >= MAX_STREAM_DIMENSION) {
            inSize = MAX_STREAM_DIMENSION - position - 1;
        }

        if (!MemoryCopy(outBuffer, &buffer[position], inSize)) {
            return false;
        }

        position += inSize;
        *positionPtr = position;
        return true;

    }

    /** 
     * @brief Write from the inBuffer to the stream. 
     * @param inBuffer contains datas to be written on stream.
     * @param outSize is the number of bytes to write.
     * @param timeout is unused here.
     * @param complete is unused here.
     * @return true if successful, false otherwise. */
    bool UnBufferedWrite(const char* inBuffer, uint32 &outSize,
                         TimeoutType timeout = TTDefault,
                         bool complete = false) {

        uint32 position = *positionPtr;
        if (((*sizePtr) + outSize) >= MAX_STREAM_DIMENSION) {
            outSize = MAX_STREAM_DIMENSION - (*sizePtr) -1;
        }
        if (!MemoryCopy(&buffer[position], inBuffer, outSize)) {
            return false;
        }


        position += outSize;
        *positionPtr = position;
	
	if (position > *sizePtr) *sizePtr = position;	
	

        return true;
    }

    /**
     * @brief Sets the read and write buffer dimensions at defSize.
     * @return true if successful, false otherwise. */
    bool SetBuffered(uint32 defSize) {
        return SetBufferSize(defSize, defSize);
    }

    /**
     * @brief Returns the current stream position.
     * @return the current stream position. */
    int64 UnBufferedSize() const {
        return *sizePtr;
    }

    /**
     * @brief Moves to the desired position.
     * @param seek is the desired absolute position.
     * @return true. */
    bool UnBufferedSeek(int64 seek) {
        (*positionPtr) = seek;
        return true;
    }

    /** 
     * @brief Returns the current stream position.
     * @return the current stream position.
     */
    int64 UnBufferedPosition() const {
        return *positionPtr;
    }

    /**
     * @brief Sets the stream size.
     * @param desSize unused here.
     * @return true. */
    bool UnBufferedSetSize(int64 desSize) {
        return true;
    }

    /**
     * @brief Switches from another stream.
     * @param a is the number of the desired stream.
     * @return true. */
    bool UnBufferedSwitch(uint32 a) {
        if (a == 2) {
            buffer = buffer2;
            positionPtr = &position2;
	    sizePtr = &size2;
        }
        if (a == 1) {
            buffer = buffer1;
            positionPtr = &position1;
	    sizePtr = &size1;
        }
        return true;
    }

    /**
     * @brief Switches from another stream.
     * @param a is the name of the desired stream.
     * @return true. */
    bool UnBufferedSwitch(const char* a) {
        if (*a == '1') {
            positionPtr = &position1;
            sizePtr = &size1;
            buffer = buffer1;
        }
        if (*a == '2') {
            positionPtr = &position2;
	    sizePtr = &size2;
            buffer = buffer2;
        }
        return true;
    }

    /**
     * @brief Removes a stream.
     * @param a is the name of the desired stream to be removed.
     * @return true. */
    bool UnBufferedRemoveStream(const char* a) {
        UnBufferedSwitch((*a - '0') % 2 + 1);
        nBuffers--;
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

    /** 
     * @brief Returns the number of avaiable buffers.
     * @return the number of avaiable buffers. */
    uint32 NOfStreams() {
        return nBuffers;
    }

    /**
     * @brief Returns the name of the buffer.
     * @param a unused.
     * @param some unused.
     * @param b unused.
     * @return false. */
    bool StreamName(uint32 a, char* some, int b) const {
        return false;
    }

    /** 
     * @brief Returns the number of the selected stream.
     * @return the number of the selected stream. */
    uint32 SelectedStream() {
        if (buffer == buffer1)
            return 1;
        else
            return 2;
    }

    /**
     * @brief Adds a buffer.
     * @param h unused.
     * @return true. */
    bool AddStream(const char* h) {
        nBuffers++;
        return true;
    }

};

/**
 * @brief Class used for streamable tests.
 *
 * Inherits from streamable, implements pure virtual functions and allows a relation with an StreamWrapperIOBuffer.*/
class SimpleStreamable: public Streamable {

public:
    /** the current stream position. */
    uint32 size;
    /** the stream */
    char buffer[MAX_STREAM_DIMENSION];
    /** if true, GetInputBuffer returns the StreamWrapperIOBuffer. */
    bool seekFlag;
    /** the related StreamWrapperIOBuffer. */
    StreamWrapperIOBuffer outputBuff;
    /** the buffer associated to the StreamWrapperIOBuffer. */
    char window[64];
protected:

    /**
     * @brief Returns the input buffer.
     * @return the StreamWrapperIOBuffer if seekFlag is true, NULL otherwise. */
    IOBuffer* GetInputBuffer() {
        if (seekFlag) {
            return &outputBuff;
        }
        return NULL;
    }

    /** 
     * @brief Returns NULL. */
    IOBuffer* GetOutputBuffer() {
        return NULL;
    }

public:

    /**
     * @brief Default constructor. Initialize also the StreamWrapperIOBuffer.
     * @param flag is true if you want to get the StreamWrapperIOBuffer, false if you want to get NULL. */
    SimpleStreamable(bool flag = false) :
            outputBuff(this, window, sizeof(window)) {
        for (int32 i = 0; i < MAX_STREAM_DIMENSION; i++)
            buffer[i] = 0;
        size = 0;
        seekFlag = flag;
    }

    /**
     * @brief Clean the stream.
     */
    void Clear() {
        size = 0;
        for (int32 i = 0; i < MAX_STREAM_DIMENSION; i++)
            buffer[i] = 0;
    }

    /** 
     * @brief Syncronize the stream position.
     * @return true if successful. */
    bool Sync() {
        if (seekFlag)
            return outputBuff.Resync();
        else
            return true;
    }

    /** 
     * @brief Read from the stream to an output buffer.
     * @param outBuffer is the output buffer.
     * @param inSize is the number of bytes to read.
     * @param timeout is unused here.
     * @param complete is unused here.
     * @return true if successful, false otherwise. */
    bool Read(char* outBuffer, uint32 &inSize, TimeoutType timeout = TTDefault,
              bool complete = false) {

        if ((size + inSize) >= MAX_STREAM_DIMENSION) {
            size = 0;
        }

        if (!MemoryCopy(outBuffer, &buffer[size], inSize)) {
            return false;
        }

        size += inSize;
        return true;

    }

    /** 
     * @brief Write from a buffer to the stream.
     * @param inBuffer is the input buffer.
     * @param outSize is the number of bytes to write.
     * @param timeout is unused here.
     * @param complete is unused here.
     * @return true if successful, false otherwise. */
    bool Write(const char* inBuffer, uint32 &outSize, TimeoutType timeout =
                       TTDefault,
               bool complete = false) {

        if ((size + outSize) >= MAX_STREAM_DIMENSION) {
            size = 0;
        }
        if (!MemoryCopy(&buffer[size], inBuffer, outSize)) {
            return false;
        }
        size += outSize;
        return true;
    }

    /** 
     * @brief Move to an absolute position.
     * @param seek is the desired position.
     * @return true. */
    bool Seek(int64 seek) {
        size = seek;
        return true;
    }

    /**	
     * @brief Move from the current position.
     * @param delta is the gap from the current position.
     * @return true. */
    bool RelativeSeek(int32 delta) {
        size += delta;
        return true;
    }

    /**
     * @brief Can write on the stream.
     * @return true. */
    bool CanWrite() const {
        return true;
    }

    /**
     * @brief Can seek on the stream.
     * @return true. */
    bool CanSeek() const {
        return true;
    }

    /**
     * @brief Can read on the stream.
     * @return true. */
    bool CanRead() const {
        return true;
    }

    /** 
     * @brief Returns the stream size.
     * @return the stream size. */
    int64 Size() {
        return MAX_STREAM_DIMENSION;
    }

    /**
     * @brief Returns the stream position.
     * @return the current stream position. */
    int64 Position() {
        return size;
    }

    /**
     * @brief Sets the stream size.
     * @param s is unused here.
     * @return true. */
    bool SetSize(int64 s) {
        return True;
    }
};

