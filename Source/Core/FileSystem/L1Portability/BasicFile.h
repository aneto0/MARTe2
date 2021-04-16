/**
 * @file BasicFile.h
 * @brief Header file for class BasicFile
 * @date 26/10/2015
 * @author Llorenç Capellà
 * @author Ivan Herrero
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

 * @details This header file contains the declaration of the class BasicFile
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICFILE_H_
#define BASICFILE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "StreamI.h"
#include "HandleI.h"
#include "StreamString.h"
#include INCLUDE_FILE_ENVIRONMENT(FileSystem,L1Portability,ENVIRONMENT,BasicFileProperties.h)

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    /**
     * @brief The BasicFile class is a wrapper for the underlaying operating
     * system file type, which allows to read and write from and to a file.
     *
     * @details This class has two states: open and closed. When the file is
     * opened, this class holds an internal reference to an specific operating
     * system structure, which maintains the status and flags of the file.
     * When the file is closed the class maintains its own status and does not
     * need to wrap any operating system structure. Be aware that when the
     * file is closed, most of the functions are disabled until a file is
     * opened.
     *
     * When the Open() method is called the flags attributes have to be
     * specified. Not all the combination flags are allowed. The following
     * list shows the flags combinations which are NOT possible:
     *    flag = FLAG_APPEND | ACCESS_MODE_R (only) -->(if it happens the FLAG_APPEND is deleted)
     *    flag = FLAG_CREAT | FLAG_CREAT_EXCLUSIVE -->(if it happens the FLAG_CREAT_EXCLUSIVE is deleted)
     *    flag = FLAG_TRUNC | ACCESS_MODE_R (only) -->(if it happens the FLAG_TRUNC is deleted)
     *    flag = FLAG_TRUNC | FLAG_CREAT_EXCLUSIVE -->(if it happens the FLAG_TRUNC is deleted)
     *
     * @invariant
     *    Size() >= 0;
     */
    class DLL_API BasicFile: public virtual StreamI, public HandleI {

    public:

        /**
         * Flag for read only mode
         */
        static const uint32 ACCESS_MODE_R = 0x00000001;

        /**
         * Flag for write only mode
         */
        static const uint32 ACCESS_MODE_W = 0x00000002;

        /**
         * Flag to write automatically at the end of the file. In this mode, before each write the pointer is positioned at the end of the file.
         */
        static const uint32 FLAG_APPEND = 0x00000010;

        /**
         * Flag to create a file. In this mode if the file does not exist it will be created and opened, if the file exist it will be only opened.
         */
        static const uint32 FLAG_CREAT = 0x00000020;

        /**
         * Flag to truncate the file. In this mode if the file exists the contents are deleted and the size of the file is set to 0. This flag is only possible if the file is opened with ACCESS_MODE_W.
         */
        static const uint32 FLAG_TRUNC = 0x00000040;

        /**
         * Flag to create a file. In this mode, if the file does not exist it will be created, but if it already exist the open() will fail.
         */
        static const uint32 FLAG_CREAT_EXCLUSIVE = 0x00000080;

        /**
         * @brief Default constructor
         * @post
         *   not CanRead() &&
         *   not CanWrite() &&
         *   not CanSeek() &&
         *   not IsOpen() &&
         *   Size() == 0 &&
         *   Position() == 0 &&
         *   GetFlags() == 0 &&
         *   GetPathName() == ""
         */
        BasicFile();

        /**
         * @brief Copy constructor
         * @details The copied object will be wrapping the same file.
         */
        BasicFile(const BasicFile &bf);

        /**
         * @brief Copy assignment operator
         * @details The assigned object will be wrapping the same file.
         */
        BasicFile& operator=(const BasicFile &bf);

        /**
         * @brief Destructor.
         */
        virtual ~BasicFile();

        /**
         * @brief Changes the flags of the file.
         * @details When the file is already opened not all the flags can be
         * changed, ONLY the following list can be modified:
         *    FLAG_APPEND
         * The rest of the flags are ignored.
         * @param[in] setFlags indicates the flags changes to perform.
         * @pre IsOpen()
         * @return true if the flags are changed.
         */
        bool SetFlags(const uint32 setFlags);

        /**
         * @brief Gets the flags of the opened file.
         * @details If the file is not opened the returned value is 0.
         * @return An uint32 containing the flags information.
         */
        uint32 GetFlags() const;

        /**
         * @brief Queries if the file is writable.
         * @return true if IsOpen && ((GetFlags() & ACCESS_MODE_W) == ACCESS_MODE_W)
         */
        virtual bool CanWrite() const;

        /**
         * @brief Queries if the file readable.
         * @return true if IsOpen && ((GetFlags() & ACCESS_MODE_R) == ACCESS_MODE_R)
         */
        virtual bool CanRead() const;

        /**
         * @brief Queries if the file is seekable.
         * @return true if IsOpen()
         */
        virtual bool CanSeek() const;

        /**
         * @brief Opens a file indicated by the pathname allowing to access to the contents of a file.
         * @param[in] pathname Indicates the file to be opened.
         * @param[in] flags Indicates the behaviour of the file.
         * @pre
         *    !IsOpen() &&
         *    flags have to be valid
         * @post
         *   (((flags & ACCESS_MODE_R) == ACCESS_MODE_R) => CanRead()) &&
         *   (((flags & ACCESS_MODE_W) == ACCESS_MODE_W)) => CanWrite()) &&
         *   CanSeek() &&
         *   IsOpen() &&
         *   GetPathName() == pathname
         * @return true if the file is open successfully.
         */
        bool Open(const char * pathname, const uint32 flags);

        /**
         * @brief Queries if the file is opened.
         * @return true if the file is opened.
         */
        bool IsOpen() const;

        /**
         * @brief Closes an open file.
         * @pre true
         * @post
         *   not CanRead() &&
         *   not CanWrite() &&
         *   not CanSeek() &&
         *   not IsOpen() &&
         *   Size() == 0 &&
         *   Position() == 0 &&
         *   GetFlags() == 0xFFFFFFFF &&
         *   GetPathName() == ""
         * @return True if the file is closed successfully.
         */
        bool Close();

        /**
         * @brief Reads size characters from a file and updates the position.
         * @param[out] output Is a pointer which indicates where the read characters have to be saved.
         * @param[in,out] size Indicates how many characters have to be read. At the end it is modified with the characters actually read.
         * @pre
         *    IsOpen() &&
         *    CanRead() &&
         *    size >= 0
         * @post
         *    Position() == this'old->Position() + size &&
         *    size is updated with the actual number of bytes read &&
         *    size <= size'old &&
         *    output contains a copy of size bytes read from the file starting at this'old->Position() in the same order.
         * @return false if the read fails.
         */
        virtual bool Read(char8* const output,
                uint32 & size);

        /**
         * @brief Reads size characters from a file and updates the position.
         * @details The timeout is the time waiting for reading, not the time while reading. If the timeout is exceeded the read fails.
         * @param[out] output Is a pointer which indicates where the read characters have to be saved.
         * @param[in,out] size Indicates how many characters have to be read. At the end it is modified with the characters actually read.
         * @param[in] msecTimeout indicates the maximum time that the method can wait for beginning reading.
         * @pre
         *    IsOpen() &&
         *    CanRead() &&
         *    size >= 0
         * @post
         *    Position() == this'old->Position() + size &&
         *    size is updated with the actual number of bytes read &&
         *    size <= size'old &&
         *    output contains a copy of size bytes read from the file starting at this'old->Position() in the same order.
         * @return false if the read fails.
         **/
        virtual bool Read(char8 * const output,
                uint32 & size,
                const TimeoutType &msecTimeout);

        /**
         * @brief Writes size characters to a file and updates the position and the size.
         * @param[in] input Is a pointer which contains the characters to be write to.
         * @param[in,out] size Indicates how many characters have to be written. At the end it is modified with the characters actually written.
         * @pre
         *    IsOpen() &&
         *    CanWrite() &&
         *    size >= 0
         * @post
         *    Position() == this'old->Position() + size &&
         *    size is updated with the actual number of bytes written &&
         *    size <= size'old &&
         *    The file contains at this'old->Position() size bytes copied from input in the same order &&
         *    ((size > 0) && (this'old->Position() + size > this'old->Size())) => Size() == this'old->Position() + size
         * @return false if the write fails.
         */
        virtual bool Write(const char8 * const input,
                uint32 & size);

        /**
         * @brief Writes size characters to a file and updates the position and the size.
         * @details The timeout is the time waiting for writing, not the time while writing. If the timeout is exceeded the write fails.
         * @param[in] input Is a pointer which contains the characters to be write to.
         * @param[in,out] size Indicates how many characters have to be written. At the end it is modified with the characters actually written.
         * @param[in] msecTimeout indicates the maximum time that the method can wait for beginning writing.
         * @pre
         *    IsOpen() &&
         *    CanWrite() &&
         *    size >= 0
         * @post
         *    Position() == this'old->Position() + size
         *    size is updated with the actual number of bytes written &&
         *    size <= size'old &&
         *    The file contains at this'old->Position() size bytes copied from input in the same order &&
         *    ((size > 0) && (this'old->Position() + size > this'old->Size())) => Size() == this'old->Position() + size
         * @return false if the write fails.
         **/
        virtual bool Write(const char8 * const input,
                uint32 & size,
                const TimeoutType &msecTimeout);

        /**
         * @brief Queries the size.
         * @return the number of characters in the file if it succeeds or 0xFFFFFFFF otherwise.
         */
        virtual uint64 Size();

        /**
         * @brief Move the pointer to the specified position.
         * @details The position is relative to the beginning of the file. If the pos is larger than the size of the file the pointer is moved
         * to the end of the file.
         * @pre
         *    IsOpen() &&
         *    CanSeek()
         * @post
         *   pos <= Size() => Position() == pos &&
         *   pos > Size() => Position() == Size()
         * @return true if the pointer points to the position pos.
         */
        virtual bool Seek(const uint64 pos);

        /**
         * @brief Move the pointer to the specified position.
         * @details The function move the pointer deltaPos from the current position. If the deltaPos plus current position is beyond the size of the file the pointer is moved
         * to the end of the file.
         * @pre
         *   IsOpen() &&
         *   CanSeek()
         * @post
         *   this'old->Position() + deltaPos < 0 => Position() == 0 &&
         *   0 <= this'old->Position() + deltaPos <= Size() => Position() == this'old->Position() + deltaPos &&
         *   this'old->Position() + deltaPos > Size() => Position() == Size();
         * @return True if the pointer is is moved to the new position.
         */
        virtual bool RelativeSeek(const int64 deltaPos);

        /**
         * @brief Queries the actual position.
         * @return The actual position of the pointer if it succeeds or 0xFFFFFFFF otherwise.
         */
        virtual uint64 Position();

        /**
         * @brief Changes the size of the file
         * @details If the new size is smaller than the old size the extra data is lost. Instead, if the size is larger than the old size, the size is extended and the gaps are filled with "\0" characters.
         * @pre
         *    IsOpen() &&
         *    CanWrite()
         * @post
         *    Size() == size &&
         *    size > this'old->Position() => From this'old->Size() to Size() - 1 the contents of the file is undefined &&
         *    size < this'old->Size() => this->Position() = size
         * @return true if the size is changed, false otherwise
         */
        virtual bool SetSize(uint64 size);

        /**
         * @brief Queries the pathname of the file
         */
        StreamString GetPathName() const;

        /**
         * @brief Queries the read handle of the file.
         * @details For the BasicFile the read handle and the write handle are the same,
         * however the BasicConsol has two different handles: one for read and one for write.
         * @return The handle independently if the flag is ACCESS_MODE_R or ACCESS_MODE_W.
         */
        virtual Handle GetReadHandle() const;

        /**
         * @brief Queries the write handle of the file.
         * @details For the BasicFile the read handle and the write handle are the same,
         * however the BasicConsol has two different handles: one for read and one for write.
         * @return The handle independently if the flag is ACCESS_MODE_R or ACCESS_MODE_W.
         */
        virtual Handle GetWriteHandle() const;

    private:

        BasicFileProperties properties;
    };
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /*BASICFILE_H_ */
