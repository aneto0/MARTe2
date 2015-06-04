/*
 * Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 express or implied.
 * See the Licence
 permissions and limitations under the Licence.
 *
 * $Id: $
 *
 **/
/** 
 * @file 
 * @brief A portable C stream mechanism. */
#if !defined (CSTREAM_H)
#define CSTREAM_H

#include "GeneralDefinitions.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>




struct CStream;

/** Define the function called by NewBuffer */
typedef void (*CStreamNewBufferFN)(CStream *p);

/** @brief  A portable C stream mechanism.
  *
  * It provides the base infrastructure for the most advanced streaming classes
  * (e.g. File, Socket and FString) to do printf like operations. */

struct CStream {

    void * context;

    /** Pointer to the buffer for read/write ops. */
    char * bufferPtr;

    uint32 sizeLeft;

    CStreamNewBufferFN NewBuffer;
};

static inline bool CPut(CStream * p, char c) {
    if (p->sizeLeft == 0) {
        p->NewBuffer(p);
        if (p->sizeLeft == 0)
            return False;
    }
    p->sizeLeft--;
    p->bufferPtr[0] = c;
    (p->bufferPtr)++;
    return True;
}

static inline bool CGet(CStream * p, char & c) {
    if (p->sizeLeft == 0) {
        p->NewBuffer(p);
        if (p->sizeLeft == 0)
            return False;
    }
    p->sizeLeft--;
    c = *(p->bufferPtr)++;
    return True;
}

extern "C" {

/** @brief Read 'size' bytes from the CStream cs to the buffer.
  * @param cs is the stream.
  * @buffer is where datas must be copied.
  * @return false if the size left is zero, so a minor number of bytes is copied. */
bool CRead(CStream *cs, void *buffer, uint32 &size);

/** @brief Write 'size' bytes from the buffer cs to the CStream.
  * @param cs is the stream.
  * @buffer is the buffer which provide datas.
  * @return false if the size left is zero on the CStream. */
bool CWrite(CStream *cs, const void *buffer, uint32 &size);


/** @brief Write a 32 bit integer in the stream.
  * @param cs is the stream.
  * @param n is the integer to print.
  * @param desiredSize is the minimum size of chars to print on the stream.
  * @param desiredPadding is the char printed in all spaces added by DesiredSize.
  * @param mode could be i-d (decimal) || u (unsigned decimal) || x-X-p (exadecimal) || o (octal).
  * @return true. */
bool CPrintInt32(CStream *cs, int32 n, uint32 desiredSize = 0,
                 char desiredPadding = 0, char mode = 'i');

/** @brief Write a 64 bit integer in the stream.
  * @param cs is the stream.
  * @param n is the integer to print.
  * @param desiredSize is the minimum size of chars to print on the stream.
  * @param desiredPadding is the char printed in all spaces added by DesiredSize.
  * @param mode could be i-d (decimal) || x-X-p (exadecimal) || o (octal).
  * @return true. */
bool CPrintInt64(CStream *cs, int64 n, uint32 desiredSize = 0,
                 char desiredPadding = 0, char mode = 'i');

/** @brief Write a double in the stream.
  * @param cs is the stream.
  * @param ff is the double to print.
  * @param desiredSize is the minimum size of chars to print on the stream.
  * @param desiredSubSize is the desired number of decimals after the point.
  * @param desiredPadding is the char printed in all spaces added by DesiredSize.
  * @param mode could be f (normal notation) || e (scientific notation).
  * @return true. */
bool CPrintDouble(CStream *cs, double ff, int desiredSize = 0,
                  int desiredSubSize = 6, char desiredPadding = 0, char mode =
                          'f');

/** @brief Write a string in the stream.
  * @param cs is the stream.
  * @param s is the string to print.
  * @param desiredSize is the minimum size of chars to print on the stream.
  * @param desiredPadding is the char printed in all spaces added by DesiredSize.
  * @param rightJustify -> if true padding is added on the right, if false it is added on the left.
  * @return false only if the string in the argument is NULL. */
bool CPrintString(CStream *cs, const char *s, uint32 desiredSize = 0,
                  char desiredPadding = 0, bool rightJustify = True);



/** @brief Perform a portable printf function on the stream.
  * @param cs is the stream.
  * @param format is the printf format.
  * @param argList is a list of the parameters to print.
  * @return false only if format is NULL.
  * The Supported format flags type (%<pad><size><type>)
  * can be (o d i X x Lo Ld Li LX Lx f e s c). */
bool VCPrintf(CStream *cs, const char *format, va_list argList);

/** @brief Create the va_list and call VCPrintf 
  * @see VCPrintf. 
  * @return false only if format is NULL. */
bool CPrintf(CStream *cs, const char *format, ...);

/** @brief Get a token from the stream.
  * @param cs is the stream.
  * @param buffer is the location where the token is put.
  * @param terminator contains the chars used as delimiters.
  * @param saveTerminator save the terminator in each token operation.
  * @param skip contains the delimiters which can be skipped if read consecutively.
  * If skip is NULL all consecutive delimiters are skipped.
  * @return false if no data are read from the stream. */
bool CGetToken(CStream *cs, char *buffer, const char *terminator,
               uint32 maxSize, char *saveTerminator = NULL, const char *skip =
               NULL);

/** @brief Get a token from the stream and write it on another stream using a string as delimiter.
  * @param cs is the input stream.
  * @param out is the output stream.
  * @param terminator is the delimiter string.
  * @return false if no data are read from the stream. */
bool CGetStringToken(CStream *cs, CStream *out, const char *terminator,
                     uint32 totalSize);

/** @brief Skip a desired number of terminators. Consecutive terminators are calculated as one.
  * @param cs is the stream.
  * @param count is the number of terminator to skip.
  * @param contains char terminators which must be skipped.
  * @return false if no data are read from the stream. */
bool CSkipTokens(CStream *cs, uint32 count, const char *terminator);

/** @brief Get a token from a buffer and write it on a buffer using char delimiters.
  * @param input is the input stream.
  * @param buffer is where data are written.
  * @param terminator contains the delimiter chars.
  * @return false if no data are read from the stream. */
bool CGetCStringToken(const char *&input, char *buffer, const char *terminator,
                      uint32 maxSize);

/** @brief Break the input string where a char contained in terminator is found.
  * @param input is the input string.
  * @param terminator contains the chars used as delimiters.
  * @param saveTerminator is a pointer to the delimiter char.
  * @param skip contains the delimiters which can be skipped if read consecutively.
  * If skip is NULL all consecutive delimiters are skipped.
  * @return the pointer over the delimiter for a possible next operation. */
char *CDestructiveGetCStringToken(char *&input, const char *terminator,
                                  char *saveTerminator = NULL,
                                  const char *skip = "");

/** @brief Get a token from a stream and write it in another stream using chars as delimiters.
  * @param csIn is the input stream.
  * @param csOut is the output stream.
  * @param terminator contains the chars used as delimiters.
  * @param saveTerminator is a pointer to the delimiter char.
  * @param skip contains the delimiters which can be skipped if read consecutively.
  * If skip is NULL all consecutive delimiters are skipped.
  * @return false if no data are read from the stream. */
bool CGetCSToken(CStream *csIn, CStream *csOut, const char *terminator,
                 char *saveTerminator = NULL, const char *skip = NULL);

/** @brief Perform the printf operation on a buffer.
  * @param buffer is the buffer where data must be printed.
  * @param format is the printf format.
  * @param arglist is the list of objects to print.
  * @return 1. */
int bl2_vsprintf(char *buffer, const char *format, va_list argList);

/** @brief Create the list and call vsprintf.
  * @param buffer is the buffer where data must be written.
  * @param format is the printf format.
  * @return 1. */
int bl2_sprintf(char *buffer, const char *format, ...);

/** @brief Perform the printf operation on a buffer with a maximum size.
  * @param buffer is the buffer where data must be printed.
  * @param format is the printf format.
  * @param arglist is the list of objects to print.
  * @return 1. 
  * size is the number of bytes of the printed string, incluse the terminated char.*/
int bl2_vsnprintf(char *buffer, size_t size, const char *format,
                  va_list argList);

/** @brief Create the list and call vsnprintf.
  * @param buffer is the buffer where data must be written.
  * @param format is the printf format.
  * @return 1. */
int bl2_snprintf(char *buffer, size_t size, const char *format, ...);
}

#endif

