/**
 * @file ReferencesExample6.cpp
 * @brief Source file for class ReferencesExample6
 * @date 14/03/2018
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
 * the class ReferencesExample6 (public, protected, and private). Be aware that some
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
#include "ClassRegistryItemT.h"
#include "ConfigurationDatabase.h"
#include "ErrorLoggerExample.h"
#include "IntrospectionT.h"
#include "Matrix.h"
#include "Object.h"
#include "ObjectRegistryDatabase.h"
#include "Reference.h"
#include "ReferenceT.h"
#include "ReferenceContainerFilterReferences.h"
#include "StandardParser.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe2Tutorial {

class ObjectEx1: public MARTe::Object {
public:
	CLASS_REGISTER_DECLARATION()

	/**
	 * @brief NOOP.
	 */
	ObjectEx1 () {
	}

	virtual ~ObjectEx1() {
		using namespace MARTe;
		if (GetName() != NULL) {
			REPORT_ERROR_STATIC(ErrorManagement::Information, "No more references "
					"pointing at %s [%s]. The Object will "
					"be safely deleted.", GetName(), GetClassProperties()->GetName());
		}
	}

    virtual bool Initialise(MARTe::StructuredDataI &data) {
        using namespace MARTe;
        bool ok = Object::Initialise(data);
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Initialise called for %s", GetName());
        return ok;
    }

    MARTe::Reference FindParent() {
        using namespace MARTe;
        //Print the path to the root
        //First find myself
        Reference thisRef(this);
        ReferenceContainer results;
        ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, thisRef);
        ObjectRegistryDatabase::Instance()->ReferenceContainer::Find(results, filter);
        REPORT_ERROR_STATIC(ErrorManagement::Information, "For object %s found %d objects", GetName(), results.Size());
        for (uint32 n=0; n<results.Size(); n++) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Parent [%d] name: %s", n, results.Get(n)->GetName());
        }
        Reference parent;
        if (results.Size() > 1u) {
            parent = results.Get(results.Size() - 2u);
        }
        return parent;
    }

};
CLASS_REGISTER(ObjectEx1, "")
}
int main(int argc, char **argv) {
	using namespace MARTe;
	using namespace MARTe2Tutorial;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    StreamString configurationCfg = ""
            "+GrandParent = {\n"
            "    Class = ReferenceContainer\n"
            "    +Parent1 = {\n"
            "        Class = ReferenceContainer\n"
            "        +Child1= {\n"
            "            Class = ObjectEx1\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+Parent2 = {\n"
            "    Class = ReferenceContainer\n"
            "    +Child2 = {\n"
            "        Class = ObjectEx1\n"
            "    }\n"
            "}";

    REPORT_ERROR_STATIC(ErrorManagement::Information, "Loading CFG:\n%s", configurationCfg.Buffer());
    ConfigurationDatabase cdb;
    StreamString err;
    //Force the string to be seeked to the beginning.
    configurationCfg.Seek(0LLU);
    StandardParser parser(configurationCfg, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        //After parsing the tree is pointing at the last leaf
        cdb.MoveToRoot();
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    else {
        StreamString errPrint;
        errPrint.Printf("Failed to parse %s", err.Buffer());
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, errPrint.Buffer());
    }

    if (ok) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Successfully loaded the configuration file");
    }

    ReferenceT<ObjectEx1> child1 = ObjectRegistryDatabase::Instance()->Find("GrandParent.Parent1.Child1");
    if (child1.IsValid()) {
        Reference parent = child1->FindParent();
        if (parent.IsValid()) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Object [%s] => Parent [%s]", child1->GetName(), parent->GetName());
        }
    }

    ReferenceT<ObjectEx1> child2 = ObjectRegistryDatabase::Instance()->Find("Parent2.Child2");
    if (child2.IsValid()) {
        Reference parent = child2->FindParent();
        if (parent.IsValid()) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Object [%s] => Parent [%s]", child2->GetName(), parent->GetName());
        }
    }

	return 0;
}

