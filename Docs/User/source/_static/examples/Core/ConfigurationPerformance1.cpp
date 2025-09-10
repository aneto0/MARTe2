/**
 * @file ConfigurationExample8.cpp
 * @brief Source file for class ConfigurationExample8
 * @date 27/06/2022
 * @author Andre' Neto
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

 * @details This source file contains the definition of all the methods for
 * the class ConfigurationExample8 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ClassRegistryDatabase.h"
#include "ConfigurationDatabase.h"
#include "ErrorLoggerExample.h"
#include "Matrix.h"
#include "Object.h"
#include "Reference.h"
#include "ReferenceT.h"
#include "StreamString.h"
#include "StructuredDataIHelper.h"
#include "Vector.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

int main(int argc, char **argv) {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    uint64 now = HighResolutionTimer::Counter();
    for(uint32 b=0; b<68; b++) {
        StreamString n1;
        n1.Printf("%d", b);
        cdb.CreateAbsolute(n1.Buffer());
        for(uint32 i=0; i<10024; i++) {
            StreamString n2;
            n2.Printf("%d", i);
            cdb.CreateRelative(n2.Buffer());
            cdb.Write("FullType", "uint32");
            cdb.Write("NumberOfDimensions", 1);
            cdb.Write("NumberOfDimensions1", 1);
            cdb.Write("NumberOfDimensions2", 1);
            cdb.Write("NumberOfDimensions3", 1);
            cdb.Write("NumberOfDimensions4", 1);
            cdb.Write("NumberOfDimensions5", 1);
            cdb.Write("NumberOfElements", 1);
            StreamString qn;
            qn.Printf("aaa.bbb.cc.ddd.eee.{%d}.{%d}", b, i);
            cdb.Write("QualifiedName", qn.Buffer());
            cdb.Write("MemberSize", 4);
            cdb.MoveToAncestor(1u);
        }
        cdb.MoveToAncestor(1u);
        printf("Node [%d]\n", b);
    }
    uint64 end = HighResolutionTimer::Counter();
    uint64 tdiff = end - now;
    float64 ttime = tdiff * HighResolutionTimer::Period();
    printf("Took [%e] seconds\n", ttime);
    return 0;
}

