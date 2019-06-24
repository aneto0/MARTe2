/**
 * @file SimpleStructuredData.cpp
 * @brief Header file for class AnyType
 * @date 24 Jun 2019
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#include "SimpleStructuredData.h"
#include "AnyType.h"
namespace MARTe{


ErrorManagement::ErrorType SimpleStructuredData::Write(CCString name, const AnyType &value){
	ErrorManagement::ErrorType ret;
	Reference newvalue;
	value.Clone(newvalue);
	newvalue->SetName(name);
	db.Delete(name);
	db.Insert(newvalue);
	return ret;
}

ErrorManagement::ErrorType SimpleStructuredData::Read(CCString path,const AnyType &value){
	ErrorManagement::ErrorType ret;

	Reference savedValue = db.Find(path);
	AnyType at;
	savedValue.ToAnyType(at);
	at.CopyTo(value);

	return ret;

}

CLASS_REGISTER(SimpleStructuredData, "1.0")


} //MARTe
