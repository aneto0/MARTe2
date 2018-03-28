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
#include "ErrorLoggerExample.h"
#include "Object.h"
#include "ObjectRegistryDatabase.h"
#include "Reference.h"
#include "ReferenceContainer.h"
#include "ReferenceT.h"
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
ObjectEx1	() {
	}

	virtual ~ObjectEx1() {
		using namespace MARTe;
		if (GetName() != NULL) {
			REPORT_ERROR_STATIC(ErrorManagement::Information, "No more references "
					"pointing at %s [%s]. The Object will "
					"be safely deleted.", GetName(), GetClassProperties()->GetName());
		}
	}

	void AddParent(MARTe::Reference ref) {
		anotherContainer.Insert(ref);
	}

	/**
	 * A private container. This should be avoided!
	 */
	MARTe::ReferenceContainer anotherContainer;

};

CLASS_REGISTER(ObjectEx1, "")

/**
 * Does not implement the Purge method and as such a cyclic link will not be broken.
 */
class ContainerEx1: public MARTe::ReferenceContainer {
public:
	CLASS_REGISTER_DECLARATION()

	/**
	 * @brief NOOP.
	 */
ContainerEx1	() {
	}

	virtual ~ContainerEx1() {
		using namespace MARTe;
		REPORT_ERROR_STATIC(ErrorManagement::Information, "No more references "
				"pointing at %s [%s]. The Object will "
				"be safely deleted.", GetName(), GetClassProperties()->GetName());
	}

	void SetAReference(MARTe::Reference aReferenceIn) {
		aReference = aReferenceIn;
	}

	MARTe::Reference aReference;
};
CLASS_REGISTER(ContainerEx1, "")

/**
 * Implements the Purge method.
 */
class ContainerEx2: public ContainerEx1 {
public:
	CLASS_REGISTER_DECLARATION()

	/**
	 * @brief NOOP.
	 */
ContainerEx2	() {
	}

	virtual ~ContainerEx2() {
		using namespace MARTe;
		REPORT_ERROR_STATIC(ErrorManagement::Information, "No more references "
				"pointing at %s [%s]. The Object will "
				"be safely deleted.", GetName(), GetClassProperties()->GetName());
	}

	/**
	 * Destroys an existent links
	 */
	virtual void Purge(MARTe::ReferenceContainer &purgeList) {
		using namespace MARTe;
		//Destroy the circular link
		aReference = Reference();
		//Alternative which would also destroy the link
		/*ReferenceT<ObjectEx1> ref = Get(0);
		if (ref.IsValid()) {
			ref->anotherContainer.Purge();
		}*/
		//Do not forget to call the parent implementation
		ReferenceContainer::Purge(purgeList);
	}

};
CLASS_REGISTER(ContainerEx2, "")

}

int main(int argc, char **argv) {
	using namespace MARTe;
	using namespace MARTe2Tutorial;
	SetErrorProcessFunction(&ErrorProcessExampleFunction);

	CCString className1 = "ObjectEx1";
	CCString className2 = "ContainerEx1";
	CCString className3 = "ContainerEx2";

	//Automatically generate a new object instance based on the class
	//name and on the correct Heap and with the template reference.
	ReferenceT<ObjectEx1> ref1(className1,
			GlobalObjectsDatabase::Instance()->GetStandardHeap());
	if (ref1.IsValid()) {
		ref1->SetName("ObjectExIntance1");
		REPORT_ERROR_STATIC(ErrorManagement::Information, "Successfully "
				"created an instance of %s", className1.GetList());
	}
	ReferenceT<ObjectEx1> ref2(className1,
			GlobalObjectsDatabase::Instance()->GetStandardHeap());
	if (ref2.IsValid()) {
		ref2->SetName("ObjectExIntance2");
		REPORT_ERROR_STATIC(ErrorManagement::Information, "Successfully "
				"created an instance of %s", className1.GetList());
	}
	ReferenceT<ContainerEx1> ref3(className2,
			GlobalObjectsDatabase::Instance()->GetStandardHeap());
	if (ref3.IsValid()) {
		ref3->SetName("ContainerExIntance1");
		ref3->Insert(ref1);
		//This will create a circular dependency
		ref3->SetAReference(ref1);
		ref1->AddParent(ref3);
		REPORT_ERROR_STATIC(ErrorManagement::Information, "Successfully "
				"created an instance of %s", className2.GetList());

	}
	ReferenceT<ContainerEx2> ref4(className3,
			GlobalObjectsDatabase::Instance()->GetStandardHeap());
	if (ref4.IsValid()) {
		ref4->SetName("ContainerExIntance2");
		ref4->Insert(ref2);
		ref4->SetAReference(ref2);
		ref2->AddParent(ref4);
		REPORT_ERROR_STATIC(ErrorManagement::Information, "Successfully "
				"created an instance of %s", className3.GetList());
	}

	if (ref3.IsValid()) {
		ref3->Purge();
	}
	if (ref4.IsValid()) {
		ref4->ReferenceContainer::Purge();
	}

	if (ref1.IsValid() && ref2.IsValid() && ref3.IsValid() && ref4.IsValid()) {
		REPORT_ERROR_STATIC(ErrorManagement::Information, "%s "
				"number of references: %d", ref1->GetName(),
				ref3->NumberOfReferences());
		REPORT_ERROR_STATIC(ErrorManagement::Information, "%s "
				"number of references: %d", ref2->GetName(),
				ref2->NumberOfReferences());
		REPORT_ERROR_STATIC(ErrorManagement::Information, "%s "
				"number of references: %d", ref3->GetName(),
				ref3->NumberOfReferences());
		REPORT_ERROR_STATIC(ErrorManagement::Information, "%s "
				"number of references: %d", ref4->GetName(),
				ref4->NumberOfReferences());

		if (ref1->NumberOfReferences() == 2u
				&& ref3->NumberOfReferences() == 2u) {
			REPORT_ERROR_STATIC(ErrorManagement::Warning,
					"As expected %s and %s have a circular dependency and thus cannot be destroyed."
					, ref1->GetName(), ref3->GetName());
		}
	}
	return 0;
}

