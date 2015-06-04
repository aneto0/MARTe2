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

#include "BString.h"
#include "GeneralDefinitions.h"
#include "Memory.h"

const int32 StringGranularity = 0x00000020;
const int32 StringGranularityMask = 0xFFFFFFE0;

bool FSAlloc(BString &s, int32 allocSize) {
    allocSize++; // +1 to accomodate string terminator
    int32 deltaSize = allocSize - s.allocatedSize;
    if ((deltaSize <= 0) && (deltaSize >= -10 * StringGranularity))
        return True;

    // at least a granule
    int32 newSize = allocSize;
    newSize += StringGranularity - 1;
    newSize &= StringGranularityMask;

    char *newBuffer;
    if (s.buffer == NULL) {
        newBuffer = (char *) MemoryMalloc(newSize);
        newBuffer[0] = 0;
    }
    else {
        newBuffer = (char *) MemoryRealloc((void * &) s.buffer, newSize);
    }
    if (newBuffer == NULL)
        return False;
    s.buffer = newBuffer;
    s.allocatedSize = newSize;
    return True;
}

bool FSRead(BString &s, void* buffer, uint32 &size) {
    // adjust position
    if (s.position > s.Size())
        s.position = s.Size();  // append
    //
    uint32 sizeLeft = s.Size() - s.position;
    if (sizeLeft == 0) {
        size = 0;
        return False;
    }
    if (size > sizeLeft)
        size = sizeLeft;
    memcpy(buffer, &(s.buffer[s.position]), size);
    s.position += size;
    return True;
}

bool FSWrite(BString &s, const void* buffer, uint32 &size) {
    // adjust position
    if (s.position > s.Size())
        s.position = s.Size();  // append
    // adjust size
    uint32 neededSize = s.position + size;
    if ((neededSize + 1) > s.allocatedSize)
        FSAlloc(s, neededSize);
//    s.SetSize(neededSize);

    // just in case check if that worked
    uint32 sizeLeft = s.allocatedSize - s.position - 1;
    if (size > sizeLeft)
        size = sizeLeft;
    memcpy(&(s.buffer[s.position]), buffer, size);

    s.position += size;
    if (s.size < s.position)
        s.size = s.position;
    s.buffer[s.position] = 0;

    return True;
}

