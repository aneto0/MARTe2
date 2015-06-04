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

#include "BasicString.h"
#include "GeneralDefinitions.h"

#if 0 // not needed for BasicString

bool BSRead(BasicString &s, char* buffer, uint32 position, uint32 &size) {
    // adjust position
    if (position > s.Size())
        position = s.Size();  // append
    //
    uint32 sizeLeft = s.Size() - position;
    if (sizeLeft == 0) {
        size = 0;
        return False;
    }
    if (size > sizeLeft)
        size = sizeLeft;
    memcpy(buffer, &(s.buffer[position]), size);
//    position += size;
    return True;
}

#endif

bool BSWrite(BString &s, const char* buffer, uint32 position, uint32 &copySize) {
    // adjust position
    if (position > s.Size())
        position = s.Size();  // append
    // adjust size
    uint32 neededSize = position + copySize;
    uint32 maxSize = s.BufferAllocatedSize() - 1;
    if (neededSize > maxSize) s.SetStringBufferSize(neededSize);

    // recalculates actual memory availability
    maxSize = s.BufferAllocatedSize() - 1;

    // just in case check if that worked
    uint32 sizeLeft = maxSize - position ;
    if (copySize > sizeLeft)
        copySize = sizeLeft;
    // size + 1 as the CopyBuffer does not include the terminator automatically 
    CharBuffer::CopyBuffer(&(s.buffer[position]),buffer, copySize+1);
    
    // updates BString
    s.size = position + copySize;

    return True;
}

