#include "HighResolutionTimer.h"
#include "Memory.h"
#include "Threads.h"
#include "stdio.h"

int main() {
    TimeValues timeStamp;
    HighResolutionTimer::GetTimeStamp(timeStamp);
    printf("\n%d %d %d %d %d %d %d\n", timeStamp.microseconds, timeStamp.seconds, timeStamp.minutes, timeStamp.hours, timeStamp.days, timeStamp.month,
           timeStamp.year);

    HighResolutionTimer::GetTimeStamp(timeStamp);
    printf("\n%d %d %d %d %d %d %d\n", timeStamp.microseconds, timeStamp.seconds, timeStamp.minutes, timeStamp.hours, timeStamp.days, timeStamp.month,
           timeStamp.year);

    printf("\nHello World\n");
    //normal memory
    uint32 *array1 = (uint32*) MemoryMalloc(sizeof(uint32) * 10);

    array1[9] = 2;

    Memory::defaultAllocationFlag = MemoryStandardMemory | MemoryAddHeader;

    uint32 *array2 = (uint32*) MemoryMalloc(sizeof(uint32) * 10);

    array2[9] = 3;
    uint32 size;
    TID tid;

    MemoryGetHeaderInfo(array2, size, tid);

    printf("\n2=%d 3=%d 40=%d %d=%d\n", array1[9], array2[9], size, Threads::Id(), tid);

    Memory::defaultAllocationFlag = MemoryStandardMemory;
    MemoryFree((void*&) array1);

    Memory::defaultAllocationFlag = MemoryStandardMemory | MemoryAddHeader;
    MemoryFree((void*&) array2);

    if (Global::errorType == Debug) {
        printf("\nOK\n");
    }
    else {
        printf("\nFail\n");
    }
    return 0;
}
