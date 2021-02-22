/**
 * @file ThreadsInformationQuery.cpp
 * @brief Source file for class ThreadsInformationQuery
 * @date 30/03/2019
 * @author Andre Neto
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
 * the class ThreadsInformationQuery (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "StreamString.h"
#include "Threads.h"
#include "ThreadsInformationQuery.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
ThreadsInformationQuery::ThreadsInformationQuery() : Object() {

}

ThreadsInformationQuery::~ThreadsInformationQuery() {

}

bool ThreadsInformationQuery::ExportData(StructuredDataI & data) {
    bool ok = Object::ExportData(data);
    uint32 nOfThreads = Threads::NumberOfThreads();
    uint32 n;
    for (n = 0u; (n < nOfThreads) && (ok); n++) {
        ThreadIdentifier tinfo;
        if (ok) {
            tinfo = Threads::FindByIndex(n);
        }
        if (tinfo != 0u) {
            StreamString idx;
            const uint32 nn = n;
            ok = idx.Printf("%d", nn);
            if (ok) {
                ok = data.CreateRelative(idx.Buffer());
            }
            if (ok) {
                StreamString tname = Threads::Name(tinfo);
                ok = data.Write("Name", tname.Buffer());
            }
            if (ok) {
                ProcessorType cpuMask = Threads::GetCPUs(tinfo);
                // TODO implement a correct setter and getter for cpu mask instead of using uint64
                uint32 convertedMask = cpuMask.GetLegacyUint32Mask();
                ok = data.Write("Affinity", convertedMask);
            }
            if (ok) {
                Threads::PriorityClassType pclass = Threads::GetPriorityClass(tinfo);
                //lint -e{930} cast from enum required to be able to Write in StructuredDataI
                ok = data.Write("PriorityClass", static_cast<uint32>(pclass));
            }
            if (ok) {
                Threads::ThreadStateType tstate = Threads::GetState(tinfo);
                //lint -e{930} cast from enum required to be able to Write in StructuredDataI
                ok = data.Write("State", static_cast<uint32>(tstate));
            }
            if (ok) {
                uint8 level = Threads::GetPriorityLevel(tinfo);
                ok = data.Write("PriorityLevel", level);
            }
            if (ok) {
                ok = data.MoveToAncestor(1u);
            }
        }
    }
    return ok;
}
CLASS_REGISTER(ThreadsInformationQuery, "1.0")
}
