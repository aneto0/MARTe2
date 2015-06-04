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
 * $Id: Endianity.h 3 2012-01-15 16:26:07Z aneto $
 *
 **/

#include <stdio.h>
#include "Processor.h"
#include "FastMath.h"
#include "HighResolutionTimer.h"
#include "Endianity.h"
#include "EndianityTest.h"
#include "MutexSem.h"
#include "TimerTest.h"
#include "MutexTest.h"
#include "ThreadsDatabaseTest.h"

void Success(bool ret, const char *file, uint32 line, const char *command) {
    if (!ret) {
        printf("***************************\n");
        printf("File %s Line %i\n", file, line);
        printf("***** %s failed\n", command);
        printf("***************************\n");
    }
    else {
        printf("%s OK\n", command);
    }
}

#define ExpectSuccess(x)       Success((x),__FILE__,__LINE__,#x);

int main(int argc, char **argv) {
    ThreadsDatabaseTest mt;
    mt.TestRemoveEntry(10);
//        mt.DeadLock();
}

