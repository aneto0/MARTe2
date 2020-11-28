/**
 * @file ConfigurationExample8.cpp
 * @brief Source file for class ConfigurationExample8
 * @date 08/04/2018
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
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ClassRegistryDatabase.h"
#include "ClassRegistryItem.h"
#include "ConfigurationDatabase.h"
#include "ErrorLoggerExample.h"
#include "ObjectRegistryDatabase.h"
#include "Reference.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe2Tutorial {
    void PrintNumberOfInstances(const MARTe::char8 *className) {
        using namespace MARTe;
        ClassRegistryDatabase *crd = ClassRegistryDatabase::Instance();
        const ClassRegistryItem *cri = crd->Find(className);
        if (cri != NULL_PTR(const ClassRegistryItem *)) {
            const ClassProperties *cp = cri->GetClassProperties();
            if (cp != NULL_PTR(const ClassProperties *)) {
                REPORT_ERROR_STATIC(ErrorManagement::Information, "[%s] - instances: %d", cp->GetName(), cri->GetNumberOfInstances());
            }
        }
    }

    void PrintNumberOfInstancesCDB(MARTe::uint32 callNumber) {
        using namespace MARTe;
        REPORT_ERROR_STATIC(ErrorManagement::Information, "----------------------------------[%d]----------------------------------", callNumber);
        //List all the classes and the related number of instances that are still alive
        //PrintNumberOfInstances("ConfigurationDatabase"); 
        PrintNumberOfInstances("ConfigurationDatabaseNode"); 
        PrintNumberOfInstances("AnyObject"); 
    }

    void PrintCDB(MARTe::ConfigurationDatabase &cdb){
        using namespace MARTe;
        StreamString pp;
        ConfigurationDatabase cdbp = cdb;
        cdbp.MoveToRoot();
        pp.Printf("%!", cdbp);
        printf("\n=========================================================\n");
        printf("%s", pp.Buffer());
        printf("=========================================================\n");
    }

    void ConfigurationDatabaseFun1(MARTe::ConfigurationDatabase &cdb) {
        using namespace MARTe;
        ConfigurationDatabase cdb2 = cdb;
        cdb2.MoveAbsolute("A.B");
        cdb2.CreateAbsolute("C.D");
        cdb2.Write("e", 4);
        ConfigurationDatabase cdb3;
        cdb3.CreateAbsolute("A.A");
        cdb3.Write("o", 0);
        cdb3 = cdb2;
        cdb3.MoveAbsolute("C.D");
        cdb3.Write("f", 5);
    }

    void ConfigurationDatabaseFun2(MARTe::ConfigurationDatabase &cdb) {
        using namespace MARTe;
        ConfigurationDatabase cdb2;
        cdb2.CreateAbsolute("F.G");
        cdb2.Write("h", 4);
        cdb = cdb2;
        PrintCDB(cdb);
    }

    void ConfigurationDatabaseRun() {
        using namespace MARTe;
        ConfigurationDatabase cdb;
        cdb.CreateAbsolute("A.B");
        cdb.Write("c", 1);
        PrintNumberOfInstancesCDB(1);
        ConfigurationDatabaseFun1(cdb);
        PrintNumberOfInstancesCDB(2);
        PrintCDB(cdb);
        ConfigurationDatabaseFun2(cdb);
        PrintCDB(cdb);
        PrintNumberOfInstancesCDB(3);
    }
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

int main(int argc, char **argv) {
    using namespace MARTe;
    using namespace MARTe2Tutorial;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    PrintNumberOfInstancesCDB(0);
    ConfigurationDatabaseRun();
    PrintNumberOfInstancesCDB(4);
    return 0;
}

